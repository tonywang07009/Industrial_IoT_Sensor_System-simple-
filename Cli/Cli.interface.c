#include "Cli.interface.h"

int main(void)
{
    if (net_init() != 0) // The clean sock pre call
    {
        printf("net_init failed\n");
        return 1;
    }
    Cli_information config = {0}; // init for typesturct

    while (1)
    {
        printf("=== Wellcome to the Simple Iot simluation system ~~ === \n");
        int choose = 0;
        printf(" 1): setting ip & port \n 2): open the server simluation \n 3): open the client simluation \n ");
        printf("4): Display static parmerter & need check meachine \n )5: Display production line blance.\n");
        printf("0): exit\n");
        printf("Please choose The function: ");
        scanf("%d", &choose);

        switch (choose)
        {
        case 0:
            printf("bye \n");
            break; // break the cli
            break;

        case 1: // The setting ip and port
            printf("Setting the IP");
            scanf("%19s", config.ip);
            printf("\n Setting the Port");
            scanf("%d", &config.port);
            // The ip need count the strlen , because that's is char
            if (strlen(config.ip) > 0 && config.port > 0)
            {
                printf("your setting :\n ip: %s \n Port: %d", config.ip, config.port);
            }
            else
            {
                printf("setting is fail \n");
                continue;
            }
            continue;

        case 2: // The open the server

            break;

        case 3: // The open the client

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

            printf("Setting the send the package count setting: ");
            scanf("%d", &config.send_package_count_setting);
            printf("\n");
            printf("Setting the send the thread_count_setting: ");
            scanf("%d", &config.thread_count_setting);
            printf("\n");
            printf("Setting the send the bad_probability (ex: 0.2): ");
            scanf("%lf", &config.bad_probability); // lf : double scanf format
            printf("\n");

            run_multi_client_sessions(&config);
            break;
        case 4: // Display static parmerter & need check meachine

        case 5: // Display production line blance. (systemlog .so)

        case 6: // system log

        default:

            printf("Unknow choose ! \n");
            continue;

            break;
        }
    }
    net_cleanup(); // clear the net content in final
}

int run_multi_client_sessions(const Cli_information *config)
{

    if (config->thread_count_setting <= 0)
    {
        printf("The thread_count_setting is illegal!!");

        return 1;
    }
    else if (config->send_package_count_setting <= 0)
    {
        printf("Thesend_package_count_setting is illegal !!");
        return 1;
    }
    /*define the different process need setting*/
    pthread_t tids[config->thread_count_setting];
    ClientArg_t cfg[config->thread_count_setting];

    int bad_count = 0; // This is static the how many bad meachine
    int total_bad = 0;

    for (int i = 0; i < config->thread_count_setting; i++)
    {
        cfg[i].ip = config->ip;
        cfg[i].port = config->port;
        cfg[i].machine_id = (uint32_t)(i + 1);
        cfg[i].send_count = config->send_package_count_setting;
        cfg[i].bad_probilty = config->bad_probability;
        cfg[i].bad_count = bad_count;
    }

    printf("\n");
    /* The create thread*/
    for (int i = 0; i < config->thread_count_setting; i++)
    {
        pthread_create(&tids[i], NULL, client_thread_function, &cfg[i]);
        // Research
    }
    /* join the function setting*/
    for (int i = 0; i < config->thread_count_setting; i++)
    {
        pthread_join(tids[i], NULL);
        // Research
        total_bad += cfg[i].bad_count;
        printf("Machine %u bad_count = %d\n", cfg[i].machine_id, cfg[i].bad_count);
    }

    if (total_bad != 0)
    {
        printf("Total bad packages = %d\n", total_bad);
    }
    else
    {
        printf("No bad packages");
    }
    return 0;
}