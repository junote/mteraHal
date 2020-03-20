from hal import MmapDevice
import thread
import time

class MbFpga(MmapDevice):
    def __init__(self):
        MmapDevice.__init__(self, "mbFpga", 0x1500000)
    
    def read(self, regAddr):
        return self.read32(regAddr)
    
    def write(self, regAddr, value):
        self.write32(regAddr, value)

I2cIrq = 0x00
I2cIrqEn = 0x04
I2cCtrl = 0x08
I2cAddr = 0x0c
I2cWrData = 0x10
I2cRdData = 0x14

class FpgaI2c():
    def __init__(self, baseAddr):
        self.master = MbFpga()
        self.baseAddr = baseAddr
    
    def _readReg(self, addr):
        return self.master.read(self.baseAddr + addr)
    
    def _writeReg(self, addr, value):
        self.master.write(self.baseAddr + addr, value)

    # support 8bit addr and 8bit data only
    def i2cRead8(self, txBuf, rxBuf):
        #txbuf[0] = devaddr, txBuf[1]=regaddr
        if len(txBuf) == 2:
            self._writeReg(I2cAddr, (txBuf[0] |1)|(txBuf[1] << 8)) 
        else:
            print "need at least 2 parameters: devaddr regaddr"
            return 
        #clear irq
        self._readReg(I2cIrq)
        #get rxBuf len
        rxLen = len(rxBuf)
        #creat a thread lock
        myLock = thread.allocate_lock()
        data = []
        with myLock:
            while rxLen > 0:
            #i2c go
                self._writeReg(I2cCtrl,1)
                time.sleep(0.5)
                status = self._readReg(I2cIrq)
                if status & 0x01 == 1:
                    if status & 0b1110 == 0:
                        #read data buf
                        data.append(self._readReg(I2cRdData) & 0xff)
                        rxLen -= 1
                    else:
                        print "read error devAddr: 0x%x"%txBuf[0],
                        "regAddr 0x%x"%txBuf[1], "error 0x%x"%status
                else:
                    print "read not complete","read error devAddr: 0x%x"%txBuf[0],
                    "regAddr 0x%x"%txBuf[1], "error 0x%x"%status
                    return
        return data

    # support 8bit addr and data only
    def i2cWrite8(self, txBuf):
        #txbuf[0] = devaddr, txBuf[1]=regaddr
        self._writeReg(I2cAddr, (txBuf[0] |0)|(txBuf[1] << 8)) 
        #clear irq
        self._readReg(I2cIrq)
        #get txBuf len
        txLen = len(txBuf)
        #creat a thread lock
        myLock = thread.allocate_lock()
        with myLock:
            id = 2
            while id < txLen:
            #i2c go
                self._writeReg(I2cWrData,txBuf[id])
                self._writeReg(I2cCtrl,1)
                time.sleep(0.001)
                status = self._readReg(I2cIrq)
                if status & 0x01 == 1:
                    if status & 0b1110 == 0:
                        #read data buf
                        id += 1
                    else:
                        print "write error devAddr: 0x%x"%txBuf[0],
                        "regAddr 0x%x"%txBuf[1],"error 0x%x"%status
                else:
                    print "write not complete","read error devAddr: 0x%x"%txBuf[0],
                    "regAddr 0x%x"%txBuf[1],"error 0x%x"%status
                    return

    
    def read8(self,devAddr, regAddr):
        txBuf = [devAddr, regAddr]
        rxBuf = [0]
        rxBuf = self.i2cRead8(txBuf, rxBuf)
        return rxBuf[0]
    
    def write8(self, devAddr, regAddr, value):
        txBuf = [devAddr, regAddr, value]
        self.i2cWrite8(txBuf)

MdioStart = 0x00
MdioWData = 0x04
MdioFinish = 0x08
MdioRData = 0x0c

