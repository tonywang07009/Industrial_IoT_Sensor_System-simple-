#ifndef _PROTOCOL_SOCKET_H_
#define _PROTOCOL_SOCKET_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "net_compatblie.h"
#include "../Data_sturct/Package_Data_Struct.h"
#include "../Parser/protocol_parser.h"
#include "../Parser/protocol_crc_16.h"

int net_connect(const char *ip, uint16_t port, net_socket_t *out_sock);
int proto_send_packet(net_socket_t sock, const Packet_t *package);
int proto_recv_and_parse(net_socket_t sock, ParsedData_t *output, int timeout_ms);

#endif