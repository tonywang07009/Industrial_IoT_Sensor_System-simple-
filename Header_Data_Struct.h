#ifndef __HEADER_DATA__
#define __HEADER_DATA__

#include <stdint.h>   // for 標準資料單位
#pragma pack(push, 1) // 記憶體對齊強制
                      // 將資料連接串起

/*The Sensortype*/
typedef enum Sensortype // the enum sepc is all big wirte
{
    SENSOR_TYPE_VIBRATION = 0X01, // 振幅
    SENSOR_TYPE_CURRENT = 0X02,   // 電流
    SENSOR_TYPE_ENV = 0X03,       // 溫度
    SENSOR_TYPE_HUMIDITY = 0X04,  // 濕度
    SENSOR_TYPE_LIGHT = 0X05      // 光

} Sensortype_t; // union code // The member specify number is can change enum role.

/*The operation type*/
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

    uint32_t machine_id;
    uint32_t timestamp_sec; // unix time 時間戳

    uint16_t seq_no; // 包的序列號

} Protocol_Header_t; //_t :ansi encode

#pragma pack(pop)
#endif