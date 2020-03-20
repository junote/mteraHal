import time
from hal import I2cDev
from misc import hexdump


class Si534x():
    def __init__(self, bus_no, dev_addr):
        self.bus_no = bus_no
        self.dev_addr = dev_addr
        self.bus = I2cDev(self.bus_no, self.dev_addr)
        self.write(0, 0)
        print "device :", str(hex(self.read(3)))[2:4] + str(hex(self.read(2)))[2:4]
        print "grade:", self.read(4)
        print "device rev: ", self.read(5)
        
    def read(self, addr):
        # write page first
        self.bus.write8(1, int(addr/0x100))
        return self.bus.read8(addr % 0x100)

    def printReg(self, addr, len):
        # write page first, and only in 1 page
        if (addr % 0x100 + len) > 0xff:
            print "only print in 1 page" 
            return
        self.write(1, int(addr/0x100))
        for i in range(len):
            print "%02x" % (addr + i), ": ", "%02x" % (self.read(addr + i))

    def write(self, addr, value):
        self.bus.write8(1, int(addr/0x100))
        self.bus.write8((addr % 0x100), value)

    def write_file(self, file):
        with open(file, "r") as f:
            lines = f.readlines()
        pre = {}
        mid = {}
        post = {}
        index = 0
        for i, line in enumerate(lines):
            if "0x" in line:
                pre[int(line[:6], base=16)] = int(line[7:], base=16)
            if "End configuration preamble" in line:
                index = i
                break
        for i, line in enumerate(lines[(index+1):]):
            if "0x" in line:
                mid[int(line[:6], base=16)] = int(line[7:], base=16)
            if "End configuration registers" in line:
                index = i
                break
        for line in enumerate(lines[(index+1):]):
            if "0x" in line:
                post[int(line[:6], base=16)] = int(line[7:], base=16)
        for addr in pre.keys:
            self.write(addr, pre[addr])
        time.sleep(1)
        for addr in mid.keys:
            self.write(addr, pre[addr])
        for addr in post.keys:
            self.write(addr, pre[addr])

    def dump_page(self, page_no):
        self.write(1, page_no)
        tmp = []
        for i in range(256):
            tmp.append(self.read(i))
        hexdump(0, tmp)
