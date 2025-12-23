#include <sys/types.h>
#include "Multi_process_server.h"

#define MAX_MEACHINES 128

static StatAccumulator_t vib_stats[MAX_MEACHINES];

int main(void)
{
    for (int i = 0; i < MAX_MEACHINES; i++)
    {
        stats_reset(&vib_stats[i]);
    }

    net_init(); // The sockclose

    net_socket_t server_sock = socket(AF_INET, SOCK_STREAM, 0);
    // The socket anncount

    if (server_sock == INVALID_SOCKET)
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
}
