from hal import I2cBus

class I2cDev(I2cBus):
    def __init__(self, busName):
        I2cBus.__init__(self,busName)

    def read8(self, devAddr, regAddr):
        writeBuf1 = [devAddr, regAddr]
        writeBuf2 = [devAddr]
        readBuf = [0]
        retBuf = self.i2cRepeatStartRead(writeBuf1,writeBuf2,readBuf)
        return retBuf[0]

    def write8(self, devAddr, regAddr, value):
        writeBuf = [devAddr, regAddr, value]
        self.i2cWrite(writeBuf)
