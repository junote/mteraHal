#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
// #include <pthread.h>
#include <unistd.h>
#include <cstdint>

#include "Pm6010.h"

namespace Devices
{
    const uint32_t ilkn_serdes_port_ids[] ={48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71};
    const uint32_t port_serdes_ids[]={0xff, 40, 36, 32, 28, 24, 8, 0, 12, 20};
    const char *digiNames[] =     {"DIGI_0"};
    const char *digiKoDevName[] = { "/dev/digi0"};
    #define LO_TRIB_SLOT_2xODU4 { \
    { 0x000FFFFF,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000 },  /* LO Channel 0 - 5G TS */   \
    { 0x00000000,0x000FFFFF,0x00000000,0x00000000,0x00000000,0x00000000 },  /* LO Channel 1 - 5G TS */   \
}

    Pm6010::Pm6010(uint32_t index):index(index), device_id((index +1)* 0x100000)
    {

        void* mapAddr;
  
        // Open digi device so mmap() can be done against it
        int fd = open(digiKoDevName[index], O_RDWR);
  
        mapAddr = mmap(NULL,                    // requested address to map to
                        0x2000000,         // size of range to map
                        PROT_READ | PROT_WRITE,  // set both read and write permissions
                        MAP_SHARED,              // changes to range are shared
                        fd,                      // file descriptor for this device
                        0);
  
        // The device can close now
        close(fd);


        size_t const bufferLength = 16;
        char dev_name[bufferLength];
        strncpy(dev_name, digiNames[index], bufferLength);
  
        // device configuration
        g5_config_t device_cfg;
        device_cfg.init();
  
        // global configurations
        device_cfg.refclk_rate = G5_REFCLK_RATE_125_MHZ; // according to OSM10T_HW_Arch : System Timing
        device_cfg.device_info.base_address = reinterpret_cast<uint64_t>(mapAddr);
        device_cfg.device_info.name_ptr = dev_name;

        device_cfg.ilkn_cfg.burst_max = G5_ILKN_BURST_MAX_256;
        device_cfg.ilkn_cfg.burst_min = G5_ILKN_BURST_MIN_64; // From DigiG4/OSM4F
        device_cfg.ilkn_cfg.burst_short = G5_ILKN_BURST_SHORT_64; // According to Jericho’s programmers guide: https://ptliswsvn01.intra.coriant.com/svn/DWDM_HW_LIS/mTera/OSM10FT/03_Implementation/00_Components/00_Electrical/05_Traffic_Processors/Jericho/4_ProgrammersGuide/88X7X_88X8X-PG209_coriant.pdf (Page 46)
        device_cfg.ilkn_cfg.rx_header_mode = G5_ILKN_RX_HEADER_NONE; // note from HW team - In OSM4F we had “4B” for header, but in fact we had a hack in digi_adpt_customer.c: adpt_opsa_pkt_size_get() to change the header to 16 bytes at least for the OTN traffic.
        device_cfg.ilkn_cfg.tx_header_mode = G5_ILKN_TX_HEADER_NONE;
        device_cfg.ilkn_cfg.rx_xoff_mode = G5_ILKN_RX_LINK_MODE_HYBRID; // Used when both PIF and CBR sourced traffc is on the Interlaken Interface
    
       // Interlaken Interface configuration
        g5_ilkn_intf_cfg_t *ilkn_intf_cfg_ptr = &device_cfg.ilkn_intf[G5_ILKN_INTF_0]; // G5_ILKN_INTF_0, G5_ILKN_INTF_1 or G5_ILKN_INTF_2
        ilkn_intf_cfg_ptr->metaframe = G5_ILKN_MF_2048; // Length of the Interlaken meta frame which includes the Synchronization, Scrambler State, Skip, and Diagnostic words carried on each Interlaken lane
        ilkn_intf_cfg_ptr->fc_enable = FALSE; // NOT used in mTera
        ilkn_intf_cfg_ptr->fc_cal_length = 300; // Number of entries in the ﬂow control calendar. The range is 1 - 300
    
       // Interlaken Lane configuration
        // note from HW team - This is something I still got to work with BRCM. It is not clear if we will use 24x 25.78G or 20x 28.125 G between DigiG5 and Jericho. As starting point, please keep what you have.
        g5_ilkn_lane_cfg_t *ilkn_lane_ptr = &device_cfg.ilkn_lane[G5_ILKN_INTF_0];
        ilkn_lane_ptr->rate = G5_ILKN_RATE_25G78125;
    
       // ilkn_serdes_port_ids_tx assigns lanes 48-71 (all of bank C)
        memcpy(ilkn_lane_ptr->serdes_port_ids_tx, ilkn_serdes_port_ids, sizeof(ilkn_serdes_port_ids));
    
       // ilkn_serdes_port_ids_rx assigns lanes 48-71 (all of bank C)
        memcpy(ilkn_lane_ptr->serdes_port_ids_rx, ilkn_serdes_port_ids, sizeof(ilkn_serdes_port_ids));

        g5_api_device_init(device_id, device_cfg);
    }

    Pm6010::~Pm6010()
    {
        g5_api_device_uninit(device_id, TRUE);
    }


    void Pm6010::startPrbsLane(uint32_t serdes_port_id, uint8_t rate)
    {
        uint32_t chnl_id = serdes_port_id;
        g5_api_serdes_port_rate_set(device_id,
                                 chnl_id,
                                 serdes_port_id,
                                 rate);
        
        g5_serdes_pin_cfg_t serdes_port_cfg;
            // get the config for the serdes port
        g5_api_serdes_pin_cfg_get(device_id,
                                    serdes_port_id,
                                    &serdes_port_cfg);
        serdes_port_cfg.print("serdes config");

        // get pin status and adaptation objects
        g5_serdes_pin_status_t serdes_port_status;
        g5_api_serdes_pin_status_get(device_id,
                                       serdes_port_id,
                                       &serdes_port_status);
        serdes_port_status.print("pin status");

        // Configure the TX source to come from PRBS gen
        g5_api_serdes_tx_source_set(device_id,
                                     chnl_id,
                                     G5_SERDES_TX_SRC_SHW_PRBS,
                                     0,0);
        
        // Set a PRBS pattern on single pin
        //support prbs31 only
        uint8_t prbs_mode;
        if (rate <= 8)
            prbs_mode = G5_SERDES_PRBS_SHW_NRZ_31;
        else
            prbs_mode = G5_SERDES_PRBS_SHW_PAM4_31Q;
        uint32_t pattern[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        g5_api_serdes_prbs_set(device_id,
                                    serdes_port_id,
                                    TRUE, // generator configuration
                                    FALSE, // single pin
                                    prbs_mode,
                                    pattern);

        g5_api_serdes_prbs_set(device_id,
                                    serdes_port_id,
                                    FALSE, // monitor configuration
                                    FALSE, // single pin
                                    prbs_mode,
                                    pattern);
    }

    void Pm6010::checkPrbsLane(uint32_t serdes_port_id)
    {
        // get pin status and adaptation objects
        g5_serdes_pin_status_t serdes_port_status;
        g5_api_serdes_pin_status_get(device_id,
                                       serdes_port_id,
                                       &serdes_port_status);
        serdes_port_status.print("pin status");

        // Get the PRBS counts from the specified serdes pin
        g5_serdes_prbs_ber_t serdes_prbs_status;
        g5_api_serdes_pin_prbs_get(device_id,
                                    serdes_port_id,
                                    TRUE,
                                    &serdes_prbs_status);
        serdes_prbs_status.print("PRBS BER");
    }

    void Pm6010::insPrbsErrLane(uint32_t serdes_port_id, int errs)
    {
        for (size_t i = 0; i < errs; i++)
        {
            g5_api_serdes_pin_tx_err_set(device_id, serdes_port_id);
        }
        
    }

    void Pm6010::disPrbsLane(uint32_t serdes_port_id)
    {
        g5_api_serdes_tx_source_set(device_id,
                                     serdes_port_id,
                                     G5_SERDES_TX_SRC_DSIS,
                                     0,0);
    }

    void Pm6010::lpbkRx2TxLane(uint32_t serdes_port_id, bool enable)
    {
        if(enable)
            g5_api_serdes_tx_source_set(device_id,
                             serdes_port_id,
                             G5_SERDES_TX_SRC_RX_P_N,
                             0,0);
        else
            g5_api_serdes_tx_source_set(device_id,
                 serdes_port_id,
                 G5_SERDES_TX_SRC_DSIS,
                 0,0);
    }

    void Pm6010::lpbkTx2RxLane(uint32_t serdes_port_id, bool enable)
    {
        if(enable)
            g5_api_serdes_rx_source_set(device_id,
                             serdes_port_id,
                             G5_SERDES_RX_SRC_TX_DSIS,
                             0,0);
        else
            g5_api_serdes_rx_source_set(device_id,
                 serdes_port_id,
                 G5_SERDES_RX_SRC_RX_P_N,
                 0,0);
    }

    void Pm6010::setSerdesTxeq(uint32_t serdes_port_id, uint32_t cm2, uint32_t cm1, uint32_t c2, uint32_t c1, uint32_t c0)
    {
        g5_api_serdes_tx_eq_set(device_id, serdes_port_id, cm2, cm1, c2, c1, c0);
    }

    //channel id = port * 0x1000
    void Pm6010::createOtnPort(uint8_t port)
    {
        g5_api_serdes_port_los_cfg(device_id,port_serdes_ids[port],FALSE, 0,FALSE);

        uint32_t otu4_chnl_id = port * 0x1000;
        g5_api_otn_prov(device_id, 
                otu4_chnl_id, 
                port_serdes_ids[port], 
                G5_SERDES_PIN_MODE_NRZ,        // pin_mode
                G5_OTN_PROV_MODE_LINEOTN,      // prov_mode
                G5_OTUK_OTU4,                  // rate
                G5_OTN_FEC_MODE_NONE);   // gfec_mode
    }


    void Pm6010::createEthPort(uint8_t port)
    {
        //mannual set eth port to 0-23
        g5_api_enet_serdes_route_set(device_id,G5_ENET_SERDES_ROUTE_NMS_MANUAL);
        for(int i = 0; i < 4; ++i)
            g5_api_enet_phy_base_serdes_port_id_set(device_id,(port-1)*4+i,port_serdes_ids[port]+i);

        //set los pin
        g5_api_serdes_port_los_cfg(device_id,port_serdes_ids[port],FALSE, 0,FALSE);

        //prov
        uint32_t enet_chnl_id = 0x1000 * port;
        g5_api_enet_prov(device_id,
                          enet_chnl_id,
                          port_serdes_ids[port],
                          G5_SERDES_PIN_MODE_NRZ,   // pin_mode
                          G5_ENET_PORT_MODE_100GE,  // mode 
                          FALSE,                    // serdes_port_only
                          G5_ENET_FEC_MODE_NONE);
        
        //map to odu
        g5_api_client_map_cfg(device_id,
                               enet_chnl_id,
                               G5_MAP_ODU4_100GE_GMP,  // map mode 
                               0,                      // client_num_incr
                               G5_GFP_SLICE_CFG_AUTO); // gfp_slice allocation  
    }

    // group id = 0x1000 *port 
    //changel id = groupid + 0x100, +0x200
    void Pm6010::createOtuc2Port(uint8_t port)
    {
        for (uint32_t i = 0; i < 2; i++)
        {
        // Assign the LOS pin to the base lane for each 100G port
            g5_api_serdes_port_los_cfg(device_id,
                                        port_serdes_ids[i] + 4*i,
                                        FALSE,  // los_inv
                                        port+i,      // los_pin 
                                        FALSE); // los_enable
        }

            // Provision an OTUCn group for LINEOTN
        uint32_t group_channel_id = 0x1000 * port;
        g5_api_otucn_group_prov(device_id,
                                 group_channel_id,
                                 G5_OTUCN_MODE_CN_MUXED,
                                 G5_OTN_PROV_MODE_LINEOTN,
                                 G5_OTUCN_OTUC_INTF,
                                 G5_MAX_INT,
                                 G5_MAX_INT,
                                 G5_OTN_FEC_MODE_NONE);// G5_OTN_FEC_MODE_CORRECTION
        
        for (uint32_t i = 0; i < 2; i++)
        {
            g5_api_otucn_port_add(device_id,
                                   group_channel_id + i+1,
                                   group_channel_id,
                                   port_serdes_ids[i] + 4*i,
                                   G5_SERDES_PIN_MODE_PAM4_ADJ,
                                   G5_MAX_INT,
                                   G5_MAX_INT,
                                   0);
        }
        // Configure the OTUCn group with its 5 OTUCs
        g5_api_otucn_group_config(device_id,
                                       group_channel_id);

        // ODU4 trib slot allocation into ODUC2 group
        uint32_t odu4_trib_slot[2][G5_ODU_TRIB_SLOT_NUM] = LO_TRIB_SLOT_2xODU4;

        // Provision 5xODU4s into an OTUC5 group looping each back at the ODUKSW
        for (uint32_t i = 0; i < 2; i++)
        {
            g5_odu_lo_data_t lo_info;
            lo_info.init();

            lo_info.oduk_type = G5_ODUK_ODU4;
            lo_info.payload_mode = G5_ODU_LINE_5G_TS;
            lo_info.tx_tribslot_data.trib_port_number = i;
            lo_info.rx_tribslot_data.trib_port_number = i;

            // TX trib slot mask
            memcpy(&lo_info.tx_tribslot_data.trib_slot_mask , odu4_trib_slot[i], sizeof(lo_info.tx_tribslot_data.trib_slot_mask));
            // RX trib slot mask
            memcpy(&lo_info.rx_tribslot_data.trib_slot_mask , odu4_trib_slot[i], sizeof(lo_info.rx_tribslot_data.trib_slot_mask));

            uint32_t otu4_chnl_id = port * 0x1000 + i * 100;

            // provision LO ODU4 channels in to the HO ODUC5 channel.
            g5_api_odu_mux_cfg(device_id,
                                    otu4_chnl_id,
                                    group_channel_id,
                                    lo_info);
        }   
    }
    //chanel_id = 0x10000, 0x20000,.... 0x50000
    void Pm6010::setupIlkn()
    {
            // ILKN channel info
        g5_traffic_ilkn_cbr_t ilkn_info;
        ilkn_info.init();
        ilkn_info.fabric_ingress_data.rate_info.rate = G5_ODUK_ODU4;
        ilkn_info.fabric_ingress_data.rate_info.client_bitrate = 0;
        ilkn_info.fabric_ingress_data.pkt_data.fabric_class = G5_OPSA_TRAFFIC_CLASS_256;
        ilkn_info.fabric_ingress_data.pkt_data.header_size = 4;
        ilkn_info.fabric_ingress_data.halt_buffer = 0;
        ilkn_info.fabric_ingress_data.ilkn_header.ilkn_bcw_chnl = 0;
        ilkn_info.fabric_ingress_data.ilkn_header.ilkn_2b_header = 0;
        ilkn_info.fabric_ingress_data.ilkn_header.ilkn_4b_header = 0;
        ilkn_info.fabric_egress_data.rate_info.rate = G5_ODUK_ODU4;
        ilkn_info.fabric_egress_data.rate_info.client_bitrate = 0;
        ilkn_info.fabric_egress_data.pkt_data.fabric_class = G5_OPSA_TRAFFIC_CLASS_256;
        ilkn_info.fabric_egress_data.pkt_data.header_size = 4;
        ilkn_info.fabric_egress_data.opsa_fifo.latency_s = 600;
        ilkn_info.fabric_egress_data.halt_buffer = 4;
        ilkn_info.fabric_egress_data.ilkn_header.ilkn_bcw_chnl = 0;
        ilkn_info.fabric_egress_data.ilkn_header.ilkn_2b_header = 0;
        ilkn_info.fabric_egress_data.ilkn_header.ilkn_4b_header = 0;
        for (int i = 1; i < 5; ++i)
        {
            uint32_t ilkn_chnl_id = 0x10000 * i;
            ilkn_info.fabric_ingress_data.ilkn_header.ilkn_bcw_chnl = 0;//i?
            ilkn_info.fabric_egress_data.ilkn_header.ilkn_bcw_chnl = 0;//?
            g5_api_ilkn_cbr_prov(device_id,
                              ilkn_chnl_id,
                              ilkn_info);
        }
    }
    void Pm6010::createConnect(uint32_t src, uint32_t dest)
    {
        g5_api_switch_connect(device_id, src, dest);
    }
    void Pm6010::disConnect(uint8_t port)
    {
         g5_api_switch_disconnect(device_id, port * 0x1000);
    }
    void Pm6010::deprovPort(uint8_t port)
    {
        g5_api_deprov(device_id, port*0x1000, G5_DEPROV_SEGMENT);
    }
}


