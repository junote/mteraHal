form hal import MmapDevice

class MbCpld(MmapDevice):
    def __init__(self):
        MmapDevice.__init__(self, "mbCpld", 0x1000)
    
    def read(self, regAddr):
        return self.read8(regAddr)
    
    def write(self, regAddr, value):
        self.write8(regAddr, value)

class UpCpld(MmapDevice):
    def __init__(self):
        MmapDevice.__init__(self, "upCpld", 0x1000)
    
    def read(self, regAddr):
        return self.read8(regAddr)
    
    def write(self, regAddr, value):
        self.write8(regAddr, value)
