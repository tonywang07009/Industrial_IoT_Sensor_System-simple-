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

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    // sockaddr is defalut data
    //  struct sockaddr * to transformer to sockaddr datatype -> network
    {
        net_close(sock);
        return -1;
    }
    *out_sock = sock; // writtent the sock address
    return 0;
}

int proto_send_packet(net_socket_t sock, const Packet_t *package) // client_send
{
    printf("parser side sizeof(Packet_t) = %zu\n", sizeof(Packet_t));

    if (!package) // if the package is equal NULL
    {
        return -1;
    }

    Packet_t temp;
    int AES_result = 0;

    memcpy(&temp, package, sizeof(Packet_t));

    /* body do the AES unlock*/
    uint8_t *body_ptr = (uint8_t *)&temp.body; // 指到 bodyptr的門牌
    const uint8_t *AES_key = security_get_key();

    temp.header.body_len = sizeof(temp.body);
    size_t body_len = temp.header.body_len;

    /*step 2 into IV*/
    for (int i = 0; i < 16; ++i) // the array into the lock
    {
        temp.header.aes_iv[i] = (uint8_t)(rand() & 0XFF);
    }

    AES_result = encrypt_packet_payload(body_ptr, body_ptr, body_len, AES_key, temp.header.aes_iv); // get the AES_KEY number
    if (AES_result != 0)
    {
        return -1; // return nagative -1
    }
    // need node for AES
    /* The CRC implement*/
    uint16_t crc = protocol_crc16((const uint8_t *)&temp, (sizeof(Packet_t) - sizeof(uint16_t)));
    temp.checksum = htons(crc); // this need note

    /*Send the package*/
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

int proto_recv_and_parse(net_socket_t sock, ParsedData_t *output, int timeout_ms) // The server recive package do it
{
    printf("recv side sizeof(Packet_t) = %zu\n", sizeof(Packet_t));

    if (!output) // don't gave the menery space expect handle.
    {
        return -1;
    }

    // compaer the time uint swtich
    // this will create the compaer header
#if defined(_WIN32) || defined(_WIN64)
    DWORD tv = (DWORD)timeout_ms;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

#else

    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

#endif

    /*create the queue space*/
    uint8_t buf[sizeof(Packet_t)];
    size_t need = sizeof(Packet_t);
    uint8_t *buf_ptr = buf;

    while (need > 0) // The space over menory
    {
        ssize_t recv_var = recv(sock, (char *)buf_ptr, (int)need, 0); //  ssize_t -> long long size_t 32bit
                                                                      //  size_t -> unsigned char
        if (recv_var <= 0)
        {
            return -1;
        }

        buf_ptr += recv_var;      // This algthrom need understend
        need -= (size_t)recv_var; // The data type alignment
    }

    /*The CRC check vaildation*/
    Packet_t *packt = (Packet_t *)buf; // buffer data transformer
    const uint8_t *AES_key = security_get_key();

    bool crc_ok = protocol_crc_validate(buf, sizeof(Packet_t));
    if (!crc_ok)
    {
        printf(" The CRC validation is fail. ");
        return -1;
    }

    /*The AES decode body*/
    size_t body_len = packt->header.body_len;    // 這邊就是要解密 body
    uint8_t *body_ptr = (uint8_t *)&packt->body; // The uint8_t * is used for pointer content
                                                 //  The nomraize is used (uint8_t) can do

    int aes_decrypt_result = aes_decrypt(body_ptr, body_ptr, body_len, AES_key, packt->header.aes_iv); // The decrypt;

    if (aes_decrypt_result != 0)
    {
        printf("AES decrypt fail \n");
        return -1;
    }

    /* The parser handle process sock*/
    ParserResult_t result = parse_protocol(buf, sizeof(Packet_t), output);
    if (result != PARSER_OK)
    {
        printf("parse_protocol error: %d\n", result);
        if (result == PARSER_BUFFER_TOO_SHORT)
            printf("  -> BUFFER_TO_SHORT\n");
        if (result == PARSER_CRC_FAIL)
            printf("  -> CRC_FAIL\n");
        if (result == PARSER_VERSION_ERROR)
            printf("  -> VERSION_ERROR\n");
        if (result == PARSER_SENSOR_UNKNOWN)
            printf("  -> SENSOR_UNKNOWN\n");
        return -1;
    }
    return 0;
}

/*The program flow*/
/*
clinet
    first encode AES -> CRC Caluater

server
    CRC inspection -> decode AES


*/