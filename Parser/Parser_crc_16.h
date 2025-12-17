#ifndef __PARSER_CRC_16__
#define __PARSER_CRC_16__

#include <stdint.h>
#include <stdbool.h>

/*The function*/
uint16_t protocol_crc16(const uint8_t *data, size_t len);
// 這邊的 資料長度內容已被鎖定起來,不能修改
bool protocol_crc_validate(const uint8_t *packet, size_t len);
// size_t -> 封包長度?

#endif