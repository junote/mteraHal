from hal import I2cBus


class Pex86xx():

    def __init__(self, bus_no, dev_addr):
        self.bus_no = bus_no
        self.dev_addr = dev_addr
        self.bus = I2cBus(bus_no)

    def read(self, port, reg_addr):
        write_byte = [0] * 4
        # byte1 read command
        write_byte[0] = 4
        # byte2 2:1 station 0 port bit1
        station_sel = (port & 0xc) >> 2
        port_sel = port & 0x3
        reg_high = (reg_addr & 0xc00) >> 10
        reg_low = (reg_addr & 0x3fc) >> 2
        write_byte[1] = (station_sel << 1) + (port_sel >> 1)
        # byte2 bit7 port bit 0 bit5:2 byte enable bit1:0 reg[11:10]
        write_byte[2] = (port_sel << 7) | (0x3c) | (reg_high)
        write_byte[3] = reg_low
        write_buf1 = [self.dev_addr] + write_byte
        write_buf2 = [self.dev_addr]
        read_buf = [0] * 4
        read_buf = self.bus.i2cRepeatStartRead(write_buf1, write_buf2, read_buf)
        return ((read_buf[0]) << 24) + ((read_buf[1]) << 16) + \
            ((read_buf[2]) << 8) + ((read_buf[3]))

    def write(self, port, reg_addr, value):
        write_byte = [0] * 8
        # byte1 read command
        write_byte[0] = 3
        # byte2 2:1 station 0 port bit1
        station_sel = (port & 0xc) >> 2
        port_sel = port & 0x3
        reg_high = (reg_addr & 0xc00) >> 10
        reg_low = (reg_addr & 0x3fc) >> 2
        write_byte[1] = (station_sel << 1) + (port_sel >> 1)
        # byte2 bit7 port bit 0 bit5:2 byte enable bit1:0 reg[11:10]
        write_byte[2] = (port_sel << 7) | (0x3c) | (reg_high)
        write_byte[3] = reg_low
        # data
        write_byte[4] = (value & 0xff000000) >> 24
        write_byte[5] = (value & 0x00ff0000) >> 16
        write_byte[6] = (value & 0x0000ff00) >> 8
        write_byte[7] = value & 0x000000ff
        write_buf = [self.dev_addr] + write_byte
        return self.bus.i2cWrite(write_buf)

