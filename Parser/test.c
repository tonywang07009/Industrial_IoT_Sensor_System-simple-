#include <stdio.h>
#include <stdlib.h>
#include "protocol_parser.h"
#include "protocol_crc_16.h"

int main(void)
{
    uint8_t buf[sizeof(Packet_t)] = {0};
    // The simluation packet coming
    Packet_t *packaget = (Packet_t *)buf;

    // The into data test
    packaget->header.version = 1;
    packaget->header.machine_id = 123;

    // The CRC calucation Test
    packaget->checksum = protocol_crc16(buf, (sizeof(Packet_t) - 2));
    // -2 -> 用於 crc 檢測碼

    ParsedData_t output;

    ParserResult_t result = parse_protocol(buf, sizeof(Packet_t), &output);

    printf("Result=%d , Meachine_id=%u\n", result, output.meachine_id);

    return 0;
}
