#ifndef _CLIENT_FUNCTION_H
#define _CLIENT_FUNCTION_H

#include "../Socket/net_compatible.h"
#include "server_function.h"
#include "../Statistics_tool/statistics.h"
#include "../Parser/protocol_parser.h"
#include "../Socket/protocol_socket.h"
#include "../Security_AES/security.h"
#include "../Simulation/Radom/random_fill_parameters.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct ClientArg
{
    const char *ip;
    uint16_t port;
    uint32_t machine_id;
    int send_count;
    int bad_count;

} ClientArg_t;

void *client_thread_function(void *arg);
int run_single_client_session(const char *ip, uint16_t port, uint32_t machine_id, int send_count, int *is_bad_count);

#endif