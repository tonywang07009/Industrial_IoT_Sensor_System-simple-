#ifndef __RANDOM_FILL_PARMERTERS__
#define __RANDOM_FILL_PARMERTERS__

#include <stdlib.h> // The call random()
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "Package_Data_Struct.h"

static uint16_t rand_u16_in_range(uint16_t min, uint16_t max);
bool random_fill_sensor_payload(Packet_t *package, double bad_prob);
bool random_fill_parameters(Packet_t *package, double bad_prob);

#endif