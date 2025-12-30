#ifndef __CLI_INTERFACE__
#define __CLI_INTERFACE__

#include "../Simulation/client_function.h"
#include "../Simulation/server_function.h"

#include<stdio.h>
#include<stdlib.h>

typedef struct Cli_interface
{

    char ip;
    int  port;
    
    struct type2
    {
        int  meachine_id;
        int  package_send_count;
        int* is_bad_count;
        
    }Type2;
    

}Cli_information;


#endif