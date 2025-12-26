#ifndef __MULTI_PROCESS_SERVER__
#define __MULTI_PROCESS_SERVER__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h> 
#include <sys/stat.h>
#include "../Statistics_tool/statistics.h"
#include "../Parser/protocol_parser.h"
#include "../Socket/protocol_socket.h"
#include "../Socket/net_compatible.h" // 網路相容性標頭檔

#define SHM_NAME "/server_stats_shm"
#define MAX_MEACHINES 500
static StatAccumulator_t vib_stats[MAX_MEACHINES];

typedef struct Share_memory
{

    pthread_mutex_t lock; // The lock for multi-process
    uint64_t total_requests;
    // when child finshed package , this will ++;

} ServerSharedStats_t;

// The ipc shared memory struct
static ServerSharedStats_t* g_stats; // The shared memory pointer
int run_single_process_server(int listen_fd);
void server_set_shared_stats(ServerSharedStats_t *p);

#endif