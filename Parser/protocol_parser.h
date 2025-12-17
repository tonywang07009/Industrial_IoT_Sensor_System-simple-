// protocol_parser.h
#ifndef PROTOCOL_PARSER_H
#define PROTOCOL_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include "..\Data_sturct\Package_Data_Struct.h"
// .. ->上一個資料夾
typedef enum
{
    PARSER_OK = 0,
    PARSER_CRC_FAIL,       // CRC 撿測失敗
    PARSER_VERSION_ERROR,  // 封包版本錯誤
    PARSER_SENSOR_UNKNOWN, // 未知感測器
    PARSER_BUFFER_SHORT    // BUFFER 空間不夠

} ParserResult_t;

#endif