#ifndef __PROTOCOL_PARSER_H__
#define __PROTOCOL_PARSER_H__
// protocol_parser.h

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "../Data_sturct/Package_Data_Struct.h"
// .. ->上一個資料夾

/*相容性*/
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h> // 小端域大端序轉換
// #include<arpa/inet.h> -> for linux system
#else
#include <arpa/inet.h> // linux
#endif

/*ANSI 顏色 研究編碼*/
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RED "\033[31m"

#define COLOR_RESET "\033[0m"

typedef enum
{
    PARSER_OK = 0,
    PARSER_CRC_FAIL,       // CRC 撿測失敗
    PARSER_VERSION_ERROR,  // 封包版本錯誤
    PARSER_SENSOR_UNKNOWN, // 未知感測器
    PARSER_BUFFER_TO_SHORT // BUFFER 空間不夠

} ParserResult_t;

typedef struct Parseddata // intergration Data_sturct

{
    uint8_t sensor_type;

    uint16_t package_version; // The packeage sequence number
    uint16_t seq_no;

    uint32_t meachine_id;
    uint32_t timestamp_sec;

    // Vibration 數據 (單位已轉換為浮點)
    struct
    {
        float velocity_rms_x, velocity_rms_y, velocity_rms_z; // mm/s
        float accel_peak;                                     // 加速度                                     // g
        uint8_t status_flags;                                 // The meachine status flag
        bool valid;                                           // The sign -> 驗置

    } vibration;

    // Current 數據
    struct
    {
        float current_rms;        // A
        float power_watts;        // W
        uint32_t total_energy_wh; // Wh
        bool valid;

    } current;

    // Env 數據
    struct
    {
        float temperature;     // °C
        float humidity;        // %RH
        float illuminance_lux; // Lux
        bool valid;
    } env;

} ParsedData_t;

/*API*/
ParserResult_t parse_protocol(const uint8_t *input, size_t len, ParsedData_t *output);
void protocol_print_packet(const ParsedData_t *data);

#endif