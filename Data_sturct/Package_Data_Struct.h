#ifndef __Package_DATA__
#define __Package_DATA__

#include "Header_Data_Struct.h"
#include "Body_Data_Struct.h"

typedef struct Package_content
{
    Protocol_Header_t header;
    Protocol_Body_t body;
    uint16_t checksum; // crc check code

} Packet_t;

#endif