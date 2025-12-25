#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "server_function.h"

#define child_process_count 4 

int main(void)
{
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

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

    net_init(); // The sockclose
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
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

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

    printf("Parent: listen on 8080, forking workers...\n");

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
    }
    printf("Total requests handled by all workers: %llu\n",
       (unsigned long long)shm_stats->total_requests);

    munmap(shm_stats, sizeof(ServerSharedStats_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    close(server_sock); // The server socket close
    return 0;
}
