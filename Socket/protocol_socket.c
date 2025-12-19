#include "protocol_socket.h"

int net_connect(const char *ip, uint16_t port, net_socket_t *out_sock)
{
    if (!ip || !out_sock)
    {
        return -1;
    }
    net_socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
#if defined(_WIN32) || defined(_WIN64)
    if (sock == INVALID_SOCKET)
    {
        return -1;
    }
#else
    if (sock < 0)
    {
        return -1;
    }
#endif

    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    /*into the socket need parmeter

    */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    //  struct sockaddr * to transformer to sockaddr datatype -> network
    {
        net_close(sock);
        return -1;
    }
    *out_sock = sock; // writtent the sock address
    return 0;
}

int proto_send_packet(net_socket_t sock, const Packet_t *package)
{
    if (!package)
    {
        return -1;
    }

    Packet_t temp;
    memcpy(&temp, package, sizeof(Packet_t));

    uint16_t crc = protocol_crc16((const uint8_t *)&temp, sizeof(Packet_t) - sizeof(uint16_t));

    temp.checksum = htons(crc); // this need note

    const uint8_t *buf = (const uint8_t *)&temp;
    size_t to_send = sizeof(Packet_t);

    while (to_send > 0)
    {
        ssize_t send_var = send(sock, (const char *)buf, (int)to_send, 0);

        if (send_var <= 0)
        {
            return -1;
        }

        buf += send_var;
        to_send -= (size_t)send_var;
    }

    return 0;
}

int proto_recv_and_parse(net_socket_t sock, ParsedData_t *output, int timeout_ms) // 收到和將封包解析成 parser能用的
{
    if (!output)
    {
        return -1;
    }
// 設定 recv timeout (windwos and unix)
#if defined(_WIN32) || defined(_WIN64)
    DWORD tv = (DWORD)timeout_ms;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
#else
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif

    uint8_t buf[sizeof(Packet_t)]; // create array
    size_t need = sizeof(Packet_t);
    uint8_t *buf_ptr = buf;

    while (need > 0)
    {
        ssize_t recv_var = recv(sock, (char *)buf_ptr, (int)need, 0); // ssize_t -> long long size_t
                                                                      //  size_t -> unsigned char
        if (recv_var <= 0)
            return -1;
        buf_ptr += recv_var;
        need -= (size_t)recv_var;
    }

    ParserResult_t r = parse_protocol(buf, sizeof(Packet_t), output);
    if (r != PARSER_OK)
    {
        printf("parse_protocol error: %d\n", r);
        return -1;
    }
    return 0;
}