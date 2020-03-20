from hal import SpiBus
import time
import os

WRITE_ENABLE = 0x06
WRITE_DISABLE = 0x04
READ_ID = 0x9F
READ_STATUS_REG = 0x05
WRITE_STATUS_REG = 0x01
READ_DATA = 0x03
FAST_READ = 0x0B
PAGE_PROGRAM = 0x02
SECTOR_ERASE = 0xD8
BULK_ERASE = 0xC7
DEEP_POWER_DOWN = 0xB9
RELEASE_FROM_DPD = 0xAB
READ_SIGNATURE = 0xAB

Manufacture = {0x20: "STMicroelectonics / Numonyx",
               0x1: "Spansion"}


class SpiFlash():

    def __init__(self, devName, devCS):
        self.bus = SpiBus(devName, devCS)
        self.get_device_id()

    def get_device_id(self):
        cmd =[READ_ID, 0, 0, 0]
        data = [0] * 4
        rx = self.bus.readWrite(cmd,data)
        print("device manufacture:", Manufacture[rx[1]])
        print("capacity:", pow(2, rx[3]), "bytes")

    def readMemory(self,  addr, num_bytes=1):
        tx_bytes = []
        tx_bytes.append(READ_DATA)
        tx_bytes.append((addr >> 16) & 0xff)
        tx_bytes.append((addr >> 8) & 0xff)
        tx_bytes.append((addr >> 0) & 0xff)
        data = [0]*(4 + num_bytes)
        return self.bus.readWrite(tx_bytes, data)[4:]

    def readReg(self,  command, num_bytes=1):
        tx_bytes = [command]
        data = [0]*(1 + num_bytes)
        return self.bus.readWrite(tx_bytes, data)[1:]

    def writeReg(self,  command, value):
        tx_bytes = [command]
        tx_bytes.append(value)
        return self.bus.write(tx_bytes)

    def erase_device(self, timeout=500):
        # must send WREN
        self.bus.write([WRITE_ENABLE])
        # Bulk erase command
        self.bus.write([BULK_ERASE])
        # wait write complete
        timeout = time.time() + timeout
        status = self.readReg(READ_STATUS_REG)[0]
        print "status = ", hex(status)
        while ((status & 0x01) == 0x01):
            status = self.readReg(READ_STATUS_REG)[0]
            if (time.time() > timeout):
                print("timeout when erasing")
                return -1
        return 0

    def do_flash_program(self, file_path, addr=0):
        filesize = os.path.getsize(file_path)
        page_size = 256

        print "erasing....."
        self.erase_device()

        print "programing....."
        with open(file_path, 'rb') as binary_file:
            for prog_addr in range(addr, (addr+filesize), page_size):
                prog_data = list(binary_file.read(page_size))
                self.bus.write([WRITE_ENABLE])
                if prog_addr % (16 * 1024) == 0:
                    print "page programing " , hex(prog_addr)
                tx_bytes = []
                tx_bytes.append(PAGE_PROGRAM)
                tx_bytes.append((prog_addr >> 16) & 0xff)
                tx_bytes.append((prog_addr >> 8) & 0xff)
                tx_bytes.append((prog_addr >> 0) & 0xff)
                tx_bytes += prog_data
                self.bus.write(tx_bytes)
                timeout = time.time() + 1
                status = self.readReg(READ_STATUS_REG)[0]
                while (status & 0x01) == 0x01:
                    status = self.readReg(READ_STATUS_REG)[0]
                    if (time.time() > timeout):
                        print "timeout when progaming" 
                        return -1

        print "verifing........"
        with open(file_path, 'rb') as binary_file:
            for ref_addr in range(addr, (addr+filesize), page_size):
                ref_data = list(binary_file.read(page_size))
                if prog_addr % (16 * 1024) == 0:
                    print "verify programing " , hex(prog_addr)
                rx_bytes = self.readMemory(ref_addr, page_size)
                if ref_data != rx_bytes:
                    print "found error at " , hex(ref_addr)
                    return -1
        
        return 0
