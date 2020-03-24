#ifndef PM6010_H
#define PM6010_H
#include "g5_api.h"
#include <cstdint>
namespace Devices
{
    class Pm6010
    {
        public:
            explicit Pm6010(uint32_t index);
            ~Pm6010();

            //prbs
            void startPrbsLane(uint32_t serdes_port_id, uint8_t rate);
            void checkPrbsLane(uint32_t serdes_port_id);
            void insPrbsErrLane(uint32_t serdes_port_id, int errs);
            void disPrbsLane(uint32_t serdes_port_id);

            //lpbk
            void lpbkRx2TxLane(uint32_t serdes_port_id, bool enable = true);
            void lpbkTx2RxLane(uint32_t serdes_port_id, bool enable = true);

            //serdes tx eq
            void setSerdesTxeq(uint32_t serdes_port_id, uint32_t cm2, uint32_t cm1, uint32_t c2, uint32_t c1, uint32_t c0);

            //traffic
            void createOtnPort(uint8_t port);
            void createEthPort(uint8_t port);
            void createOtuc2Port(uint8_t port);
            void setupIlkn();
            void createConnect(uint32_t src, uint32_t dest);
            void disConnect(uint8_t port);
            void deprovPort(uint8_t port);



        private:
            uint32_t device_id;
            uint32_t index ;

    };
}


#endif