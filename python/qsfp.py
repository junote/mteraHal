import time
from hal import log10
from misc import hexdump


class Qsfp28():
    def __init__(self, bus, dev_addr=0xa0):
        self.bus = bus
        self.dev_addr = dev_addr

    def read(self, addr):
        # write page first
        self.bus.write8(self.dev_addr, 0x7f, int(addr/0x100))
        return self.bus.read8(self.dev_addr, (addr % 0x100))

    def printReg(self, addr, len=1):
        # write page first, and only in 1 page
        if (addr % 0x100 + len) > 0xff:
            print "only print in 1 page"
            return
        self.bus.write(self.dev_addr, 0x7f, int(addr/0x100))
        for i in range(len):
            print "addr 0x%x"%(addr + i),": 0x%x"%self.read(addr)

    def dump_page(self, page_no):
        self.write(0x7f, page_no)
        tmp =[]
        for i in range(256):
            tmp.append(self.read(i))
        hexdump(0, tmp)

    def write(self, addr, data):
        self.bus.write(self.dev_addr, 0x7f, int(addr/0x100))
        self.bus.write(self.dev_addr, (addr % 0x100), data)

    def info(self):
        '''
        Gathers information about a module currently connected to the board.
        :return:
        '''
        # Ensure reading from 0x00 upper page using byte 127 (page select)
        self.write(addr=127, data=0)

        print "="*60
        print "Module ID                          :\t"%self.read(addr=0)
        print "Extended ID                        :\t"%self.read(addr=129)
        print "Ethernet Compliance Code           :\t"%self.read(addr=131)
        print "SONET Compliance Code              :\t"%self.read(addr=132)
        print "SAS/SATA Compliance Code           :\t"%self.read(addr=133)
        print "Gigabit Ethernet Compliance Code   :\t"%self.read(addr=134)
        print "Encoding value                     :\t"%self.encoding_mechanism()
        print "Rate Selection Support             :\t"%self.rate_sel_support()
        print "Extended Rate Selection Support    :\t"%self.ext_rate_sel_support()
        print "Vendor Name                        :\t"%self.str_read(addr=148, length=16)
        print "Vendor Part Number                 :\t"%self.str_read(addr=168, length=16)
        print "Vendor Serial Number               :\t"%self.str_read(addr=196, length=16)
        print "Vendor Date Code                   :\t"%self.str_read(addr=212, length=8)
        print "="*60

    def status(self):
        '''
        Gathers status about a QSFP module connected to the board.
        :return:
        '''
        print "="*60
        print "Temperature     :\t"%self.get_temp()
        print "Supply Voltage  :\t"%self.get_voltage()
        print "TX Power Alarm/Warning  :\t"%self.tx_power_alarm_warning_int()
        print "RX Power Alarm/Warning:\t"%self.rx_power_alarm_warning_int()
        print "TX Bias Alarm/Warning :\t{}"%self.tx_bias_alarm_warning_int()

        print "-"*50
        print "%20s%20s%20s"%("Channel", "TX Power", "Power(dbm)")
        for ch in range(1, 5):
            print "%5d"%ch,
            print "%010x"%(self.ch_tx_power(ch)),
            print "%0.2f"%(10*log10(self.ch_tx_power(ch)))
        print "-"*50
        print "%20s%20s%20s"%("Channel", "RX Power", "Power(dbm)")
        for ch in range(1, 5):
            print "%5d"%ch,
            print "%010x"%(self.ch_rx_power(ch)),
            print "%0.2f"%(10*log10(self.ch_rx_power(ch)))
        print "-"*50
        print "%20s%20s%20s"%("TX LOS Indicator", "Channel", "State")
        for ch in range(1, 5):
            print "%30d%20d"%(ch, self.ch_status_int_tx_los_indicator(ch_no=ch))
        print "-"*50
        print "%20s%20s%20s"%("RX LOS Indicator", "Channel", "State")
        for ch in range(1, 5):
            print "%30d%20d"%(ch, self.ch_status_int_rx_los_indicator(ch_no=ch))
        print "-"*50
        print "%20s%20s%20s"%("TX Loss of Lock Indicator", "Channel", "State")
        for ch in range(1, 5):
            print "%30d%20d"%(ch, self.ch_status_int_tx_lol_indicator(ch_no=ch))
        print "-"*50
        print "%20s%20s%20s"%("RX Loss of Lock Indicator", "Channel", "State")
        for ch in range(1, 5):
            print "%30d%20d"%(ch, self.ch_status_int_rx_lol_indicator(ch_no=ch))
        print "-"*50
        ##################
        # Changed due to Address 0x01 reading 0x00 - Undefined revision compliance
        # Fault indicator addressing slightly different in older versions
        #
        # for ch in range(1,5):
        #    print "Ch. {0} TX Adaptive EQ Fault Indicator   :\t{1}".format(ch, self.ch_status_int_tx_adapt_eq_fault_indicator(ch_no=ch)))
        # Changed from RX Adaptive EQ Fault Indicator to TX Fault Indicator (older version)
        print "%20s%20s%20s"%("TX Fault Indicator", "Channel", "State")
        for ch in range(1, 5):
            print "%20d%20d"%(ch, self.ch_status_int_rx_adapt_eq_fault_indicator(ch_no=ch))
        print "="*60

    def encoding_mechanism(self):
        '''
        Indicates if the encoding algorithm based on Table 4-2 in SFF-8024

        Args:
            None

        Returns:
            (str) - Encoding mechanism

        Raises:
            None

        Notes:
        '''
        enc = self.read(addr=139)
        mec = "Undefined"
        if enc == 0x01:
            mec = "8B/10B"
        elif enc == 0x02:
            mec = "4B/5B"
        elif enc == 0x03:
            mec = "NRZ"
        elif enc == 0x04:
            mec = "SONET Scrambled"
        elif enc == 0x05:
            mec = "64B/66B"
        elif enc == 0x06:
            mec = "Manchester"
        elif enc == 0x07:
            mec = "256B/257B"
        elif enc == 0x08:
            mec = "PAM4"

        return mec

    def rate_sel_support(self):
        '''
        Shows the availability of Rate Selection support feature

        Args:
            None

        Returns:
            True - if the Rate Selection support feature is available
            False - otherwise

        Raises:
            None

        Notes:
        '''
        return (self.read(addr=221) & 0x0C == 0) and (self.read(addr=195) & 0x20 == 0)

    def ext_rate_sel_support(self):
        '''
        Shows the availability of Extended Rate Selection support feature

        Args:
            None

        Returns:
            True - if the Extended Rate Selection support feature is available
            False - otherwise

        Raises:
            None

        Notes:
        '''
        return (self.read(addr=221) & 0x08 == 0x08) and (self.read(addr=141) != 0)

    def rate_sel_rx(self, val):
        '''
        Software Rate Select for the Rx Channels in the format:
        (Rx4-MSB, Rx4-LSB, Rx3-MSB, Rx3-LSB, Rx2-MSB, Rx2-LSB, Rx1-MSB, Rx1-LSB)

        Args:
            val (int): Integer that represents the value to be set for the Rate Select

        Returns:
            None

        Raises:
            None

        Notes:
            (MSB, LSB) = (0, 0) for the bit rates less than 2.2 Gbps
                       = (0, 1) for the bit rates from 2.2 to 6.6 Gbps
                       = (1, 0) for the bit rates from 6.6 Gbps and above
                       = (1, 1) Reserved. No effect.
        '''
        self.write(addr=87, data=val)

    def rate_sel_tx(self,  val):
        '''
        Software Rate Select for the Rx Channels in the integer converted format:
        (Tx4-MSB, Tx4-LSB, Tx3-MSB, Tx3-LSB, Tx2-MSB, Tx2-LSB, Tx1-MSB, Tx1-LSB)

        Args:
            val (int): Integer that represents the value to be set for the Rate Select

        Returns:
            None

        Raises:
            None

        Notes:
            (MSB, LSB) = (0, 0) for the bit rates less than 2.2 Gbps
                       = (0, 1) for the bit rates from 2.2 to 6.6 Gbps
                       = (1, 0) for the bit rates from 6.6 Gbps and above
                       = (1, 1) Reserved. No effect.
        '''
        self.write(addr=88, data=val)

    def tx_ch_disable(self,  ch_no):
        '''
        Disables the corresponding Optical Tx channel.

        Args:
            ch_no (int): Integer that selects one of the four Tx channels

        Returns:
            None

        Raises:
            Warning if Power Override is disabled

        Notes:
            None
        '''
        rdata = self.read(addr=86)
        mdata = 2**(ch_no-1)
        self.write(addr=86, data=rdata | mdata)

    def set_low_power_mode(self):
        '''
        Configures the QSFP28 in Low Power Mode, if Power Override is not disabled.

        Args:
            None

        Returns:
            None

        Raises:
            Warning if Power Override is disabled

        Notes:
            None
        '''
        rdata = self.read(addr=93)
        if rdata & 0x01 != 0:
            wdata = rdata | 0x02
            self.write(addr=93, data=wdata)
        else:
            print "Power Override is disabled."

    def ch_status_int_los_indicator(self):
        '''
        Returns the LOS Indicator interrupt status register bits

        Args:
            None

        Returns:
            LOS Indicator interrupt status register

        Raises:
            None

        Notes:
        '''
        return self.read(addr=3)

    def ch_status_int_tx_los_indicator(self, ch_no):
        '''
        Returns the LOS Indicator interrupt status of the corresponding Tx channel

        Args:
            ch_no (int): Integer that represent one of the four Tx channel number

        Returns:
            True - if the Tx LOS indicator interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        los_ind = self.ch_status_int_los_indicator()
        if los_ind & 0xF0 == 0:
            tx_los_ind = False
        else:
            tx_los_ind = los_ind & 2**(ch_no+3) != 0

        return tx_los_ind

    def ch_status_int_rx_los_indicator(self,  ch_no):
        '''
        Returns the LOS Indicator interrupt status of the corresponding Rx channel

        Args:
            ch_no (int): Integer that represent one of the four Rx channel number

        Returns:
            True - if the Rx LOS indicator interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        los_ind = self.ch_status_int_los_indicator()
        if los_ind & 0x0F == 0:
            rx_los_ind = False
        else:
            rx_los_ind = los_ind & 2**(ch_no-1) != 0

        return rx_los_ind

    def ch_status_int_lol_indicator(self):
        '''
        Returns the LOL Indicator interrupt status register bits

        Args:
            None

        Returns:
            LOL Indicator interrupt status register

        Raises:
            None

        Notes:
        '''
        return self.read(addr=5)

    def ch_status_int_tx_lol_indicator(self,  ch_no):
        '''
        Returns the LOL Indicator interrupt status of the corresponding Tx channel

        Args:
            ch_no (int): Integer that represent one of the four Rx channel number

        Returns:
            True - if the Tx LOL indicator interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        lol_ind = self.ch_status_int_lol_indicator()
        if lol_ind & 0xF0 == 0:
            tx_lol_ind = False
        else:
            tx_lol_ind = lol_ind & 2**(ch_no+3) != 0

        return tx_lol_ind

    def ch_status_int_rx_lol_indicator(self,  ch_no):
        '''
        Returns the LOL Indicator interrupt status of the corresponding Rx channel

        Args:
            ch_no (int): Integer that represent one of the four Rx channel number

        Returns:
            True - if the Rx LOL indicator interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        lol_ind = self.ch_status_int_lol_indicator()
        if lol_ind & 0x0F == 0:
            rx_lol_ind = False
        else:
            rx_lol_ind = lol_ind & 2**(ch_no-1) != 0

        return rx_lol_ind

    def ch_status_int_adapt_eq_fault_indicator(self):
        '''
        Returns the Adaptive Eq Fault Indicator interrupt status register bits

        Args:
            None

        Returns:
            Adaptive Eq Fault Indicator interrupt status register

        Raises:
            None

        Notes:
        '''
        return self.read(addr=4)

    def ch_status_int_tx_adapt_eq_fault_indicator(self,  ch_no):
        '''
        Returns the Adaptive Eq Fault Indicator interrupt status of the corresponding Tx channel

        Args:
            ch_no (int): Integer that represent one of the four Rx channel number

        Returns:
            True - if the Tx Adaptive Eq Fault indicator interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        adapt_eq_fault_ind = self.ch_status_int_adapt_eq_fault_indicator()
        if adapt_eq_fault_ind & 0xF0 == 0:
            tx_adapt_eq_fault_ind = False
        else:
            tx_adapt_eq_fault_ind = adapt_eq_fault_ind & 2**(ch_no+3) != 0

        return tx_adapt_eq_fault_ind

    def ch_status_int_rx_adapt_eq_fault_indicator(self,  ch_no):
        '''
        Returns the Adaptive Eq Fault Indicator interrupt status of the corresponding Rx channel

        Args:
            ch_no (int): Integer that represent one of the four Rx channel number

        Returns:
            True - if the Rx Adaptive Eq Fault indicator interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        adapt_eq_fault_ind = self.ch_status_int_adapt_eq_fault_indicator()
        if adapt_eq_fault_ind & 0x0F == 0:
            rx_adapt_eq_fault_ind = False
        else:
            rx_adapt_eq_fault_ind = adapt_eq_fault_ind & 2**(ch_no-1) != 0

        return rx_adapt_eq_fault_ind

    def rx_power_alarm_warning_int(self):
        '''
        Shows the Rx Channel Power related Alarm/Warning Interrupts

        Args:
            None

        Returns:
            True - if any of the Power Alarm/Warning interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        reg_9 = self.read(addr=9)
        reg_10 = self.read(addr=10)
        return not(reg_9 == 0x00 and reg_10 == 0x00)

    def tx_power_alarm_warning_int(self):
        '''
        Shows the Tx Channel Power related Alarm/Warning Interrupts

        Args:
            None

        Returns:
            True - if any of the Power Alarm/Warning interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        reg_13 = self.read(addr=13)
        reg_14 = self.read(addr=14)
        return not(reg_13 == 0x00 and reg_14 == 0x00)

    def tx_bias_alarm_warning_int(self):
        '''
        Shows the Tx Channel Bias related Alarm/Warning Interrupts

        Args:
            None

        Returns:
            True - if any of the Bias Alarm/Warning interrupt field is high
            False - otherwise

        Raises:
            None

        Notes:
        '''
        reg_11 = self.read(addr=11)
        reg_12 = self.read(addr=12)
        return not(reg_11 == 0x00 and reg_12 == 0x00)

    def get_temp(self):
        return ((self.read(22) << 8) + self.read(23))/256

    def get_voltage(self):
        return (self.read(26) << 8) + self.read(27)/1000000

    def str_read(self,  addr, length):
        '''
        Performs an I2C read of the QSFP28 module.  Designed to read specific
        registers that provide information in ASCII code which is left aligned
        and padded on the right with ASCII spaces (0x20)

        Args:
            addr (int): register address to read
            length (int): number of registers the information is stored in (size)

        Returns:
            (str) ASCII converted information

        Raises:
            None
        '''
        hex_list = []
        for i in range(length):
            data = self.read(addr=(addr + i))
            hex_list.append(data)

        new_str = ''
        for item in hex_list:
            if item != 32:
                new_str = new_str+chr(item)

        return new_str

    def ch_tx_power(self, ch):
        reg1 = self.read(50 + (ch-1)*2)
        reg2 = self.read(51 + (ch-1)*2)
        return ((reg1 << 8) + reg2)*0.0001

    def ch_rx_power(self, ch):
        reg1 = self.read(34 + (ch-1)*2)
        reg2 = self.read(35 + (ch-1)*2)
        return ((reg1 << 8) + reg2)*0.0001

    def ch_tx_bias(self, ch):
        reg1 = self.read(42 + (ch-1)*2)
        reg2 = self.read(43 + (ch-1)*2)
        return ((reg1 << 8) + reg2)*2
