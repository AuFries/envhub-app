#include "system_usage.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
} cpu_stat_t;

static bool g_initialized = false;
static cpu_stat_t g_prev_cpu_stat;

static bool read_cpu_stat(cpu_stat_t *out)
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp)
    {
        return false;
    }

    /*
     * Format of first line:
     * cpu  user nice system idle iowait irq softirq steal guest guest_nice
     *
     * We only need the first 8 fields after "cpu".
     */
    int scanned =
        fscanf(fp, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu", &out->user, &out->nice,
               &out->system, &out->idle, &out->iowait, &out->irq, &out->softirq, &out->steal);

    fclose(fp);

    return (scanned == 8);
}

static float compute_cpu_percent(const cpu_stat_t *prev, const cpu_stat_t *curr)
{
    uint64_t prev_idle = prev->idle + prev->iowait;
    uint64_t curr_idle = curr->idle + curr->iowait;

    uint64_t prev_non_idle =
        prev->user + prev->nice + prev->system + prev->irq + prev->softirq + prev->steal;
    uint64_t curr_non_idle =
        curr->user + curr->nice + curr->system + curr->irq + curr->softirq + curr->steal;

    uint64_t prev_total = prev_idle + prev_non_idle;
    uint64_t curr_total = curr_idle + curr_non_idle;

    uint64_t total_delta = curr_total - prev_total;
    uint64_t idle_delta = curr_idle - prev_idle;

    if (total_delta == 0U)
    {
        return 0.0f;
    }

    return ((float)(total_delta - idle_delta) * 100.0f) / (float)total_delta;
}

static bool read_mem_percent(float *out_mem_percent)
{
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp)
    {
        return false;
    }

    char key[64];
    unsigned long long value_kb;
    char unit[32];

    unsigned long long mem_total_kb = 0;
    unsigned long long mem_available_kb = 0;

    while (fscanf(fp, "%63s %llu %31s", key, &value_kb, unit) == 3)
    {
        if (strcmp(key, "MemTotal:") == 0)
        {
            mem_total_kb = value_kb;
        }
        else if (strcmp(key, "MemAvailable:") == 0)
        {
            mem_available_kb = value_kb;
        }

        if ((mem_total_kb != 0U) && (mem_available_kb != 0U))
        {
            break;
        }
    }

    fclose(fp);

    if (mem_total_kb == 0U)
    {
        return false;
    }

    if (mem_available_kb > mem_total_kb)
    {
        mem_available_kb = mem_total_kb;
    }

    unsigned long long used_kb = mem_total_kb - mem_available_kb;
    *out_mem_percent = ((float)used_kb * 100.0f) / (float)mem_total_kb;

    return true;
}

bool system_usage_init(void)
{
    if (!read_cpu_stat(&g_prev_cpu_stat))
    {
        g_initialized = false;
        return false;
    }

    g_initialized = true;
    return true;
}

bool system_usage_read(system_usage_t *out)
{
    if (out == NULL)
    {
        return false;
    }

    if (!g_initialized)
    {
        if (!system_usage_init())
        {
            return false;
        }
    }

    cpu_stat_t curr_cpu_stat;
    if (!read_cpu_stat(&curr_cpu_stat))
    {
        return false;
    }

    out->cpu_percent = compute_cpu_percent(&g_prev_cpu_stat, &curr_cpu_stat);
    g_prev_cpu_stat = curr_cpu_stat;

    if (!read_mem_percent(&out->mem_percent))
    {
        return false;
    }

    return true;
}