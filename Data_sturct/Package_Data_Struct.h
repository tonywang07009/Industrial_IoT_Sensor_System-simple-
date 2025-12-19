#ifndef __Package_DATA__
#define __Package_DATA__

#include "Header_Data_Struct.h"
#include "Body_Data_Struct.h"

typedef struct package_content
{
    Protocol_Header_t header;
    Protocol_Body_t body;
    uint16_t checksum; // 檢測碼

} Packet_t;

#endif