class FpgaMdio():
    def __init__(self,baseAddr):
        self.master = MbFpga()
        self.baseAddr = baseAddr
    
    def _readReg(self, addr):
        return self.master.read(self.baseAddr + addr)
    
    def _writeReg(self, addr, value):
        self.master.write(self.baseAddr + addr, value)
    
    def mdioRead(self, phyAddr, devAddr, regAddr,len=1):
        #send regAddr first
        #clear start
        self._writeReg(MdioStart, 0)
        #op<<26|phyAddr<<21|devAddr<16|regAddr
        WData = (0x00 << 0x26)|(phyAddr << 21) |(devAddr << 16)|(regAddr)
        self._writeReg(MdioWData, WData)
        myLock = thread.allocate_lock()
        with myLock:
            now = time.time()
            notFinished = True if self._readReg(MdioFinish) == 0 else 1
            while notFinished and now + 0.01 > time.time():
                notFinished = True if self._readReg(MdioFinish) == 0 else 1
                time.sleep(0.0001)
            #clear start
            self._writeReg(MdioStart, 0)
            if notFinished:
                print "fail to read at baseAddr 0x%x"%self.baseAddr, "phyAddr:0x%x"%phyAddr, \
                    "devAddr: 0x%x"%devAddr, "regAddr: 0x%x"%regAddr
                return 
            else:    
                #get data
                if len == 1:
                    WData = (0x03 << 0x26)|(phyAddr << 21) |(devAddr << 16)|(regAddr)   
                else: 
                    WData = (0x02 << 0x26)|(phyAddr << 21) |(devAddr << 16)|(regAddr)  
                self._writeReg(MdioWData, WData)
                self._writeReg(MdioStart, 1)
                now = time.time()
                notFinished = True if self._readReg(MdioFinish) == 0 else 1
                while notFinished and now + 0.01 > time.time():
                    notFinished = True if self._readReg(MdioFinish) == 0 else 1
                    time.sleep(0.0001)
                if notFinished:
                    print "fail to read at baseAddr 0x%x"%self.baseAddr, "phyAddr:0x%x"%phyAddr, \
                        "devAddr: 0x%x"%devAddr, "regAddr: 0x%x"%regAddr
                    return 
            rdData = []
            if len == 1 :
                self._writeReg(MdioStart, 0)
                return self._readReg(MdioRData & 0xffff)
            else:
                while len > 0:
                    rdData.append(self._readReg(MdioRData) & 0xffffffff)
                    time.sleep(0.001)
                self._writeReg(MdioStart, 0)
                return rdData
                
    def mdioWrite(self, phyAddr, devAddr, regAddr, value):
        #send regAddr first
        #clear start
        self._writeReg(MdioStart, 0)
        #op<<26|phyAddr<<21|devAddr<16|regAddr
        WData = (0x00 << 0x26)|(phyAddr << 21) |(devAddr << 16)|(regAddr)
        self._writeReg(MdioWData, WData)
        myLock = thread.allocate_lock()
        with myLock:
            now = time.time()
            notFinished = True if self._readReg(MdioFinish) == 0 else 1
            while notFinished and now + 0.01 > time.time():
                notFinished = True if self._readReg(MdioFinish) == 0 else 1
                time.sleep(0.0001)
            #clear start
            self._writeReg(MdioStart, 0)
            if notFinished:
                print "fail to read at baseAddr 0x%x"%self.baseAddr, "phyAddr:0x%x"%phyAddr, \
                    "devAddr: 0x%x"%devAddr, "regAddr: 0x%x"%regAddr
                return False
            else:    
                #write value
                WData = (0x03 << 0x26)|(phyAddr << 21) |(devAddr << 16)|(value)   
                self._writeReg(MdioWData, WData)
                self._writeReg(MdioStart, 1)
                now = time.time()
                notFinished = True if self._readReg(MdioFinish) == 0 else 1
                while notFinished and now + 0.01 > time.time():
                    notFinished = True if self._readReg(MdioFinish) == 0 else 1
                    time.sleep(0.0001)
                if notFinished:
                    print "fail to write at baseAddr 0x%x"%self.baseAddr, "phyAddr:0x%x"%phyAddr, \
                        "devAddr: 0x%x"%devAddr, "regAddr: 0x%x"%regAddr, "value:0x%x"%value
                    return False
                self._writeReg(MdioStart, 0)










        

    






        
    
