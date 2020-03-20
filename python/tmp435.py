# from hal import I2cDev

TEMPERATURE_RESOLUTION = 0.065
EXTENDED_OFFSET = 64


class Tmp435():
    def __init__(self, bus, devAddr):
        # self.bus_no = bus_no
        self.devAddr = devAddr
        # self.bus = I2cDev(self.bus_no, self.devAddr)
        self.bus = bus

    def read(self, regAddr):
        return self.bus.read8(self.devAddr, regAddr)

    def write(self, regAddr, value):
        self.bus.write8(self.devAddr, regAddr, value)

    def init(self):
        # soft reset
        self.write(0xfc, 1)
        # set extend range mode
        self.write(0x03, (self.read(0x3) | 0x4))
        # set conversion rate 1 Hz
        self.write(0x0a, 4)

    def disp_temp(self):
        local_temp = self.read(0) - EXTENDED_OFFSET
        local_temp += (self.read(0x15) >> 4) * TEMPERATURE_RESOLUTION
        remote_temp = self.read(1) - EXTENDED_OFFSET
        remote_temp += (self.read(0x10) >> 4) * TEMPERATURE_RESOLUTION
        print "local temperature: ",  "%3.2f" % local_temp, "degC"
        print "remote temperature: ",  "%3.2f" % remote_temp, "degC"

    def dump_reg(self):
        print "TMP435 readable registers:"
        print "Local Temp High Byte               [00h]: ", "0x%02x"%(self.read(0x00))
        print "Remote Temp High Byte              [01h]: ", "0x%02x"%(self.read(0x01))
        print "Status Register                    [02h]: ", "0x%02x"%(self.read(0x02))
        print "Configuration 1                    [03h]: ", "0x%02x"%(self.read(0x03))
        print "Conversion Rate                    [05h]: ", "0x%02x"%(self.read(0x05))
        print "Local Temp Hight Limit High Byte   [05h]: ", "0x%02x"%(self.read(0x05))
        print "Local Temp Low Limit High Byte     [06h]: ", "0x%02x"%(self.read(0x06))
        print "Remote Temp Hight Limit High Byte  [07h]: ", "0x%02x"%(self.read(0x07))
        print "Remote Temp Low Limit High Byte    [08h]: ", "0x%02x"%(self.read(0x08))
        print "Remote Temp Low Byte               [10h]: ", "0x%02x"%(self.read(0x10))
        print "Remote Temp Hight Limit Low Byte   [13h]: ", "0x%02x"%(self.read(0x13))
        print "Remote Temp Low Limit Low Byte     [14h]: ", "0x%02x"%(self.read(0x14))
        print "Local Temp Low Byte                [15h]: ", "0x%02x"%(self.read(0x15))
        print "Local Temp Hight Limit Low Byte    [16h]: ", "0x%02x"%(self.read(0x16))
        print "Local Temp Low Limit Low Byte      [17h]: ", "0x%02x"%(self.read(0x17))
        print "N-Correction                       [18h]: ", "0x%02x"%(self.read(0x18))
        print "Remote Therm Limit                 [19h]: ", "0x%02x"%(self.read(0x19))
        print "Configuration 2                    [0Ah]: ", "0x%02x"%(self.read(0x0A))
        print "Channel Mask                       [1Fh]: ", "0x%02x"%(self.read(0x1F))
        print "Local Therm Limit                  [20h]: ", "0x%02x"%(self.read(0x20))
        print "Therm Hysteresis                   [21h]: ", "0x%02x"%(self.read(0x21))
        print "Consecutive Alert                  [22h]: ", "0x%02x"%(self.read(0x22))
        print "Beta Compensation                  [25h]: ", "0x%02x"%(self.read(0x25))
        print "Device ID                          [FDh]: ", "0x%02x"%(self.read(0xFD))
        print "Manufacturer ID                    [FEh]: ", "0x%02x"%(self.read(0xFE))