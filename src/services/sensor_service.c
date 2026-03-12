#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#include "sensor_service.h"
#include "log.h"

#define UPDATE_INTERVAL_SECONDS 2
#define STALE_TIMEOUT_MS       10000ULL

#define BQ27441_BASE "/sys/class/power_supply/bq27441-0"
#define BQ27441_STATUS_PATH   BQ27441_BASE "/status"
#define BQ27441_CAPACITY_PATH BQ27441_BASE "/capacity"
#define BQ27441_VOLTAGE_PATH  BQ27441_BASE "/voltage_now"
#define BQ27441_CURRENT_PATH  BQ27441_BASE "/current_now"

/* TODO: these are likely variable and should be auto-detected */
#define SCD30_BASE "/sys/bus/iio/devices/iio:device2"
#define SCD30_CO2_RAW_PATH   SCD30_BASE "/in_concentration_co2_raw"
#define SCD30_TEMP_PATH      SCD30_BASE "/in_temp_input"
#define SCD30_HUMID_PATH     SCD30_BASE "/in_humidityrelative_input"

#define BMP580_BASE "/sys/bus/iio/devices/iio:device0"
#define BMP580_PRESSURE_PATH BMP580_BASE "/in_pressure_input"
#define BMP580_TEMP_PATH     BMP580_BASE "/in_temp_input"

#define SGP30_BASE "/sys/bus/iio/devices/iio:device1"
#define SGP30_ECO2_PATH SGP30_BASE "/in_concentration_co2_input"
#define SGP30_TVOC_PATH SGP30_BASE "/in_concentration_voc_input"

static pthread_t sensor_thread;
static pthread_mutex_t sensor_mutex = PTHREAD_MUTEX_INITIALIZER;
static sensor_snapshot_t snapshot;
static bool running = false;

typedef enum
{
    READ_RESULT_OK = 0,
    READ_RESULT_MISSING,
    READ_RESULT_ERROR,
} read_result_t;

static void *sensor_thread_main(void *arg);

static read_result_t read_bq27441(fuel_gauge_bq27441_t *out);
static read_result_t read_scd30(sensor_scd30_t *out);
static read_result_t read_bmp580(sensor_bmp580_t *out);
static read_result_t read_sgp30(sensor_sgp30_t *out);

static bool read_int_from_file(const char *path, int *out);
static bool read_float_from_file(const char *path, float *out);
static bool read_file_text(const char *path, char *buf, size_t buf_sz);
static bool path_exists(const char *path);

static uint64_t get_monotonic_time_ms(void);

static sensor_status_t derive_sensor_status(read_result_t rr,
                                            uint64_t now_ms,
                                            uint64_t last_update_ms);

bool sensor_service_init(void)
{
    pthread_mutex_lock(&sensor_mutex);

    memset(&snapshot, 0, sizeof(snapshot));

    snapshot.bq27441.status = SENSOR_STATUS_MISSING;
    snapshot.scd30.status = SENSOR_STATUS_MISSING;
    snapshot.bmp580.status = SENSOR_STATUS_MISSING;
    snapshot.sgp30.status = SENSOR_STATUS_MISSING;

    running = false;

    pthread_mutex_unlock(&sensor_mutex);

    return true;
}

bool sensor_service_start(void)
{
    int ret;

    pthread_mutex_lock(&sensor_mutex);

    if (running) {
        pthread_mutex_unlock(&sensor_mutex);
        return true;
    }

    running = true;

    pthread_mutex_unlock(&sensor_mutex);

    ret = pthread_create(&sensor_thread, NULL, sensor_thread_main, NULL);
    if (ret != 0) {
        pthread_mutex_lock(&sensor_mutex);
        running = false;

        snapshot.bq27441.status = SENSOR_STATUS_ERROR;
        snapshot.scd30.status = SENSOR_STATUS_ERROR;
        snapshot.bmp580.status = SENSOR_STATUS_ERROR;
        snapshot.sgp30.status = SENSOR_STATUS_ERROR;

        pthread_mutex_unlock(&sensor_mutex);
        return false;
    }

    return true;
}

void sensor_service_stop(void)
{
    pthread_mutex_lock(&sensor_mutex);
    if (!running) {
        pthread_mutex_unlock(&sensor_mutex);
        return;
    }
    running = false;
    pthread_mutex_unlock(&sensor_mutex);

    pthread_join(sensor_thread, NULL);
}

bool sensor_service_get_snapshot(sensor_snapshot_t *out)
{
    if (!out)
        return false;

    pthread_mutex_lock(&sensor_mutex);
    *out = snapshot;
    pthread_mutex_unlock(&sensor_mutex);

    return true;
}

