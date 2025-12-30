#include "Cli.interface.h"

int main(void)
{

    while (1)
    {
        int choose = 0;
        Cli_information config;

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
            scanf ("%c",&config.Type1.ip);
            printf("\n Setting the Port");
            scanf("%d",&config.Type1.port);

            if(config.Type1.ip != NULL && config.Type1.port !=NULL)
            {
                printf("your setting :\n ip: %c \n Port: %d",config.Type1.ip,config.Type1.port);
            }
            else
            {
                printf("setting is fail \n");
                continue;
            }
            continue;

        case 2: // The open client
            if(config.Type1.ip == NULL && config.Type1.port ==NULL)
            {
                printf("you need seeting the ip and port first \n");
                continue;
            }
            
            run_single_client_session(&config.Type1.ip,config.Type1.port,)
            
        default:
        
            printf("Unknow choose ! \n");
            continue;

            break;
        }        

    }
    


}