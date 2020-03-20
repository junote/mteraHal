#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

#include "BcmCli.h"
#include "tlab_bcm_init.h"

namespace Devices
{
    using namespace std;


    BcmCli::BcmCli(string libBcm):myLibBcm(libBcm),myHandle(0)
    {
        myHandle = dlopen(myLibBcm.c_str(), RTLD_LAZY);

        if(!myHandle)
        {
            cout << "open bcm lib fail" << dlerror() << endl;
            return;
        }

        typedef int (*bcm_tlab_main_loop_t)(int argc, char *argv[]);
        bcm_tlab_main_loop_t bcm_tlab_main_loop = (bcm_tlab_main_loop_t)dlsym(myHandle, "bcm_tlab_main_loop");
        if(!bcm_tlab_main_loop)
        {
            cout << "error: " << dlerror() << endl;
            dlclose(myHandle);
            return;
        }

        const char *argv[] = {"bcm", ""};
        int argc = 1;
        // bcm_tlab_main_loop(argc, (char **)argv);
        thread myThread(bcm_tlab_main_loop, argc, (char **)argv);
        myThread.join();
        cout << "done\n";
    }

    void BcmCli::start()
    {
        typedef void (*bcm_tlab_start_shell_t)(void);
        bcm_tlab_start_shell_t bcm_tlab_start_shell = (bcm_tlab_start_shell_t)dlsym(myHandle, "bcm_tlab_start_shell");

        typedef int (*bcm_tlab_shell_enabled_t)(void);
        bcm_tlab_shell_enabled_t bcm_tlab_shell_enabled = (bcm_tlab_shell_enabled_t)dlsym(myHandle, "bcm_tlab_shell_enabled");
        
        if(!bcm_tlab_start_shell)
        {
            cout << "error: " << dlerror() << endl;
            dlclose(myHandle);
            return;
        }
        bcm_tlab_start_shell();
        while (bcm_tlab_shell_enabled());

    }

    BcmCli::~BcmCli()
    {
        dlclose(myHandle);
    }    
} // namespace Devices
