#include <stdio.h>
#include <string.h>

#include "..\Parser\protocol_parser.h"
#include "..\Socket\protocol_socket.h"
#include "..\Socket\net_compatible.h" // 網路相容性標頭檔

int main(void)
{
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

    printf("The server will opening in port 8080");
    net_socket_t client_sock = accept(server_sock, NULL, NULL);

    if (client_sock == INVALID_SOCKET) // INVALID_SOCKET -> 為 socket 保留字 待研究
    {
        perror("disaccept");
        return 1;
    }

    printf("The client is connet");

    ParsedData_t data; // 宣告符合 protocol 的資料型態

    int result = proto_recv_and_parse(client_sock, &data, 5000);

    if (result == 0)
    {
        printf("✅ The parser sussful.\n");
        printf("  MeachineID: %u\n", data.meachine_id);
        printf("  Time stemp: %u\n", data.timestamp_sec);
        printf("  Sequence: %u\n", data.seq_no);
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