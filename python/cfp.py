import time
import thread
from fpga import FpgaMdio

REG_MOD_GEN_CTRL = 0xA010
REG_MOD_STATE = 0xA016
REG_MOD_GLB_ALRM = 0xA018
REG_MOD_GEN_STATUS = 0xA01D
REG_MOD_FAULT_STATUS = 0xA01E

cfpDict = {0x11:"CFP2", 0x12:"CFP4", 0x14:"CFP2-ACO", 0x15:"CFP8", 0x19:"CFP2-DCO"}

class Cfp():
    def __init__(self, bus, phyAddr):
        self.bus  = bus
        self.phyAddr = phyAddr
        #PMA/PMD = 1, WIS = 2
        self.devAddr = 1
        self.READY_STATE = False
        self.FAULT_STATE = True
        self.TX_OFF_STATE = True
        self.TX_TURN_ON_STATE = False
    
    def read(self, regAddr):
        return self.bus.mdioRead(self.phyAddr, self.devAddr, regAddr)
    
    def write(self, regAddr, value):
        self.bus.mdioWrite(self.phyAddr, self.devAddr, regAddr, value)
    
    def printReg(self, regAddr, len = 1):
        for i in range(len):
            print "addr 0x%4x"%(regAddr + i),": %4x"%(self.read(regAddr + i))
    
    def identifier(self):
        """
        Returns the module name
        Args:
            None
        Returns:
            Module ID 
        Notes:
            None
        """
        mod_id = self.read(0x8000)
        
        return cfpDict[mod_id]
    
    def state(self):
        '''
        Returns the State of the CFP8 module.

        Args:
            None

        Returns:
            CFP Module State

        Raises:
            None

        Notes:
        '''
        mod_state = self.read(REG_MOD_STATE)
        return mod_state
    
    def info(self):
        '''
        Reads CFP module information registers. Note that module registers can not
        be accessed if the module is in reset or initialization state.

        Args:
            None

        Returns:
            Nothing

        Raises:
            Nothing
        '''
        print "="*60
        print "Module ID                       :\t%s"%(self.identifier())  
        print "Extended ID                     :\t%04x"%(self.read(0x8001))

        #If Application Code registers read 0x00 the documentation specifies that this value is undefined
        eth_app_code = self.read(0x8003)
        if eth_app_code == 0x00:
            print "Ethernet Application Code      :\tUndefined"
        else:
            print "Ethernet Appplication Code      :\t%02x"%(eth_app_code)
        sonet_sdh_app_code = self.read(0x8006)
        if sonet_sdh_app_code == 0x00:
            print "SONET/SDH Application Code      :\tUndefined"
        else:
            print "SONET/SDH Application Code      :\t%02x"%(sonet_sdh_app_code)
        otn_app_code = self.read(0x8007)
        if otn_app_code == 0x00:
            print "OTN Application Code            :\tUndefined"
        else:
            print "OTN Application Code            :\t%02x"%(otn_app_code)

        #Number of network and host lanes are help in the upper and lower 4 bits of the register respectively
        num_lanes = self.read(0x8009)
        num_host_lanes = num_lanes & 0x0f
        if num_host_lanes == 0:
            num_host_lanes = 16
        num_lanes = num_lanes >> 4
        num_network_lanes = num_lanes & 0x0f
        if num_network_lanes == 0:
            num_network_lanes = 16
        print "Number of Network Supported     :\t%d"%(num_network_lanes)
        print "Number of Host Supported        :\t%d"%(num_host_lanes)

        #If bit rate registers read 0x00 the documentation specifies that this value is undefined
        max_network_lane_bit_rate = self.read(0x800b)
        if max_network_lane_bit_rate == 0x00:
            print "Maximum Network Lane Bit Rate   :\tUndefined"
        else:
            print "Maximum Network Lane Bit Rate   :\t0.2 Gbps x %d"%(max_network_lane_bit_rate)
        max_host_lane_bit_rate = self.read(0x800c)
        if max_host_lane_bit_rate == 0x00:
            print "Maximum Host Lane Bit Rate      :\tUndefined"
        else:
            print "Maximum Host Lane Bit Rate      :\t0.2 Gbps x %d"%(max_host_lane_bit_rate)
  
        print "Vendor Name                     :\t%s"%(self.str_read(0x8021, length=16))
        print "Vendor Part Number              :\t%s"%(self.str_read(0x8034, length=16))
        print "Vendor Serial Number            :\t%s"%(self.str_read(0x8044, length=16))
        print "Vendor Date Code                :\t%s"%(self.str_read(0x8054, length=8))

        #Version numbers stored in two registers as x.y where x represents the register at the lower address and y
        #at the higher address
        #[2:] --> slices of the '0x' in the hex numbers returned by the read() functions
        hw_version_num = str(self.read(0x806a))[2:]+'.'+str(self.read(0x806b))[2:]
        print "Module Hardware Version Number  :\t%s"%(hw_version_num)
        fw_version_num = str(self.read(0x806c))[2:]+'.'+str(self.read(0x806d))[2:]
        print "Module Firmware Version Number  :\t%s"%(fw_version_num)
        print "="*60    

    def status(self):
        '''
        Queries CFP module status pins and status registers. Note that module registers 
        can not be accessed if the module is in reset or initialization state

        Args:
            None

        Returns:
            Nothing

        Raises:
            Nothing
        '''


        #Get number of network lanes
        num_lanes = self.read(0x8009)
        num_host_lanes = num_lanes & 0x0f
        if num_host_lanes == 0:
            num_host_lanes = 16
        num_lanes = num_lanes >> 4
        num_network_lanes = num_lanes & 0x0f
        if num_network_lanes == 0:
            num_network_lanes = 16


        print "Module State Latch                           :\t%04x"%(self.read(0xa022))
        print "Global Alarm Summary                         :\t%04x"%(self.read(REG_MOD_GLB_ALRM))
        print "Module General Status                        :\t%04x"%(self.read(REG_MOD_GEN_STATUS))
        print "Module General Status Latch                  :\t%04x"%(self.read(0xa023))
        print "Module Fault Status                          :\t%04x"%(self.read(REG_MOD_FAULT_STATUS))
        print "Module Fault Status Latch                    :\t%04x"%(self.read(0xa024))
        print "Module Alarms and Warnings 1                 :\t%04x"%(self.read(0xa01f))
        print "Module Alarms and Warnings 1 Latch           :\t%04x"%(self.read(0xa025))
        print "Module Alarms and Warnings 2                 :\t%04x"%(self.read(0xa020))
        print "Module Alarms and Warnings 2 Latch           :\t%04x"%(self.read(0xa026))
        
        print "-"*60
        print "Network Lane Alarm/Warning Summary : %04x"%(self.read(0xa019))
        print "%41s%15s}"%("Lane", "State")
        for i in range(num_network_lanes):
            print "%40d\t\t%04x"%(i, self.read((0xa200 + i)))
        print "Network Lane Alarm/Warning Latches"
        print "%41d%4x"%("Lane", "State")
        for i in range(num_network_lanes):
            print "%40d\t\t%04x"%(i, self.read((0xa220 + i)))

        print "-"*60
        print "Network Lane Fault/Status Summary : {}"%(self.read(0x0A01a))
        print "%41d%4x"%("Lane", "State")
        for i in range(num_network_lanes):
            print "%40d\t\t%04x"%(i, self.read((0xa210 + i)))
        print "Network Lane Fault/Status Latches"
        print "%41d%4x"%("Lane", "State")
        for i in range(num_network_lanes):  
            print "%40d\t\t%04x"%(i, self.read((0xa230 + i)))

        print "-"*60
        print "Host Lane Fault and Status Summary : {}"%(self.read(0x0A01b))
        print "%41d%4x"%("Lane", "State")
        for i in range(num_host_lanes):
            print "%40d\t\t%04x"%(i, self.read((0xa400 + i)))
        print "Host Lane Fault/Status Latches"
        print "%41d%4x"%("Lane", "State")
        for i in range(num_host_lanes):
            print "%40d\t\t%04x"%(i, self.read((0xa410 + i)))
        print "="*70   

    def txon(self):
        '''
        Turns on the CFP8 module Transmitter ON and leaves it in the Ready State.

        Args:
            None

        Returns:
            True  - if the module's transmitter turn on is success
            False - otherwise

        Raises:
            TimeoutError

        Notes:
            CFP module either enables or disables lanes according to the configuration in
            individual Network Lane TX_DIS Control CFP register.
            CFP8 transmitter cannot be turned on if CFP8 is not powered up or if there is any fault.
        '''
        fault_state = False
        error_count = 0

        # 1. Check the module state
        mod_state = self.state()
        if mod_state == self.READY_STATE:
            print "CFP is already in Ready State..."
            return True
        elif mod_state == self.FAULT_STATE:
            print "CFP is in Fault State. Cannot Turn-On the Transmitter."
            error_count += 1
        elif mod_state != self.TX_OFF_STATE:
            print "CFP is not in TX-OFF State. See if the CFP8 is powered up."
            return False
        else:
            print "Turning ON the CFP8 Transmitters..."

            # 2. Assert Hard TX_DIS.
            # self.reg.write('DFPGA_TOP.CFP8_CTRL({}).CFP8_TX_DIS'%(self.device_idx), 0)

            # 3. CFP8 enters TX-Turn-On state.
            #   In this transient state, CFP module either enables or disables lanes according
            #   to the configuration in individual Network Lane TX_DIS Control CFP register.

            # 4. Upon successfully turning-on the desired transmitters, CFP8 asserts MOD_READY
            #   and enters the Ready State.
            #   If the turning-on the transmitters fail due to any faults, CFP8 enters Fault State
            #   and deasserts MOD_READY.
            timeout = time.time() + 20
            mod_state = self.state()
            while mod_state != self.READY_STATE:
                mod_state = self.state()
                if mod_state == self.FAULT_STATE:
                    print("CFP is in Fault State. TX-ON Failed.")
                    fault_state = True
                elif mod_state == self.TX_TURN_ON_STATE:
                    print """CFP is in Tx-Turn-on State. \
                                       Waiting for transition to Ready State"""
                if time.time() > timeout:
                    print "Timeout for the CFP module to enter TX-OFF State"


            # 5. Return True if CFP8 enters Ready state successfully without any faults
            return not fault_state

    def prg_cntl1(self):
        '''
        Reads the register 0xA007[7:0] which specifies the function selection for
        PRG_CNTL1. 

        Args:
            None

        Returns:
            A string representing the selected function of PRG_CNTL1

        Raises:
            None
        '''
        reg = self.read(0xa007)
        reg = reg & 0x0f
        func = "No Active"
        if reg == 0x01:
            func = "TRXIC_RSTn"
        elif reg == 0x0a:
            func = "TX_DIS"
        return func

    def prg_alrm1(self):
        '''
        Reads the register 0xA00A[7:0] which specifies the function selection for
        PRG_ALRM1. 

        Args:
            None

        Returns:
            A string representing the selected function of PRG_CNTL1

        Raises:
            None
        '''
        reg = self.read(0xa00a)
        reg = reg & 0x0f
        func = "Not Active"
        if reg == 0x01:
            func = "HIPWR_ON"
        elif reg == 0x02:
            func = "Ready State"
        elif reg == 0x03:
            func = "Fault State"
        elif reg == 0x04:
            func = "RX_ALRM"
        elif reg == 0x05:
            func = "TX_ALRM"
        elif reg == 0x06:
            func = "RX_NETWORK_LOL"
        elif reg == 0x07:
            func = "TX_LOSF"
        elif reg == 0x08:
            func = "TX_HOST_LOL"
        elif reg == 0x09:
            func = "Out of Alignment"
        elif reg == 0x0a:
            func = "RX_LOS"
        return func

    def str_read(self, addr, length):
        '''
        Performs an MDIO read of the CFP8 module.  Designed to read specific registers 
        that provide vendor information in ASCII code which is left aligned and padded
        on the right with ASCII spaces (0x20)

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
            data = self.read((addr + i))
            hex_list.append(str(data)[2:])
        
        new_str = ''
        for item in hex_list:
            if item != '20':
                item = int(item, 16)
                new_str = new_str+chr(item)

        return new_str
