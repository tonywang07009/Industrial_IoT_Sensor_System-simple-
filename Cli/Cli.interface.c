#include "Cli.interface.h"

int main(void)
{
    Cli_information config;
    while (1)
    {
        int choose = 0;
     

        printf("Please choose The function: ");
        scanf("%d",&choose);

        switch (choose)
        {
        case 0:
            printf("bye \n");
            break; // break the cli
            break;

        case 1: // The setting ip and port 
            printf("Setting the IP");
            scanf ("%c",&config.ip);
            printf("\n Setting the Port");
            scanf("%d",&config.port);

            if(config.ip != NULL && config.port !=NULL)
            {
                printf("your setting :\n ip: %c \n Port: %d",config.ip,config.port);
            }
            else
            {
                printf("setting is fail \n");
                continue;
            }
            continue;

        case 2: // The open client
            if(config.ip == NULL && config.port ==NULL)
            {
                printf("you need seeting the ip and port first \n");
                continue;
            }
            printf("Setting the send package amount:");
            scanf("%d",config.Type2.package_send_count); 
            run_single_client_session(&config.ip,config.port,config.Type2.meachine_id,config.Type2.package_send_count,&config.Type2.is_bad_count);
        
        case 3: // The open the server 
            
            run_single_process_server()
            
        default:
        
            printf("Unknow choose ! \n");
            continue;

            break;
        }        

    }
    


}