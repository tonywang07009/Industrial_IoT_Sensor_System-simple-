#ifndef __MULTI_PROCESS_SERVER__
#define __MULTI_PROCESS_SERVER__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "..\Statistics_tool\statistics.h"
#include "..\Parser\protocol_parser.h"
#include "..\Socket\protocol_socket.h"
#include "..\Socket\net_compatible.h" // 網路相容性標頭檔

#define MAX_MEACHINES 500
static StatAccumulator_t vib_stats[MAX_MEACHINES];

typedef struct Share_memory
{

    uint64_t total_requests;
    // when child finshed package , this will ++;

} SharedStats;

int run_single_process_server(int listen_fd);
void debug_dump_body_hex(const Packet_t *pkt);

#endif