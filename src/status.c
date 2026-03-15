#include "status.h"
#include "data_logger.h"

#include <stdio.h>
#include <string.h>

static sensor_status_entry_t g_sensor_status[STATUS_SENSOR_COUNT];
static char g_summary_buf[64];

const char *status_service_sensor_name(status_sensor_id_t sensor_id)
{
    switch (sensor_id)
    {
        case STATUS_SENSOR_BQ27441:
            return "BQ27441";
        case STATUS_SENSOR_SCD30:
            return "SCD30";
        case STATUS_SENSOR_BMP580:
            return "BMP580";
        case STATUS_SENSOR_SGP30:
            return "SGP30";
        case STATUS_SENSOR_COUNT:
        default:
            return "Unknown";
    }
}

static status_severity_t severity_from_sensor_status(sensor_status_t status)
{
    switch (status)
    {
        case SENSOR_STATUS_OK:
            return STATUS_SEV_INFO;

        case SENSOR_STATUS_MISSING:
        case SENSOR_STATUS_STALE:
        case SENSOR_STATUS_ERROR:
        default:
            return STATUS_SEV_WARNING;
    }
}

static void fill_entry_text(sensor_status_entry_t *entry)
{
    const char *name;

    if (!entry)
        return;

    name = status_service_sensor_name(entry->sensor_id);

    switch (entry->sensor_status)
    {
        case SENSOR_STATUS_MISSING:
            snprintf(entry->summary, sizeof(entry->summary), "%s missing", name);
            snprintf(entry->detail, sizeof(entry->detail), "%s is not registered on the system",
                     name);
            break;

        case SENSOR_STATUS_STALE:
            snprintf(entry->summary, sizeof(entry->summary), "%s stale", name);
            snprintf(entry->detail, sizeof(entry->detail),
                     "%s has not produced fresh data recently", name);
            break;

        case SENSOR_STATUS_ERROR:
            snprintf(entry->summary, sizeof(entry->summary), "%s read error", name);
            snprintf(entry->detail, sizeof(entry->detail),
                     "%s is present but reads are consistently failing", name);
            break;

        case SENSOR_STATUS_OK:
        default:
            snprintf(entry->summary, sizeof(entry->summary), "OK");
            snprintf(entry->detail, sizeof(entry->detail), "%s operating normally", name);
            break;
    }
}

void status_service_init(void)
{
    int i;

    memset(g_sensor_status, 0, sizeof(g_sensor_status));
    memset(g_summary_buf, 0, sizeof(g_summary_buf));

    for (i = 0; i < STATUS_SENSOR_COUNT; i++)
    {
        g_sensor_status[i].sensor_id = (status_sensor_id_t)i;
        g_sensor_status[i].sensor_status = SENSOR_STATUS_OK;
        g_sensor_status[i].severity = STATUS_SEV_INFO;
        g_sensor_status[i].active = false;
        g_sensor_status[i].since_ms = 0;
        fill_entry_text(&g_sensor_status[i]);
    }
}

void status_service_set_sensor_status(status_sensor_id_t sensor_id, sensor_status_t sensor_status,
                                      uint64_t now_ms)
{
    sensor_status_entry_t *entry;

    if (sensor_id >= STATUS_SENSOR_COUNT)
        return;

    entry = &g_sensor_status[sensor_id];

    sensor_status_t old_status = entry->sensor_status;

    /* Only update "since_ms" when the status actually changes. */
    if (old_status == sensor_status)
        return;

    data_logger_log_status_event(now_ms, sensor_id, old_status, sensor_status, entry->detail);

    entry->sensor_status = sensor_status;
    entry->severity = severity_from_sensor_status(sensor_status);
    entry->active = (sensor_status != SENSOR_STATUS_OK);
    entry->since_ms = now_ms;
    fill_entry_text(entry);
}

bool status_service_get_sensor_status(status_sensor_id_t sensor_id, sensor_status_entry_t *out)
{
    if (!out)
        return false;

    if (sensor_id >= STATUS_SENSOR_COUNT)
        return false;

    *out = g_sensor_status[sensor_id];
    return true;
}

size_t status_service_get_active_sensor_statuses(sensor_status_entry_t *out, size_t max_entries)
{
    size_t count = 0;
    int i;

    if (!out || max_entries == 0)
        return 0;

    for (i = 0; i < STATUS_SENSOR_COUNT && count < max_entries; i++)
    {
        if (!g_sensor_status[i].active)
            continue;

        out[count++] = g_sensor_status[i];
    }

    return count;
}

size_t status_service_get_active_count(void)
{
    size_t count = 0;
    int i;

    for (i = 0; i < STATUS_SENSOR_COUNT; i++)
    {
        if (g_sensor_status[i].active)
            count++;
    }

    return count;
}

status_severity_t status_service_get_overall_severity(void)
{
    int i;
    status_severity_t highest = STATUS_SEV_INFO;

    for (i = 0; i < STATUS_SENSOR_COUNT; i++)
    {
        if (!g_sensor_status[i].active)
            continue;

        if (g_sensor_status[i].severity > highest)
            highest = g_sensor_status[i].severity;
    }

    return highest;
}

const char *status_service_get_summary(void)
{
    const sensor_status_entry_t *first = NULL;
    size_t count = 0;
    int i;

    for (i = 0; i < STATUS_SENSOR_COUNT; i++)
    {
        if (!g_sensor_status[i].active)
            continue;

        if (!first)
            first = &g_sensor_status[i];
        count++;
    }

    if (!first)
        return "OK";

    if (count == 1)
        return first->summary;

    snprintf(g_summary_buf, sizeof(g_summary_buf), "%s (+%zu)", first->summary, count - 1);

    return g_summary_buf;
}