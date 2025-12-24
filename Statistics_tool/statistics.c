#include "statistics.h"

/*The add sample */
void stats_reset(StatAccumulator_t *acc)
{
    if (!acc)
    {
        printf("The acc space is uncreate \n");
        return;
    }
    acc->count = 0;
    acc->sum = 0;
    acc->sum_square = 0;
}

// The fix information is will put in lasts

void stats_add_sample(StatAccumulator_t *acc, const StatSample_t *sample)
{
    if (!acc || !sample)
    {
        printf("The space is uncreate");
        return;
    }

    double temp_value = sample->value;

    acc->count += 1;
    acc->sum += temp_value;
    acc->sum_square += (temp_value * temp_value);
}
/*The statistics result dispaly*/

int stats_compute(const StatAccumulator_t *acc, double last_value, StatResult_t *out)
{

    if (!acc || !out)
    {
        printf("The space is uncreate \n");
        return -1;
    }

    if (acc->count < 2) // The samole is too less.
    {
        printf("Not enough samples for stddev \n");
        return -1;
    }
    out->count = acc->count;

    double n = (double)acc->count;
    double mean = (acc->sum / n);
    double mean_sq = (acc->sum_square / n);   // E[x^2]
    double var_pop = mean_sq - (mean * mean); // variance
    double var_sample = var_pop * n / (n - 1.0);
    double sstddev = sqrt(var_sample);
    double z_value = 0;
    if (sstddev == 0.0)
    {
        z_value = 0.0;
    }
    else
    {
        z_value = (last_value - mean) / (sstddev);
    }

    out->mean = mean;
    out->stddev = sstddev;
    out->z_score = z_value;

    return 0;
}