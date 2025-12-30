#include "random_fill_parameters.h"

static uint16_t rand_u16_in_range(uint16_t min, uint16_t max)
{
    uint16_t rand_value = (uint16_t)(min + (rand() % (max - min + 1)));
    // +1 : include max
    return rand_value;
}

bool random_fill_sensor_payload(Packet_t *package, double bad_probility)
{
    if (!package)
    {
        printf("don't recive any package");
        return false;
    }

    /*step 1 probilty setting*/
    int rand_value = rand();
    double probility = (double)rand() / RAND_MAX; // The  RAND_MAX is keep world
                                                  // The value range [0.0~1.0]
    bool is_bad = (probility < bad_probility);    // if  p < b  == ture , but the ! will tern false
                                                  // p > b== false , but the ! will tern ture
    /*step 2 switch from the sensor type*/
    switch (package->header.sensor_type)
    {
    case SENSOR_TYPE_VIBRATION:
        if (!is_bad)
        {
            package->body.vibration.velocity_rms_x = htons(rand_u16_in_range(80, 150));
            package->body.vibration.velocity_rms_y = htons(rand_u16_in_range(80, 150));
            package->body.vibration.velocity_rms_z = htons(rand_u16_in_range(80, 150));
            package->body.vibration.accel_peak = htons(rand_u16_in_range(0, 2));
            package->body.vibration.status_flags = 0;
        }
        else
        {
            package->body.vibration.velocity_rms_x = htons(rand_u16_in_range(160, 400));
            package->body.vibration.velocity_rms_y = htons(rand_u16_in_range(160, 400));
            package->body.vibration.velocity_rms_z = htons(rand_u16_in_range(160, 400));
            package->body.vibration.accel_peak = htons(rand_u16_in_range(3, 6));
            package->body.vibration.status_flags = 1; // alarm
        }
        break;

    case SENSOR_TYPE_CURRENT:
        if (!is_bad)
        {
            package->body.current.current_rms = htons(rand_u16_in_range(30, 150));
            package->body.current.power_watts = htons(rand_u16_in_range(75, 200));
            package->body.current.total_energy = htonl(rand_u16_in_range(0, 1000)); // demo 用，累積能量
        }
        else
        {
            package->body.current.current_rms = htons(rand_u16_in_range(160, 400));
            package->body.current.power_watts = htons(rand_u16_in_range(210, 600));
            package->body.current.total_energy = htonl(rand_u16_in_range(1000, 5000));
        }
        break;

    case SENSOR_TYPE_ENV:
        if (!is_bad)
        {
            package->body.env.temperature = htons((int16_t)rand_u16_in_range(15, 30));
            package->body.env.humidity = htons(rand_u16_in_range(30, 75));
            package->body.env.illuminance_lux = htonl(rand_u16_in_range(2600, 10000));
        }
        else
        {
            package->body.env.temperature = htons((int16_t)rand_u16_in_range(31, 45));
            package->body.env.humidity = htons(rand_u16_in_range(80, 100));
            package->body.env.illuminance_lux = htonl(rand_u16_in_range(0, 2000));
        }
        break;

    default:
        printf("UNKNOW SENSOR TYPE");
        memset(&package->body, 0, sizeof(package->body));
        break;
    }

    return is_bad;
}

bool random_fill_parameters(Packet_t *package, double bad_probility)
{
    /*step 1 random() decision sensor for this package*/
    if (!package)
    {
        return false;
    }

    int sensor = (rand() % (SENSOR_TYPE_MAX - 1)) + 1; // point for last

    package->header.sensor_type = (uint8_t)sensor; // The data type aligment
    memset(&package->body, 0, sizeof(package->body));

    int is_bad = random_fill_sensor_payload(package, bad_probility);

    return is_bad;
}