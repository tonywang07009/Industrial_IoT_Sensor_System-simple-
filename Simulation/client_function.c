#include "client_function.h"

int run_single_client_session(const char* ip ,uint16_t port, uint32_t machine_id, int send_count )
{
    net_socket_t sock;

    if (net_connect(ip, port, &sock) != 0)
    {
        printf("connect failed\n");
        return -1;
    }

    printf("connect ok ,will sned %d package\n",send_count);
    Packet_t pkt;



    for (int i = 0; i < send_count ; i++)
    {
        memset(&pkt, 0, sizeof(Packet_t)); // clear

        pkt.header.version = 1;
        pkt.header.op_code = OPCODE_DATA_REPORT;
        pkt.header.sensor_type = SENSOR_TYPE_VIBRATION;
        pkt.header.meachine_id = htonl(machine_id); // 大端
        pkt.header.timestamp_sec = htonl(1734567890);
        pkt.header.seq_no = htons((uint16_t)(i + 1));

        pkt.body.vibration.velocity_rms_x = htons(450);
        pkt.body.vibration.velocity_rms_y = htons(300);
        pkt.body.vibration.velocity_rms_z = htons(300);
        pkt.body.vibration.accel_peak = htons(150);
        pkt.body.vibration.status_flags = 1;

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

    net_close(sock);
    return 0;


}