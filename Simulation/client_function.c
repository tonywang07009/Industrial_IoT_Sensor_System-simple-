#include "client_function.h"

int run_single_client_session(const char *ip, uint16_t port, uint32_t machine_id, int send_count, int *is_bad_count)
{
    net_socket_t sock;

    if (net_connect(ip, port, &sock) != 0)
    {
        printf("connect failed\n");
        return -1;
    }

    printf("connect ok ,will sned %d package\n", send_count);
    Packet_t pkt;

    int bad_count = 0;
    double bad_probilty = 0.2;

    for (int i = 0; i < send_count; i++)
    {
        memset(&pkt, 0, sizeof(Packet_t)); // clear

        pkt.header.version = 1;
        pkt.header.op_code = OPCODE_DATA_REPORT;
        pkt.header.machine_id = htonl(machine_id);
        pkt.header.timestamp_sec = htonl(1734567890); // demo
        pkt.header.seq_no = htons((uint16_t)(i + 1));

        bool is_bad = random_fill_parameters(&pkt, bad_probilty);

        if (is_bad == true)
        {
            bad_count++;
        }

        if (proto_send_packet(sock, &pkt) == 0)
        {
            printf("send %d / %d ok\n", i + 1, send_count);
        }
        else
        {
            printf("send %d failed\n", i + 1);
            break;
        }
    }
    *is_bad_count = bad_count;
    net_close(sock);
    return 0;
}