static void *sensor_thread_main(void *arg)
{
    (void)arg;

    while (1) {
        sensor_snapshot_t next;
        sensor_snapshot_t prev;
        bool keep_running;
        uint64_t now_ms;

        pthread_mutex_lock(&sensor_mutex);
        keep_running = running;
        prev = snapshot;
        pthread_mutex_unlock(&sensor_mutex);

        if (!keep_running)
            break;

        now_ms = get_monotonic_time_ms();

        /* Start from previous snapshot so old good values are preserved if reads fail. */
        next = prev;

        {
            fuel_gauge_bq27441_t sample = prev.bq27441;
            read_result_t rr = read_bq27441(&sample);

            if (rr == READ_RESULT_OK) {
                sample.last_update_ms = now_ms;
            }

            sample.status = derive_sensor_status(rr, now_ms, sample.last_update_ms);
            next.bq27441 = sample;
        }

        {
            sensor_scd30_t sample = prev.scd30;
            read_result_t rr = read_scd30(&sample);

            if (rr == READ_RESULT_OK) {
                sample.last_update_ms = now_ms;
            }

            sample.status = derive_sensor_status(rr, now_ms, sample.last_update_ms);
            next.scd30 = sample;
        }

        {
            sensor_bmp580_t sample = prev.bmp580;
            read_result_t rr = read_bmp580(&sample);

            if (rr == READ_RESULT_OK) {
                sample.last_update_ms = now_ms;
            }

            sample.status = derive_sensor_status(rr, now_ms, sample.last_update_ms);
            next.bmp580 = sample;
        }

        {
            sensor_sgp30_t sample = prev.sgp30;
            read_result_t rr = read_sgp30(&sample);

            if (rr == READ_RESULT_OK) {
                sample.last_update_ms = now_ms;
            }

            sample.status = derive_sensor_status(rr, now_ms, sample.last_update_ms);
            next.sgp30 = sample;
        }

        pthread_mutex_lock(&sensor_mutex);
        snapshot = next;
        pthread_mutex_unlock(&sensor_mutex);

        sleep(UPDATE_INTERVAL_SECONDS);
    }

    return NULL;
}

static uint64_t get_monotonic_time_ms(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;

    return ((uint64_t)ts.tv_sec * 1000ULL) +
           ((uint64_t)ts.tv_nsec / 1000000ULL);
}

static sensor_status_t derive_sensor_status(read_result_t rr,
                                            uint64_t now_ms,
                                            uint64_t last_update_ms)
{
    switch (rr) {
    case READ_RESULT_OK:
        return SENSOR_STATUS_OK;

    case READ_RESULT_MISSING:
        return SENSOR_STATUS_MISSING;

    case READ_RESULT_ERROR:
    default:
        if (last_update_ms != 0 &&
            now_ms >= last_update_ms &&
            (now_ms - last_update_ms) > STALE_TIMEOUT_MS) {
            return SENSOR_STATUS_STALE;
        }
        return SENSOR_STATUS_ERROR;
    }
}

static read_result_t read_bq27441(fuel_gauge_bq27441_t *out)
{
    char status_buf[32];
    int capacity = 0;
    int voltage_uv = 0;
    int current_ua = 0;

    if (!out)
        return READ_RESULT_ERROR;

    if (!path_exists(BQ27441_STATUS_PATH) ||
        !path_exists(BQ27441_CAPACITY_PATH) ||
        !path_exists(BQ27441_VOLTAGE_PATH) ||
        !path_exists(BQ27441_CURRENT_PATH)) {
        return READ_RESULT_MISSING;
    }

    if (!read_file_text(BQ27441_STATUS_PATH, status_buf, sizeof(status_buf)))
        return READ_RESULT_ERROR;

    if (!read_int_from_file(BQ27441_CAPACITY_PATH, &capacity))
        return READ_RESULT_ERROR;

    if (!read_int_from_file(BQ27441_VOLTAGE_PATH, &voltage_uv))
        return READ_RESULT_ERROR;

    if (!read_int_from_file(BQ27441_CURRENT_PATH, &current_ua))
        return READ_RESULT_ERROR;

    if (strcmp(status_buf, "Charging") == 0) {
        out->battery_status = BATTERY_STATUS_CHARGING;
    } else if (strcmp(status_buf, "Discharging") == 0) {
        out->battery_status = BATTERY_STATUS_DISCHARGING;
    } else if (strcmp(status_buf, "Full") == 0) {
        out->battery_status = BATTERY_STATUS_FULL;
    } else {
        out->battery_status = BATTERY_STATUS_UNKNOWN;
    }

    if (capacity < 0)
        capacity = 0;
    else if (capacity > 100)
        capacity = 100;

    out->capacity_percent = (uint8_t)capacity;
    out->voltage_v = voltage_uv / 1000000.0f;
    out->current_ma = current_ua / 1000.0f;

    LOGD("Read BQ27441: status=%s, capacity=%u%%, voltage=%.3f V, current=%.1f mA",
         status_buf,
         out->capacity_percent,
         out->voltage_v,
         out->current_ma);

    return READ_RESULT_OK;
}

