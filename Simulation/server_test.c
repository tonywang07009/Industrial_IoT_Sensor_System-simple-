#include <stdio.h>
#include <string.h>
#include "..\Statistics_tool\statistics.h"
#include "..\Parser\protocol_parser.h"
#include "..\Socket\protocol_socket.h"
#include "..\Socket\net_compatible.h" // 網路相容性標頭檔

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

    printf("The server will opening in port 8080\n");
    net_socket_t client_sock = accept(server_sock, NULL, NULL);

    if (client_sock == INVALID_SOCKET) // INVALID_SOCKET -> 為 socket 保留字 待研究
    {
        perror("disaccept");
        return 1;
    }

    printf("The client is connet\n");

    for (;;) // research code
    {
        ParsedData_t data; // 宣告符合 protocol 的資料型態
        int result = proto_recv_and_parse(client_sock, &data, 3000);
        if (result != 0)
        {
            uint32_t id = data.meachine_id;
            printf("recv/parse fail , break \n");
            printf("recv/parse fail at sample #%u, result=%d\n", vib_stats[id].count + 1, result);
            break;
        }
        if (result == 0)
        {
            printf("✅ The parser sussful.\n");
            protocol_print_packet(&data);
            uint32_t id = data.meachine_id;
            if (id < MAX_MEACHINES)
            {
                StatSample_t s = {                                        // 用法研究
                                  .value = data.vibration.velocity_rms_x, // 例如只看 X
                                  .machine_id = id,
                                  .timestamp_sec = data.timestamp_sec};

                stats_add_sample(&vib_stats[id], &s);

                StatResult_t static_value;
                if (stats_compute(&vib_stats[id], s.value, &static_value) == 0)
                {
                    if (static_value.count < 35)
                    {
                        printf("Need More Sample %u \n", (35 - static_value.count));
                        printf("SPC warm-up: N=%u, mean=%.2f, stddev=%.3f,  (Z=%.2f, not used yet)\n",
                               static_value.count, static_value.mean, static_value.stddev, static_value.z_score);
                    }
                    else
                    {
                        printf("Sample Enough %u", static_value.count);
                        const char *status;
                        double z_value = fabs(static_value.z_score); // fabs -> the float abs value

                        /*The value inspection*/
                        if (z_value < 1.0)
                        {
                            status = "GREEN";
                        }
                        else if (z_value >= 1.0 && z_value <= 1.645)
                        {
                            status = "ORANGE";
                        }
                        else if (z_value >= 1.645 && z_value <= 1.96)
                        {
                            status = "RED";
                        }
                        else
                        {
                            status = " CRITICAL ";
                        }

                        printf("SPC: N=%u, mean=%.2f, stddev=%.3f, Z=%.2f, STATUS=%s\n",
                               static_value.count, static_value.mean, static_value.stddev, static_value.z_score, status);
                    }
                }
            }
        }
    }
}
void debug_dump_body_hex(const Packet_t *pkt)
{
    const uint8_t *p = (const uint8_t *)&pkt->body;
    printf("Body raw bytes (hex):");
    for (size_t i = 0; i < sizeof(pkt->body); ++i)
    {
        if (i % 16 == 0)
            printf("\n  ");
        printf("%02X ", p[i]);
    }
    printf("\n");
}