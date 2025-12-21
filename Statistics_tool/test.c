#include "statistics.h"

#define N1 20
#define N2 20

double v1 = 300.0;
double v2 = 400.0;

int main(void)
{
    StatAccumulator_t acc;
    stats_reset(&acc);

    // 前 20 筆：300
    for (int i = 0; i < N1; ++i)
    {
        StatSample_t s = {.value = 300.0, .machine_id = 1, .timestamp_sec = i + 1};
        stats_add_sample(&acc, &s);

        StatResult_t r;
        if (stats_compute(&acc, s.value, &r) == 0)
        {
            printf("[v1] N=%u, mean=%.2f, stddev=%.4f, z=%.4f\n",
                   r.count, r.mean, r.stddev, r.z_score);
        }
    }

    // 後 20 筆：400
    for (int i = 0; i < N2; ++i)
    {
        StatSample_t s = {.value = 400.0, .machine_id = 1, .timestamp_sec = N1 + i + 1};
        stats_add_sample(&acc, &s);

        StatResult_t r;
        if (stats_compute(&acc, s.value, &r) == 0)
        {
            printf("[v2] N=%u, mean=%.2f, stddev=%.4f, z=%.4f\n",
                   r.count, r.mean, r.stddev, r.z_score);
        }
    }
    return 0;
}
