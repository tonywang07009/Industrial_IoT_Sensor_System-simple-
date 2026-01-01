#include "Cli_function.h"

int main(void)
{
    /* step 1  system initialization */
    if (net_init() != 0)
    {
        printf("net_init failed\n");
        return 1;
    }
    Cli_information config = {0}; // init for typesturct
    int running = 1;

    /* step 2 The cli control */
    while (running == 1)
    {
        int choose = 0;
        int server_result = 0;
        int client_result = 0;


        printf("=== Wellcome to the Simple Iot simluation system ~~ === \n");
        printf(" 1): setting ip & port \n 2): open the server simluation \n 3): open the client simluation \n ");
        printf("0): exit\n");
        printf("Please choose The function: ");
        scanf("%d", &choose);

        switch (choose)
        {
        case 0:
        {
            printf("bye \n");
            running = 0;
            break;
        }
        case 1: // The setting ip and port
        { 
            printf("Setting the IP: ");
            scanf("%19s", config.ip);
            printf("\n Setting the Port: ");
            scanf("%d", &config.port);
            printf("\n");
    
            if (strlen(config.ip) > 0 && config.port > 0) // The ip need count the strlen , because that's is char
            {
                printf("your setting :\n ip: %s \n Port: %d \n", config.ip, config.port);
            }
            else
            {
                printf("setting is fail \n");
                continue;
            }
            continue;
        }
        case 2: // The open the server
        {
            pid_t pid = fork();
            printf("you choose the open server \n");
            if (strlen(config.ip) > 0 && config.port > 0)
            {
                printf("your setting :\n ip: %s \n Port: %d\n", config.ip, config.port);
                printf("your child process will create %d\n", child_process_count);
            }
            else
            {
                printf("you need seeting the ip and port first \n");
                continue;
            }
             // create the background process.
            if (pid <0)
            {
                perror("fork");
            }
            else if(pid==0)
            { 
                server_result = run_server_process(&config);
                if (server_result !=0)
                {
                    printf("The server create fail\n");
                    _exit(1);
                }
                _exit(0);
            }
            else
            {
                printf("Server started in background, pid=%d\n", pid);
            }

            break;
        }
        case 3: // The open the client
        {
            printf("you choose the open client \n");

            if (strlen(config.ip) > 0 && config.port > 0)
            {
                printf("your setting :\n ip: %s \n Port: %d", config.ip, config.port);
            }
            else
            {
                printf("you need seeting the ip and port first \n");
                continue;
            }
            printf("\n");
            printf("==== The parmeter setting ==== \n");

            printf("Setting the meachine amount: ");
            scanf("%d", &config.thread_count_setting);

            printf("\n");
            printf("Setting the send the package count setting: ");
            scanf("%d", &config.send_package_count_setting);


            printf("\n");
            printf("Setting the send the bad_probability (ex: 0.2): ");

            scanf("%lf", &config.bad_probability); // lf : double scanf format
            printf("\n");

            client_result = run_multi_client_sessions(&config);
            
            if (client_result != 0)
            {
                printf ("The muilti client session create fail \n ");
            }
            
            break;
        }

        case 4: // Display system log
        {
           cli_show_system_log();
        }
        default:
        {

            printf("Unknow choose ! \n");
            continue;

            break;
        }
        }
    }
    net_cleanup(); // clear the net content in final
    return 0;
}


