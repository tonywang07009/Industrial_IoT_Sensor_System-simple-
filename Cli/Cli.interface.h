#ifndef __CLI_INTERFACE__
#define __CLI_INTERFACE__

#define zombie_clear signal(SIGCHLD, SIG_IGN)

#include "../Simulation/client_function.h"
#include "../Simulation/server_function.h"
#include "../Socket/net_compatible.h"
#include "../Statistics_tool/statistics.h"
#include "../Parser/protocol_parser.h"
#include "../Socket/protocol_socket.h"
#include "../Security_AES/security.h"

/*ptread need*/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Cli_interface // The for CLI used datasturct
{

    char ip[20];
    int port;
    int thread_count_setting;
    int send_package_count_setting;
    double bad_probability;

} Cli_information;

/* The mutile thread function*/

int run_multi_client_sessions(const Cli_information *cfg);

#endif