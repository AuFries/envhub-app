#ifndef SYSTEM_USAGE_H
#define SYSTEM_USAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

    typedef struct
    {
        float cpu_percent;
        float mem_percent;
    } system_usage_t;

    /**
     * Initialize the system usage module.
     *
     * Must be called once before system_usage_read().
     *
     * Returns true on success, false on failure.
     */
    bool system_usage_init(void);

    /**
     * Read current CPU and memory utilization.
     *
     * CPU utilization is calculated from the delta between the current and
     * previous /proc/stat sample, so this function should be called periodically
     * (for example once per second).
     *
     * Returns true on success, false on failure.
     */
    bool system_usage_read(system_usage_t *out);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_USAGE_H */