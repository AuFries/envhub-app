#ifndef SYSTEM_STATS_H
#define SYSTEM_STATS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

    typedef struct
    {
        float cpu_percent;
        float mem_percent;
        time_t wall_time_epoch_s;
        uint64_t uptime_seconds;
    } system_stats_t;

    /**
     * Initialize the system stats module.
     *
     * Must be called once before system_stats_read().
     *
     * Returns true on success, false on failure.
     */
    bool system_stats_init(void);

    /**
     * Read current CPU utilization, memory utilization, wall time, and uptime.
     *
     * CPU utilization is calculated from the delta between the current and
     * previous /proc/stat sample, so this function should be called periodically
     * (for example once per second).
     *
     * Returns true on success, false on failure.
     */
    bool system_stats_read(system_stats_t *out);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_STATS_H */