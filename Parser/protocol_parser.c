#include <stdio.h>
#include <string.h>

#include "protocol_parser.h"
#include "protocol_crc_16.h"

ParserResult_t parse_protocol(const uint8_t *input, size_t len, ParsedData_t *output)
{

    /*Boundary condition setting */
    if (!input || !output || len < sizeof(Packet_t)) //! input == (input== NULL) != Ture -> False ,False->True
    {
        return PARSER_BUFFER_TOO_SHORT;
    }

    /*1. need Trainsformer package sturcut*/
    /* Do the Datatype Alignment*/
    const Packet_t *package = (const Packet_t *)input;

    /*2. version checek*/
    if (package->header.version != 1)
    {
        return PARSER_VERSION_ERROR;
    }

    /*3. clear output menmory space*/
    memset(output, 0, sizeof(*output));

    output->machine_id = ntohl(package->header.machine_id); // Big end to transformer small end, notol = 32bit.
    output->timestamp_sec = ntohl(package->header.timestamp_sec);
    output->seq_no = ntohs(package->header.seq_no); // ntohs = 16 bit
    output->sensor_type = package->header.sensor_type;

    /*4 .The body variable switsh*/
    switch (package->header.sensor_type) // sensor type-> to switch deferent information content.
    {
    case SENSOR_TYPE_VIBRATION:
    {
        const Payload_Vibration_t *vibration = &package->body.vibration; // pointer to package body

        output->vibration.valid = true;
        output->vibration.velocity_rms_x = ntohs(vibration->velocity_rms_x) / 100.0f;
        output->vibration.velocity_rms_y = ntohs(vibration->velocity_rms_y) / 100.0f;
        output->vibration.velocity_rms_z = ntohs(vibration->velocity_rms_z) / 100.0f;
        output->vibration.accel_peak = ntohs(vibration->accel_peak) / 100.0f;
        output->vibration.status_flags = vibration->status_flags;

        break;
    }

    case SENSOR_TYPE_CURRENT:
    {
        const Payload_Current_t *current = &package->body.current;

        output->current.valid = true;
        output->current.current_rms = ntohs(current->current_rms) / 100.0f;
        output->current.power_watts = ntohs(current->power_watts) / 10.0f;
        output->current.total_energy_wh = (double)ntohl(current->total_energy);

        break;
    }

    case SENSOR_TYPE_ENV:
    {
        const Payload_Env_t *env = &package->body.env;
        int16_t temperature_raw = (int16_t)ntohs(*(const uint16_t *)&env->temperature);
        // for Endianness transformer
        uint16_t humidity_raw = ntohs(env->humidity);

        output->env.valid = true;
        output->env.temperature = temperature_raw / 100.0f;
        output->env.humidity = humidity_raw / 100.0f;
        output->env.illuminance_lux = (float)ntohl(env->illuminance_lux);

        break;
    }

    default:
        return PARSER_SENSOR_UNKNOWN;
    }

    return PARSER_OK;
}

void protocol_print_packet(const ParsedData_t *data) // The print implemnt
{
    if (!data)
    {
        return;
    }

    printf("Machine ID : %u\n", data->machine_id);
    printf("Timestamp  : %u\n", data->timestamp_sec);
    printf("Seq        : %u\n", data->seq_no);

    Sensortype_t sensor_type = (Sensortype_t)data->sensor_type; // The data transformer.

    switch (sensor_type)
    {
    case SENSOR_TYPE_VIBRATION:
        if (data->vibration.valid)
        {
            printf("[VIBRATION] vel X : %.2f mm/s\n", data->vibration.velocity_rms_x);
            printf("[VIBRATION] vel Y : %.2f mm/s\n", data->vibration.velocity_rms_y);
            printf("[VIBRATION] vel Z : %.2f mm/s\n", data->vibration.velocity_rms_z);
            printf("[VIBRATION] accel : %.2f g\n", data->vibration.accel_peak);
            printf("[VIBRATION] flags : 0x%02X\n", data->vibration.status_flags);
        }
        break;

    case SENSOR_TYPE_CURRENT:
        if (data->current.valid)
        {
            printf("[CURRENT] I_rms : %.2f A\n", data->current.current_rms);
            printf("[CURRENT] P     : %.2f W\n", data->current.power_watts);
            printf("[CURRENT] E     : %.3d Wh\n", data->current.total_energy_wh);
        }
        break;

    case SENSOR_TYPE_ENV:
        if (data->env.valid)
        {
            printf("[ENV] Temp  : %.2f °C\n", data->env.temperature);
            printf("[ENV] RH    : %.2f %%\n", data->env.humidity);
            printf("[ENV] Lux   : %.2f lx\n", data->env.illuminance_lux);
        }
        break;

    default:
        printf("[UNKNOWN SENSOR TYPE]\n");
        break;
    }
}