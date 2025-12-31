#ifndef __CLI_INTERFACE__
#define __CLI_INTERFACE__

/*ptread need*/
#include <sys/types.h>
#include <signal.h> // This is used for SIGCHLD, SIG_IGN
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <log.h> // 封裝 .so檔

#define zombie_clear signal(SIGCHLD, SIG_IGN)
#define child_process_count 32
#define LOG_PATH "system.log"
#define LOG_TAIL_LINES 35

#include "../Simulation/client_function.h"
#include "../Simulation/server_function.h"
#include "../Socket/net_compatible.h"
#include "../Statistics_tool/statistics.h"
#include "../Parser/protocol_parser.h"
#include "../Socket/protocol_socket.h"
#include "../Security_AES/security.h"


typedef struct Cli_interface // The for CLI used datasturct
{

    char ip[20];
    int port;
    int thread_count_setting;
    int send_package_count_setting;
    double bad_probability;

} Cli_information;

/* The mutile thread function*/

int run_server_process(const Cli_information* cfg);
int run_multi_client_sessions(const Cli_information *cfg);
static void cli_show_system_log(void);
static void cli_show_stats_and_audit(void);


#endif