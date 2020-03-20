from pmbus import PMBus

class Ltc2977(PMBus):
    def __init__(self, busId, addr):
        PMBus.__init__(self,busId, addr)


    def status(self):
        print "temperature:\t","%3.2f"%(self.getTempurature()),"degC"
        print "Voltage In:\t","%3.2f"%(self.getVoltageIn()),"v"
        print "="*30, "\n"

        for i in range(8):
            self._writeBytePMBus(0, i)
            print "ch", i, "\t%3.2f"%self.getVoltageOut(), "v"
            print "-" * 20