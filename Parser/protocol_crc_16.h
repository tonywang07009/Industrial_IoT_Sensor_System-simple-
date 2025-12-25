#ifndef __PARSER_CRC_16__
#define __PARSER_CRC_16__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*The function*/
uint16_t protocol_crc16(const uint8_t *data, size_t len);
bool protocol_crc_validate(const uint8_t *input, size_t len);
// size_t -> 封包長度?

#endif