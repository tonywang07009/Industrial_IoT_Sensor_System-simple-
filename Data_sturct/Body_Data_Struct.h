#ifndef __BODY_DATA__
#define __BODY_DATA__

#include <stdint.h>
#pragma pack(push, 1)
// check the compiler don't do the byte alignment

/* Units velocity in 0.01mm/s,accel in 0.01g*/
/* This is for measure for machine fatigue and tool or fixture imbalance */
typedef struct VibrationPayload
{
    uint8_t status_flags;    // bit 0=Normal, bit 1=Warning, bit 2=Critical
    uint16_t velocity_rms_x; // Example : 350 = 3.50 mm/s
    uint16_t velocity_rms_y;
    uint16_t velocity_rms_z;
    uint16_t accel_peak; // Example : 150 =1.50g

} Payload_Vibration_t;

/* Units current in 10mA , power_watts in 0,1w , total_energy in Wh  */
/* This is for measure aberrant for machine processing , for example: Belt slipping
   and measure power spending*/
typedef struct CurrentPayload
{
    uint16_t current_rms;
    uint16_t power_watts; // Example 100 = 10w
    uint32_t total_energy;

} Payload_Current_t;

/*Units temperature in Celsius , humidity in 0.01% RH , illuminance_lux in Lux */
/*This is for check the product quality and measure the Comfortable for worker */
typedef struct EnvPayload
{
    int16_t temperature;
    uint16_t humidity; // Example 7500 = 75% RH
    uint32_t illuminance_lux;

} Payload_Env_t;

/*Body mean: change value */
/*This space max amount :64 byte*/
typedef union PacketBody
{
    Payload_Vibration_t vibration;
    Payload_Current_t current;
    Payload_Env_t env;

    uint8_t raw_bytes[64];

} Protocol_Body_t;

#pragma pack(pop)
// Recover the byte alignment

#endif
