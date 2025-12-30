#ifndef __CLI_INTERFACE__
#define __CLI_INTERFACE__

#include "../Simulation/client_function.h"
#include "../Simulation/server_function.h"

#include<stdio.h>
#include<stdlib.h>

typedef struct Cli_interface
{
    struct type1
    {
        char ip;
        int  port;
    }Type1;
    
    struct type2
    {
        char ip;
        int  port;
        int  meachine_id;
        int  package_send_count;
        int* is_bad_count;
        
    }Type2;
    

}Cli_information;


#endif