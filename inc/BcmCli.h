#ifndef BCMCLI_H
#define BCMCLI_H

#include "ss_gdef.h"


#include <string>
#include <mutex>

namespace Devices
{
    using namespace std;
    class BcmCli
    {
        public:
            BcmCli(string libBcm);
            ~BcmCli();

            void start();

        private:
            string myLibBcm;
            void *myHandle;
    };
    
} // namespace Devices


#endif