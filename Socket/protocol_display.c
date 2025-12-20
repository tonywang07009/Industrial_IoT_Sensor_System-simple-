#include "protocol_display.h"

void print_parsed(const ParsedData_t *data)
{
    printf("Meachine ID =%u,Time=%u,seq=%u\n",
           data->meachine_id, data->timestamp_sec, data->seq_no);
    // print data content.

    switch (data->sensor_type) // The switch type
                               // %u -> unsiged int
    {
    case SENSOR_TYPE_VIBRATION:
    {
        if (data->vibration.valid)
        {
            printf("[VIB] velocity X: %.2f mm/s\n", data->vibration.velocity_rms_x);
            printf("[VIB] velocity Y: %.2f mm/s\n", data->vibration.velocity_rms_y);
            printf("[VIB] velocity Z: %.2f mm/s\n", data->vibration.velocity_rms_z);
            printf("[VIB] velocity acceleration: %.2f g \n", data->vibration.accel_peak);
        }
        break;
    }
    case SENSOR_TYPE_CURRENT:
    {
        if (data->current.valid) // Switch Ptr
        {
            printf("[CUR] current: %.2f A\n", data->current.current_rms);
            printf("[CUR  power: %.2f W\n", data->current.power_watts);                 // power_watts ->ampfily
            printf("[CUR] electric energy: %.3d Wh \n", data->current.total_energy_wh); // The energy spending.
        }
        break;
    }
    case SENSOR_TYPE_ENV:
    {
        if (data->env.valid)
        {
            printf("[ENV] temperature: %.2f", data->env.temperature);
            printf("[ENV] humidity: %.2f", data->env.humidity);
            printf("[ENV] illuminance: %.2f", data->env.illuminance_lux);
        }
        break;

    default:
        printf("[UNKNOWN SENSOR]\n");
        break;
    }
    }
}