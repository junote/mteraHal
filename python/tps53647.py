from pmbus import PMBus


class Tps53647(PMBus):
    def __init__(self, busId, addr):
        PMBus.__init__(busId, addr)
        self.vr12 = self._readBytePMBus(0xdd) >> 7

    def _decodeVID(self, value):
        if value == 0:
            return 0
        else:
            if self.vr12 == 1:
                return 0.25 + (value - 1) * 0.005
            else:
                return 0.5 + (value - 1) * 0.01

    def _encodeVID(self, value):
        if value == 0:
            return 0
        else:
            if self.vr12 == 1:
                return int(200*(value-0.25)+1)
            else:
                return int(100*(value-0.5)+1)

    def getVoutMax(self):
        vidValue = self._readWordPMBus(0x24) & 0xff
        return(self._decodeVID(vidValue))

    def setVoutMax(self, value):
        vidValue = self._encodeVID(value)
        self._writeWordPMBus(0x24, vidValue)

    def getVoutMarginHigh(self):
        vidValue = self._readWordPMBus(0x25) & 0xff
        return(self._decodeVID(vidValue))

    def setVoutMarginHigh(self, value):
        vidValue = self._encodeVID(value)
        self._writeWordPMBus(0x25, vidValue)

    def getVoutMarginLow(self):
        vidValue = self._readWordPMBus(0x26) & 0xff
        return(self._decodeVID(vidValue))

    def setVoutMarginLow(self, value):
        vidValue = self._encodeVID(value)
        self._writeWordPMBus(0x26, vidValue)

    def getVoutSetting(self):
        vidValue = self._readWordPMBus(0x21) & 0xff
        return(self._decodeVID(vidValue))

    def setVoutSetting(self, value):
        vidValue = self._encodeVID(value)
        self._writeWordPMBus(0x21, vidValue)

    def getVoltageOut(self):
        voltageOutMessage = self._readWordPMBus(0x8B)
        self.voltageOut = self._decodeVID(voltageOutMessage)
        return self.voltageOut

    def status(self):
        print "temperature:\t", "%3.2f"%(self.getTempurature())
        print "Voltage In:\t ", "%3.2f"%(self.getVoltageIn())
        print "Current In:\t",  "%3.2f"%(self.getCurrentIn())
        print "="*30
        print "Voltage Out:\t", "%3.2f"%({self.getVoltageOut())
        print "Current Out:\t", "%3.2f"%({self.getCurrentOut())