static read_result_t read_scd30(sensor_scd30_t *out)
{
    int co2_raw = 0;
    int temp_milli = 0;
    int humid_milli = 0;

    if (!out)
        return READ_RESULT_ERROR;

    if (!path_exists(SCD30_CO2_RAW_PATH) ||
        !path_exists(SCD30_TEMP_PATH) ||
        !path_exists(SCD30_HUMID_PATH)) {
        return READ_RESULT_MISSING;
    }

    if (!read_int_from_file(SCD30_CO2_RAW_PATH, &co2_raw))
        return READ_RESULT_ERROR;

    if (!read_int_from_file(SCD30_TEMP_PATH, &temp_milli))
        return READ_RESULT_ERROR;

    if (!read_int_from_file(SCD30_HUMID_PATH, &humid_milli))
        return READ_RESULT_ERROR;

    out->co2_ppm = co2_raw / 100.0f;
    out->temperature_c = temp_milli / 1000.0f;
    out->humidity_rh = humid_milli / 1000.0f;

    LOGD("Read SCD30: CO2=%.1f ppm, Temp=%.1f C, Humidity=%.1f %%",
         out->co2_ppm,
         out->temperature_c,
         out->humidity_rh);

    return READ_RESULT_OK;
}

static read_result_t read_bmp580(sensor_bmp580_t *out)
{
    float pressure_kpa = 0.0f;
    int temp_milli = 0;

    if (!out)
        return READ_RESULT_ERROR;

    if (!path_exists(BMP580_PRESSURE_PATH) ||
        !path_exists(BMP580_TEMP_PATH)) {
        return READ_RESULT_MISSING;
    }

    /*
     * Linux IIO conventions here are:
     *   in_pressure_input -> kPa
     *   in_temp_input     -> millidegrees C
     */
    if (!read_float_from_file(BMP580_PRESSURE_PATH, &pressure_kpa))
        return READ_RESULT_ERROR;

    if (!read_int_from_file(BMP580_TEMP_PATH, &temp_milli))
        return READ_RESULT_ERROR;

    out->pressure_hpa = pressure_kpa * 10.0f;
    out->temperature_c = temp_milli / 1000.0f;

    LOGD("Read BMP580: Pressure=%.2f hPa, Temp=%.2f C",
         out->pressure_hpa,
         out->temperature_c);

    return READ_RESULT_OK;
}

static read_result_t read_sgp30(sensor_sgp30_t *out)
{
    float eco2_frac = 0.0f;
    float tvoc_frac = 0.0f;

    if (!out)
        return READ_RESULT_ERROR;

    if (!path_exists(SGP30_ECO2_PATH) ||
        !path_exists(SGP30_TVOC_PATH)) {
        return READ_RESULT_MISSING;
    }

    if (!read_float_from_file(SGP30_ECO2_PATH, &eco2_frac))
        return READ_RESULT_ERROR;

    if (!read_float_from_file(SGP30_TVOC_PATH, &tvoc_frac))
        return READ_RESULT_ERROR;

    out->eco2_ppm = eco2_frac * 1000000.0f;
    out->tvoc_ppb = tvoc_frac * 1000000000.0f;

    LOGD("Read SGP30: eCO2=%.0f ppm, TVOC=%.0f ppb",
         out->eco2_ppm,
         out->tvoc_ppb);

    return READ_RESULT_OK;
}

static bool path_exists(const char *path)
{
    return (path && access(path, F_OK) == 0);
}

static bool read_int_from_file(const char *path, int *out)
{
    char buf[64];
    char *end = NULL;
    long v;

    if (!out)
        return false;

    if (!read_file_text(path, buf, sizeof(buf)))
        return false;

    errno = 0;
    v = strtol(buf, &end, 10);
    if (errno != 0 || end == buf)
        return false;

    *out = (int)v;
    return true;
}

static bool read_float_from_file(const char *path, float *out)
{
    char buf[64];
    char *end = NULL;
    float v;

    if (!out)
        return false;

    if (!read_file_text(path, buf, sizeof(buf)))
        return false;

    errno = 0;
    v = strtof(buf, &end);
    if (errno != 0 || end == buf)
        return false;

    *out = v;
    return true;
}

static bool read_file_text(const char *path, char *buf, size_t buf_sz)
{
    int fd;
    ssize_t n;

    if (!path || !buf || buf_sz == 0)
        return false;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return false;

    n = read(fd, buf, buf_sz - 1);
    close(fd);

    if (n <= 0)
        return false;

    while (n > 0 &&
           (buf[n - 1] == '\n' ||
            buf[n - 1] == '\r' ||
            buf[n - 1] == ' '  ||
            buf[n - 1] == '\t')) {
        n--;
    }

    buf[n] = '\0';
    return true;
}