#ifndef __STATS_H__
#define __STATS_H__

#include <math.h>
#include <stdio.h>
#include <stdint.h>

/*The data struct*/

typedef struct statistics_sample_information
{
    double value;
    uint32_t machine_id;
    uint32_t timestamp_sec; // For the timestamp used;

} StatSample_t;

/*The statistics*/

typedef struct sample_accumulator
{
    uint32_t count;    // The sample N
    double sum;        // The sample summray
    double sum_square; // The Variance summray

} StatAccumulator_t;

typedef struct statistics_result
{

    uint32_t count;
    double mean;
    double stddev;
    double z_score; // The z-test.

} StatResult_t;

/*The Accumulator initlization */

void stats_reset(StatAccumulator_t *acc);

/*The add sample */

void stats_add_sample(StatAccumulator_t *acc, const StatSample_t *sample);
// The fix information is will put in last

/*The statistics result dispaly*/

int stats_compute(const StatAccumulator_t *acc, double last_value, StatResult_t *out);

#endif /*__STATS_H__*/