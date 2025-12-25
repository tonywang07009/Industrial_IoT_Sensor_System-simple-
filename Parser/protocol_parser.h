#ifndef __PROTOCOL_PARSER_H__
#define __PROTOCOL_PARSER_H__
// protocol_parser.h

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "../Data_sturct/Package_Data_Struct.h"
// The call from last datafolder

/*compatibility*/
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h> //Windows socket

#else
#include <arpa/inet.h> // linux socket

#endif

/*The color coding*/
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"

typedef enum
{
    PARSER_OK = 0,
    PARSER_CRC_FAIL,
    PARSER_VERSION_ERROR,
    PARSER_SENSOR_UNKNOWN,
    PARSER_BUFFER_TOO_SHORT

} ParserResult_t;

typedef struct ParsedData // intergration Data_sturct

{
    uint8_t sensor_type;
    uint16_t package_version;
    uint16_t seq_no;
    uint32_t machine_id;
    uint32_t timestamp_sec;

    struct Vibration
    {
        float velocity_rms_x, velocity_rms_y, velocity_rms_z;
        float accel_peak;
        uint8_t status_flags;
        bool valid;

    } vibration;

    struct Current
    {
        float current_rms;
        float power_watts;
        double total_energy_wh;
        bool valid;

    } current;

    struct Env
    {
        float temperature;
        float humidity;
        float illuminance_lux;
        bool valid;
    } env;

} ParsedData_t;

/*API*/
ParserResult_t parse_protocol(const uint8_t *input, size_t len, ParsedData_t *output);
void protocol_print_packet(const ParsedData_t *data);

#endif