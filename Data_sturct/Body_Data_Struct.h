#ifndef __BODY_DATA__
#define __BODY_DATA__

#include <stdint.h>
#pragma pack(push, 1)

typedef struct Vibration // for 震動
{

    uint8_t status_flags;    // 0=Normal, 1=Warning, 2=Critical
                             // 計算目前的震動會不會超過 450條(PLC 計算)
    uint16_t velocity_rms_x; // 單位: 0.01 mm/s (例: 450 = 4.50 mm/s)
    uint16_t velocity_rms_y;
    uint16_t velocity_rms_z;
    uint16_t accel_peak; // 單位: 0.01 g

} Payload_Vibration_t;

typedef struct Current
{
    uint16_t current_rms; // unit : 10mA (0.01A)
    // 刀具頓掉,伺服馬達扭力上升 , 電流變大
    uint16_t power_watts;  // unit 0.1w
    uint32_t total_energy; // unit Wh
                           // 計算單個工件的能源成本 (OEE 計算)
} Payload_Current_t;

typedef struct Env
{
    int16_t temperature;      // Becouse use The int ,so that can store neagtive // 環 檢測環境溫度是否有問題                       // unit: 0.01 度c
    uint16_t humidity;        // unit: 0.01% RH //檢測是否濕度過高會導致作業員工作效率下降與產品品質控管
    uint32_t illuminance_lux; // unit : Lux 環:檢測是否因為環境光過低導致工人誤判

} Payload_Env_t;

typedef union PayloadDataType
{
    Payload_Vibration_t vibration;
    Payload_Current_t current;
    Payload_Env_t env;

    uint8_t raw_bytes[64];
    // uint 8bit , 長度 64 byte
    // The union fix max is gave 64 byte
} Protocol_Body_t;

#pragma pack(pop) // 稽查用法
#endif
