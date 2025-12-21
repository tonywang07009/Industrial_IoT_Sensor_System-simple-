#include "..\Socket\net_compatible.h"
#include "..\Socket\protocol_socket.h"
#include "..\Security_AES\security.h"

#define SENDPACKAGE 35

int main(void)
{

    if (net_init() != 0)
    {
        printf("net_init failed\n");
        return 1;
    }

    net_socket_t sock;
    if (net_connect("127.0.0.1", 8080, &sock) != 0)
    {
        printf("connect failed\n");
        net_cleanup();
        return 1;
    }
    printf("connect ok\n");

    // The package send test
    Packet_t pkt;

    for (int i = 0; i < SENDPACKAGE; i++)
    {
        memset(&pkt, 0, sizeof(Packet_t)); // clear

        pkt.header.version = 1;
        pkt.header.op_code = OPCODE_DATA_REPORT;
        pkt.header.sensor_type = SENSOR_TYPE_VIBRATION;
        pkt.header.machine_id = htonl(12345);
        pkt.header.timestamp_sec = htonl(1734567890);
        pkt.header.seq_no = htons((uint16_t)(i + 1));

        pkt.body.vibration.velocity_rms_x = htons(450);
        pkt.body.vibration.velocity_rms_y = htons(300);
        pkt.body.vibration.velocity_rms_z = htons(300);
        pkt.body.vibration.accel_peak = htons(150);
        pkt.body.vibration.status_flags = 1;

        if (proto_send_packet(sock, &pkt) == 0)
        {
            printf("send %d / %d ok\n", i + 1, SENDPACKAGE);
        }
        else
        {
            printf("send %d failed\n", i + 1);
            break;
        }
    }

    net_close(sock);
    net_cleanup();
    return 0;
}