# Statistics Module ('Statistics_tool\')

## 1. Responsibility

- Maintian online statistics information (count, mean, variance, stddev) for each machine.
- Compute Z-score (Sample > 35) for latest sample and map to SPC status (GREEN / ORANGE / RED / CRITICAL).
- Provide thread-safe API for server call.


## 2. Main Data Structures

- `StatSample_t`： sample（value, machine_id, timestamp_sec）。
- `StatAccumulator_t`：accumulate state（count, mean, M2）。
- `StatResult_t`：計算後結果（count, mean, stddev, z_score, status）。

## 3. Public API

```
void stats_reset(StatAccumulator_t *acc)

void stats_add_sample(StatAccumulator_t *acc, const StatSample_t *sample)

int stats_compute(const StatAccumulator_t *acc, double last_value, StatResult_t *out)

void stats_print_result(uint32_t machine_id, const StatResult_t *result)

```

## 4. SPC/Z-score Logic
- Z-score define: `z=(x-mean)/stddev` 
- N <35: only warm-up provide , don't opening spc jugement.

- Color correspond:

- |Z| < 1.0->GREEN
- 1.0 <= |Z| <= 1.645 -> ORANGE
- 1.645 <= |Z| <= 1.96 -> RED
- |Z| > 1.96 -> CRITICAL

## 5. Integration
- Server recive vibration X :
    1. Composition `StatSample_t` 
    2. Call `stats_add_sample()` update cumulative value
    3. Call `stats_compute()` get `StatResult_t`。'
    4. Call  `stats_print_result()` output SPC state