from hal import I2cBus
from time import sleep
from time import time as now


class At24cx():
    """Generic flash device implementation.
    """

    WRITE_CYCLE_TIME_MAX = 0.005

    DEVICES = {
        # 128k bit AT24C128
        # 256k bit AT24C256
        128 : (64, 2),
        256 : (64, 2),
    }

    def __init__(self, bus_no, dev_addr, size):
        self._cache_size, self._addr_width = self.DEVICES[size]
        self._size = size << 10
        self._cache_mask = self._cache_size-1
        self.dev_addr = dev_addr
        self.bus = I2cBus(bus_no)

    def get_word_size(cls, size):
        return cls.DEVICES[size][1]

    def capacity(self):
        """Get the flash device capacity in bytes"""
        return self._size

    def __len__(self):
        return self._size

    def read(self, address, size):
        if address+size > len(self):
            print 'Out of range'
        # although it should not be necessary, it seems that reading out
        # all bytes at once triggers errors - reason unknown
        # read out reliability is greatly improved with short read sequence
        # we use the same chunk management as with write request to
        # align as much read requests as possible on device pages
        print "Read @ 0x%04x" % (address)
        chunks = []
        # unaligned left hand side
        left = address & self._cache_mask
        csize = self._cache_size
        if left:
            length = csize - left
            chunks.append(self._do_read(address, length))
            offset = length
            address += length
        else:
            offset = 0
        # aligned buffer
        # chunks.append(self._slave.read_from(address, size-offset))
        while offset < size:
            wsize = min(csize, size-offset)
            chunks.append(self._do_read(address, wsize))
            address += wsize
            offset += wsize
        return chunks

    def write(self, address, data):
        if address+len(data) > len(self):
            print 'Out of range'
        size = len(data)
        # unaligned left hand side
        left = address & self._cache_mask
        csize = self._cache_size
        if left:
            length = csize - left
            self._do_write(address, data[:length])
            offset = length
            address += length
        else:
            offset = 0
        # aligned buffer
        while offset < size:
            wsize = min(csize, size-offset)
            self._do_write(address, data[offset:offset+wsize])
            address += wsize
            offset += wsize

    def _do_write(self, address, data):
        print("Write @ 0x%04x %s" % (address, data))
        addr_high = (address & 0xff00) >> 8
        addr_low = address & 0xff
        write_buf = [self.dev_addr, addr_high, addr_low, data]
        self.bus.i2cWrite(write_buf)
        last = now() + self.WRITE_CYCLE_TIME_MAX*4
        while now() < last:
            sleep(self.WRITE_CYCLE_TIME_MAX*4)
            break

            # if self._slave.poll():
            #    break
        else:
            print 'Device did not complete write cycle'

    def _do_read(self, address, size):
        addr_high = (address & 0xff00) >> 8
        addr_low = address & 0xff
        write_buf1 = [self.dev_addr, addr_high, addr_low]
        write_buf2 = [self.dev_addr]
        read_buf = [0] * size
        return self.bus.i2cRepeatStartRead(write_buf1, write_buf2,read_buf)
