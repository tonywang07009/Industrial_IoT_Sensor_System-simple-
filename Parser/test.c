#include <stdio.h>
#include "Parser_crc_16.h"

int main()
{
    // 測試資料：簡單的 4 bytes
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04};
    uint16_t crc = protocol_crc16(test_data, 4);

    printf("Test Data: 01 02 03 04\n");
    printf("CRC16: 0x%04X\n", crc);
    printf("Expected: 0xD0C3 (know correct )\n");

    // 完整封包測試
    uint8_t test_packet[] = {0x01, 0x02, 0x03, 0x04, 0xC3, 0xD0}; // + CRC
    bool valid = protocol_crc_validate(test_packet, 6);
    printf("Packet CRC Valid: %s\n", valid ? "PASS" : "FAIL");

    return 0;
}
