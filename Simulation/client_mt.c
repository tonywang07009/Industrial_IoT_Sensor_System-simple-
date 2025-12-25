#include "../Socket/net_compatible.h"
#include "server_function.h"
#include "../Statistics_tool/statistics.h"
#include "../Parser/protocol_parser.h"
#include "../Socket/protocol_socket.h"
#include "../Security_AES/security.h"
#include "client_function.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void *client_thread_func(void *arg) // The  thread_func implementation
{
    ClientArg_t *client_arg = (ClientArg_t *)arg;
    run_single_client_session(client_arg->ip, client_arg->port, client_arg->machine_id, client_arg->send_count, &client_arg->bad_count);
    return NULL;
}

int main(void)
{

    if (net_init() != 0)
    {
        printf("net_init failed\n");
        return 1;
    }

    int thread_count = 0;
    printf("Specy the number of thread_count: ");

    if (scanf("%d", &thread_count) != 1)
    {
        printf("Invalid input\n");
        return 1;
    }

    pthread_t tids[thread_count];
    ClientArg_t cfg[thread_count];

    int bad_count = 0;
    int send_count = 0;
    printf("Specy the number of send_count: ");
    scanf("%d", &send_count);

    for (int i = 0; i < thread_count; i++)
    {
        cfg[i].ip = "127.0.0.1";
        cfg[i].port = 8080;
        cfg[i].machine_id = (uint32_t)(i + 1);
        cfg[i].send_count = send_count;
        cfg[i].bad_count = bad_count;
    }

    printf("\n");

    for (int i = 0; i < thread_count; i++)
    {
        pthread_create(&tids[i], NULL, client_thread_func, &cfg[i]);
    }

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(tids[i], NULL);
    }

    net_cleanup();
    return 0;
}