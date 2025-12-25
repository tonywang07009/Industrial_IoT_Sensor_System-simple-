#include "protocol_crc_16.h"
bool protocol_crc_validate(const uint8_t *packet, size_t len)
{                // size_t == use least 16 bit 2 byte space
    if (len < 2) // The pack check is fail
    {
        return false;
    }
    else
    {
        uint16_t received_crc = ((uint16_t)packet[len - 2] << 8) | packet[len - 1];
        uint16_t calc_crc = protocol_crc16(packet, (len - 2));
        // the crc check code comparse.
        if (calc_crc == received_crc)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

uint16_t protocol_crc16(const uint8_t *data, size_t len)
{
    uint16_t crc = 0XFFFF; // setting check basic

    for (size_t i = 0; i < len; i++)
    {
        /*step1 the data initlization */
        crc ^= (uint16_t)data[i] << 8; // 高位元先
        // 1111111101111111
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x8000)
            {
                crc = (crc << 1) ^ 0x1021; // CRC-CCITT
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}
