#ifndef __HEADER_DATA__
#define __HEADER_DATA__

#include <stdint.h>
#pragma pack(push, 1)

/*The Sensortype*/
typedef enum Sensortype
{
    SENSOR_TYPE_VIBRATION = 0X01, // vibration
    SENSOR_TYPE_CURRENT = 0X02,   // current
    SENSOR_TYPE_ENV = 0X03,       // envinorment
    SENSOR_TYPE_MAX = 0X04

} Sensortype_t; // union code // The member specify number is can change enum role.

/*The Operation type*/
typedef enum Operationtype
{
    OPCODE_DATA_REPORT = 0X10,  // Report the data
    OPCODE_DEVICE_LOGIN = 0X20, // The Equiment sign in the system
    OPCODE_ALARM_EVENT = 0X30,  // The alarm is opening.
    OPCODE_HEARTBEAT = 0X00     // Check your equiment still alvie.
} OpCode_t;

typedef struct Protocol_Header
{
    uint8_t version;     // The package version
    uint8_t op_code;     // The operation
    uint8_t sensor_type; // The sensor recive payload
    uint8_t aes_iv[16];  // The aes_check
    uint16_t body_len;   // length of encrypted body
    uint16_t seq_no;     //
    uint32_t machine_id;
    uint32_t timestamp_sec; // unix time

} Protocol_Header_t; //_t :ansi encode

#pragma pack(pop)
#endif