#include "Cli_function.h"

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


int run_server_process(const Cli_information* cfg)
{
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); //need research point

    if(shm_fd <0)
    {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(ServerSharedStats_t)) < 0) 
    {
    perror("ftruncate");
    return 1;
    }

    ServerSharedStats_t* shm_stats = mmap(NULL,
            sizeof(ServerSharedStats_t),
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            shm_fd, 0
        );
    
    if (shm_stats == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    shm_stats->total_requests =0;

    for (int i = 0; i < MAX_MEACHINES; i++)
    {
        stats_reset(&vib_stats[i]);
    }

    net_socket_t server_sock = socket(AF_INET, SOCK_STREAM, 0);
    // The socket anncount

    if (server_sock ==-1) //The linux
    {
        perror("server_socket");
        return 1;
    }

    int sock_opt = 1; // need observer data type
                      // The most soket API data type is int

    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&sock_opt, sizeof(sock_opt));
    // SOL_SOCKET, SO_REUSEADDR, -> relize
    // (const char *) data_type transformer
    // This need deep know // Unix programming

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // bind local computer all ip
    addr.sin_port = htons(cfg->port);

    if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(server_sock, 500) < 0)
    {
        perror("listen");
        return 1;
    }

    printf("Parent: listen on %d, forking workers... \n", cfg->port); // The listen port dispaly.
    syslog_info(LOG_EVENT_SERVER_START,cfg->port,0,"workers=%d",child_process_count);
    // LOG_EVENT_SERVER_START : like 2025-12-31 19:12:00 [SERVER_START] port=8080 is_bad=0 workers=32

    shm_stats->total_requests = 0;
    
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm_stats->lock, &attr);
    pthread_mutexattr_destroy(&attr);
    server_set_shared_stats(shm_stats);

    for (int i = 0; i < child_process_count; ++i)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            // Child process
            printf("Child %d:(pid =%d) started,\n",i, getpid());
            run_single_process_server(server_sock);

            _exit(0); // the child process exit
        }
    }
    for(int i = 0 ; i < child_process_count ; i++)
    {
        wait(NULL); // The parent process wait child process finshed
                    // prevent the zombie process.
    }
    // printf("Total requests handled by all workers: %llu\n",
    //    (unsigned long long)shm_stats->total_requests);
    
    syslog_info(LOG_EVENT_SERVER_START,
                cfg->port,
                0,
                "workers=%d",
                child_process_count);

    syslog_info(LOG_EVENT_SERVER_START,
                cfg->port,0,
                "total_requests=%llu",
                shm_stats->total_requests);
    // llu -> long long unsigned 64bit
    /*Research point*/
    munmap(shm_stats, sizeof(ServerSharedStats_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    close(server_sock); // The server socket close
    return 0;

}

void cli_show_system_log(void)
{
    FILE *fp = fopen(LOG_PATH,"r");
    // The pointer the file dictory

    if(!fp)
    {
        printf("Cannot open log file: %s\n", LOG_PATH);
        return;
    }

    char* lines [LOG_TAIL_LINES] = {0}; // The init
    int count = 0;

    char buffer [2048] = {0};
    while (fgets(buffer,sizeof(buffer),fp)) // The fgets like scrpy function
    {
        int idx = count % LOG_TAIL_LINES;
        if(lines[idx]!=NULL)
        {
            free(lines[idx]); // release menory
        }
        lines[idx]=strdup(buffer);
        count++;
    }
    fclose(fp);

    int start = (count < LOG_TAIL_LINES) ? 0 : (count % LOG_TAIL_LINES); // RD
    int n = count < LOG_TAIL_LINES? count:LOG_TAIL_LINES;
    
    printf("=== Last %d log lines from %s ===\n", n, LOG_PATH);
    
    /*The RD Block*/
    for (int i = 0; i < n; i++) 
    {
        int idx = (start + i) % LOG_TAIL_LINES;

        if (lines[idx]!=NULL) 
        {
        printf("%s", lines[idx]);
        free(lines[idx]);
        }
    }

}
