#ifndef __CLI_INTERFACE__
#define __CLI_INTERFACE__

/*ptread need*/
#include <sys/types.h>
#include <signal.h> // This is used for SIGCHLD, SIG_IGN
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define zombie_clear signal(SIGCHLD, SIG_IGN)
#define child_process_count 32

#include "../Simulation/client_function.h"
#include "../Log/log.h" 
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

#endif