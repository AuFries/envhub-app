#include "data_logger.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DATA_LOGGER_DIR "/var/log/envhub"
#define DATA_LOGGER_PATH_BQ27441 DATA_LOGGER_DIR "/bq27441.csv"
#define DATA_LOGGER_PATH_SCD30 DATA_LOGGER_DIR "/scd30.csv"
#define DATA_LOGGER_PATH_BMP580 DATA_LOGGER_DIR "/bmp580.csv"
#define DATA_LOGGER_PATH_SGP30 DATA_LOGGER_DIR "/sgp30.csv"
#define DATA_LOGGER_PATH_STATUS DATA_LOGGER_DIR "/status_events.csv"

typedef struct
{
    FILE *bq27441_fp;
    FILE *scd30_fp;
    FILE *bmp580_fp;
    FILE *sgp30_fp;
    FILE *status_fp;
    bool initialized;
} data_logger_ctx_t;

static data_logger_ctx_t g_logger;

static const char *sensor_status_to_string(sensor_status_t status)
{
    switch (status)
    {
        case SENSOR_STATUS_MISSING:
            return "MISSING";
        case SENSOR_STATUS_OK:
            return "OK";
        case SENSOR_STATUS_STALE:
            return "STALE";
        case SENSOR_STATUS_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

static const char *battery_status_to_string(battery_status_t status)
{
    switch (status)
    {
        case BATTERY_STATUS_UNKNOWN:
            return "UNKNOWN";
        case BATTERY_STATUS_CHARGING:
            return "CHARGING";
        case BATTERY_STATUS_DISCHARGING:
            return "DISCHARGING";
        case BATTERY_STATUS_FULL:
            return "FULL";
        default:
            return "UNKNOWN";
    }
}

static void format_utc_timestamp(char *buf, size_t buf_sz)
{
    time_t now;
    struct tm tm_utc;

    if (!buf || buf_sz == 0)
        return;

    now = time(NULL);
    if (gmtime_r(&now, &tm_utc) == NULL)
    {
        snprintf(buf, buf_sz, "1970-01-01T00:00:00Z");
        return;
    }

    strftime(buf, buf_sz, "%Y-%m-%dT%H:%M:%SZ", &tm_utc);
}

static bool ensure_dir_exists(const char *path)
{
    struct stat st;

    if (!path)
        return false;

    if (stat(path, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
            return true;
        return false;
    }

    if (mkdir(path, 0755) == 0)
        return true;

    if (errno == EEXIST)
        return true;

    return false;
}

static bool file_is_empty(FILE *fp)
{
    long pos;

    if (!fp)
        return true;

    pos = ftell(fp);
    if (pos < 0)
        return true;

    return (pos == 0);
}

static FILE *open_csv_file(const char *path, const char *header)
{
    FILE *fp;

    if (!path || !header)
        return NULL;

    fp = fopen(path, "a+");
    if (!fp)
        return NULL;

    if (setvbuf(fp, NULL, _IOLBF, 0) != 0)
    {
        /* Not fatal. Continue. */
    }

    if (file_is_empty(fp))
    {
        if (fprintf(fp, "%s\n", header) < 0)
        {
            fclose(fp);
            return NULL;
        }

        if (fflush(fp) != 0)
        {
            fclose(fp);
            return NULL;
        }
    }

    return fp;
}

static bool flush_file(FILE *fp)
{
    if (!fp)
        return false;

    if (fflush(fp) != 0)
        return false;

    return true;
}

bool data_logger_init(void)
{
    memset(&g_logger, 0, sizeof(g_logger));

    if (!ensure_dir_exists(DATA_LOGGER_DIR))
        return false;

    g_logger.bq27441_fp = open_csv_file(
        DATA_LOGGER_PATH_BQ27441, "wall_time_utc,monotonic_ms,last_update_ms,capacity_percent,"
                                  "voltage_v,current_ma,battery_status,sensor_status");
    if (!g_logger.bq27441_fp)
        goto fail;

    g_logger.scd30_fp = open_csv_file(
        DATA_LOGGER_PATH_SCD30,
        "wall_time_utc,monotonic_ms,last_update_ms,co2_ppm,temperature_c,humidity_rh,status");
    if (!g_logger.scd30_fp)
        goto fail;

    g_logger.bmp580_fp = open_csv_file(
        DATA_LOGGER_PATH_BMP580,
        "wall_time_utc,monotonic_ms,last_update_ms,temperature_c,pressure_hpa,status");
    if (!g_logger.bmp580_fp)
        goto fail;

    g_logger.sgp30_fp =
        open_csv_file(DATA_LOGGER_PATH_SGP30,
                      "wall_time_utc,monotonic_ms,last_update_ms,tvoc_ppb,eco2_ppm,status");
    if (!g_logger.sgp30_fp)
        goto fail;

    g_logger.status_fp = open_csv_file(
        DATA_LOGGER_PATH_STATUS, "wall_time_utc,monotonic_ms,sensor,old_status,new_status,message");
    if (!g_logger.status_fp)
        goto fail;

    g_logger.initialized = true;
    return true;

fail:
    data_logger_shutdown();
    return false;
}

void data_logger_shutdown(void)
{
    if (g_logger.bq27441_fp)
    {
        fflush(g_logger.bq27441_fp);
        fclose(g_logger.bq27441_fp);
        g_logger.bq27441_fp = NULL;
    }

    if (g_logger.scd30_fp)
    {
        fflush(g_logger.scd30_fp);
        fclose(g_logger.scd30_fp);
        g_logger.scd30_fp = NULL;
    }

    if (g_logger.bmp580_fp)
    {
        fflush(g_logger.bmp580_fp);
        fclose(g_logger.bmp580_fp);
        g_logger.bmp580_fp = NULL;
    }

    if (g_logger.sgp30_fp)
    {
        fflush(g_logger.sgp30_fp);
        fclose(g_logger.sgp30_fp);
        g_logger.sgp30_fp = NULL;
    }

    if (g_logger.status_fp)
    {
        fflush(g_logger.status_fp);
        fclose(g_logger.status_fp);
        g_logger.status_fp = NULL;
    }

    g_logger.initialized = false;
}

bool data_logger_log_bq27441(uint64_t monotonic_ms, const fuel_gauge_bq27441_t *sample)
{
    char ts[32];

    if (!g_logger.initialized || !g_logger.bq27441_fp || !sample)
        return false;

    format_utc_timestamp(ts, sizeof(ts));

    if (fprintf(g_logger.bq27441_fp, "%s,%llu,%llu,%u,%.3f,%.3f,%s,%s\n", ts,
                (unsigned long long)monotonic_ms, (unsigned long long)sample->last_update_ms,
                sample->capacity_percent, sample->voltage_v, sample->current_ma,
                battery_status_to_string(sample->battery_status),
                sensor_status_to_string(sample->status)) < 0)
    {
        return false;
    }

    return flush_file(g_logger.bq27441_fp);
}

bool data_logger_log_scd30(uint64_t monotonic_ms, const sensor_scd30_t *sample)
{
    char ts[32];

    if (!g_logger.initialized || !g_logger.scd30_fp || !sample)
        return false;

    format_utc_timestamp(ts, sizeof(ts));

    if (fprintf(g_logger.scd30_fp, "%s,%llu,%llu,%.3f,%.3f,%.3f,%s\n", ts,
                (unsigned long long)monotonic_ms, (unsigned long long)sample->last_update_ms,
                sample->co2_ppm, sample->temperature_c, sample->humidity_rh,
                sensor_status_to_string(sample->status)) < 0)
    {
        return false;
    }

    return flush_file(g_logger.scd30_fp);
}

bool data_logger_log_bmp580(uint64_t monotonic_ms, const sensor_bmp580_t *sample)
{
    char ts[32];

    if (!g_logger.initialized || !g_logger.bmp580_fp || !sample)
        return false;

    format_utc_timestamp(ts, sizeof(ts));

    if (fprintf(g_logger.bmp580_fp, "%s,%llu,%llu,%.3f,%.3f,%s\n", ts,
                (unsigned long long)monotonic_ms, (unsigned long long)sample->last_update_ms,
                sample->temperature_c, sample->pressure_hpa,
                sensor_status_to_string(sample->status)) < 0)
    {
        return false;
    }

    return flush_file(g_logger.bmp580_fp);
}

bool data_logger_log_sgp30(uint64_t monotonic_ms, const sensor_sgp30_t *sample)
{
    char ts[32];

    if (!g_logger.initialized || !g_logger.sgp30_fp || !sample)
        return false;

    format_utc_timestamp(ts, sizeof(ts));

    if (fprintf(g_logger.sgp30_fp, "%s,%llu,%llu,%.3f,%.3f,%s\n", ts,
                (unsigned long long)monotonic_ms, (unsigned long long)sample->last_update_ms,
                sample->tvoc_ppb, sample->eco2_ppm, sensor_status_to_string(sample->status)) < 0)
    {
        return false;
    }

    return flush_file(g_logger.sgp30_fp);
}

bool data_logger_log_snapshot(uint64_t monotonic_ms, const sensor_snapshot_t *snapshot)
{
    bool ok = true;

    if (!snapshot)
        return false;

    ok &= data_logger_log_bq27441(monotonic_ms, &snapshot->bq27441);
    ok &= data_logger_log_scd30(monotonic_ms, &snapshot->scd30);
    ok &= data_logger_log_bmp580(monotonic_ms, &snapshot->bmp580);
    ok &= data_logger_log_sgp30(monotonic_ms, &snapshot->sgp30);

    return ok;
}

bool data_logger_log_status_event(uint64_t monotonic_ms, status_sensor_id_t sensor_id,
                                  sensor_status_t old_status, sensor_status_t new_status,
                                  const char *message)
{
    char ts[32];
    const char *sensor_name;

    if (!g_logger.initialized || !g_logger.status_fp)
        return false;

    if (!message)
        message = "";

    sensor_name = status_service_sensor_name(sensor_id);
    format_utc_timestamp(ts, sizeof(ts));

    if (fprintf(g_logger.status_fp, "%s,%llu,%s,%s,%s,%s\n", ts, (unsigned long long)monotonic_ms,
                sensor_name, sensor_status_to_string(old_status),
                sensor_status_to_string(new_status), message) < 0)
    {
        return false;
    }

    return flush_file(g_logger.status_fp);
}