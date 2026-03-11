#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

#include "sensor_service.h"
#include "log.h"

#define UPDATE_INTERVAL_SECONDS 2

#define BQ27441_BASE "/sys/class/power_supply/bq27441-0"
#define BQ27441_STATUS_PATH   BQ27441_BASE "/status"
#define BQ27441_CAPACITY_PATH BQ27441_BASE "/capacity"
#define BQ27441_VOLTAGE_PATH  BQ27441_BASE "/voltage_now"
#define BQ27441_CURRENT_PATH  BQ27441_BASE "/current_now"

// TODO: this is likely variable and should be auto-detected
#define SCD30_BASE "/sys/bus/iio/devices/iio:device2"

#define SCD30_CO2_RAW_PATH   SCD30_BASE "/in_concentration_co2_raw"
#define SCD30_TEMP_PATH      SCD30_BASE "/in_temp_input"
#define SCD30_HUMID_PATH     SCD30_BASE "/in_humidityrelative_input"
#define SCD30_CO2_SCALE_PATH SCD30_BASE "/in_concentration_co2_scale"

// TODO: this is likely variable and should be auto-detected
#define BMP580_BASE "/sys/bus/iio/devices/iio:device0"

#define BMP580_PRESSURE_PATH BMP580_BASE "/in_pressure_input"
#define BMP580_TEMP_PATH     BMP580_BASE "/in_temp_input"

// TODO: this is likely variable and should be auto-detected
#define SGP30_BASE "/sys/bus/iio/devices/iio:device1"

#define SGP30_ECO2_PATH SGP30_BASE "/in_concentration_co2_input"
#define SGP30_TVOC_PATH SGP30_BASE "/in_concentration_voc_input"

static pthread_t sensor_thread;
static pthread_mutex_t sensor_mutex = PTHREAD_MUTEX_INITIALIZER;
static sensor_snapshot_t snapshot;
static bool running = false;

static void *sensor_thread_main(void *arg);
static bool read_bq27441(fuel_gauge_bq27441_t *out);
static bool read_scd30(sensor_scd30_t *out);
static bool read_bmp580(sensor_bmp580_t *out);
static bool read_sgp30(sensor_sgp30_t *out);
static bool read_int_from_file(const char *path, int *out);
static bool read_float_from_file(const char *path, float *out);
static bool read_file_text(const char *path, char *buf, size_t buf_sz);

bool sensor_service_init(void)
{
    pthread_mutex_lock(&sensor_mutex);

    memset(&snapshot, 0, sizeof(snapshot));
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
        sensor_snapshot_t next = {0};
        bool keep_running;

        pthread_mutex_lock(&sensor_mutex);
        keep_running = running;
        pthread_mutex_unlock(&sensor_mutex);

        if (!keep_running)
            break;

        next.scd30.status = read_scd30(&next.scd30)
            ? SENSOR_STATUS_OK
            : SENSOR_STATUS_ERROR;

        next.bmp580.status = read_bmp580(&next.bmp580)
            ? SENSOR_STATUS_OK
            : SENSOR_STATUS_ERROR;

        next.sgp30.status = read_sgp30(&next.sgp30)
            ? SENSOR_STATUS_OK
            : SENSOR_STATUS_ERROR;

        (void)read_bq27441(&next.bq27441);

        pthread_mutex_lock(&sensor_mutex);
        snapshot = next;
        pthread_mutex_unlock(&sensor_mutex);

        sleep(UPDATE_INTERVAL_SECONDS);
    }

    return NULL;
}

static bool read_bq27441(fuel_gauge_bq27441_t *out)
{
    char status_buf[32];
    int capacity = 0;
    int voltage_uv = 0;
    int current_ua = 0;

    if (!out)
        return false;

    memset(out, 0, sizeof(*out));
    out->status = BATTERY_STATUS_UNKNOWN;

    if (!read_file_text(BQ27441_STATUS_PATH, status_buf, sizeof(status_buf)))
        return false;

    if (!read_int_from_file(BQ27441_CAPACITY_PATH, &capacity))
        return false;

    if (!read_int_from_file(BQ27441_VOLTAGE_PATH, &voltage_uv))
        return false;

    if (!read_int_from_file(BQ27441_CURRENT_PATH, &current_ua))
        return false;

    if (strcmp(status_buf, "Charging") == 0) {
        out->status = BATTERY_STATUS_CHARGING;
    } else if (strcmp(status_buf, "Discharging") == 0) {
        out->status = BATTERY_STATUS_DISCHARGING;
    } else if (strcmp(status_buf, "Full") == 0) {
        out->status = BATTERY_STATUS_FULL;
    } else {
        out->status = BATTERY_STATUS_UNKNOWN;
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

    return true;
}


static bool read_scd30(sensor_scd30_t *out)
{
    int co2_raw = 0;
    int temp_milli = 0;
    int humid_milli = 0;

    if (!out)
        return false;

    if (!read_int_from_file(SCD30_CO2_RAW_PATH, &co2_raw)) 
        return false;

    if (!read_int_from_file(SCD30_TEMP_PATH, &temp_milli))
        return false;

    if (!read_int_from_file(SCD30_HUMID_PATH, &humid_milli))
        return false;

    out->co2_ppm = co2_raw / 100.0f;
    out->temperature_c = temp_milli / 1000.0f;
    out->humidity_rh = humid_milli / 1000.0f;

    LOGD("Read SCD30: CO2=%.1f ppm, Temp=%.1f C, Humidity=%.1f %%", out->co2_ppm, out->temperature_c, out->humidity_rh);
    return true;
}

static bool read_bmp580(sensor_bmp580_t *out)
{
    float pressure_kpa = 0.0f;
    int temp_milli = 0;

    if (!out)
        return false;

    memset(out, 0, sizeof(*out));

    /*
     * Linux IIO conventions here are:
     *   in_pressure_input -> kPa
     *   in_temp_input     -> millidegrees C
     */
    if (!read_float_from_file(BMP580_PRESSURE_PATH, &pressure_kpa))
        return false;

    if (!read_int_from_file(BMP580_TEMP_PATH, &temp_milli))
        return false;

    out->pressure_hpa = pressure_kpa * 10.0f;
    out->temperature_c = temp_milli / 1000.0f;

    LOGD("Read BMP580: Pressure=%.2f hPa, Temp=%.2f C",
         out->pressure_hpa,
         out->temperature_c);

    return true;
}

static bool read_sgp30(sensor_sgp30_t *out)
{
    float eco2_frac = 0.0f;
    float tvoc_frac = 0.0f;

    if (!out)
        return false;

    memset(out, 0, sizeof(*out));

    if (!read_float_from_file(SGP30_ECO2_PATH, &eco2_frac))
        return false;

    if (!read_float_from_file(SGP30_TVOC_PATH, &tvoc_frac))
        return false;

    out->eco2_ppm = eco2_frac * 1000000.0f;
    out->tvoc_ppb = tvoc_frac * 1000000000.0f;

    LOGD("Read SGP30: eCO2=%.0f ppm, TVOC=%.0f ppb",
         out->eco2_ppm,
         out->tvoc_ppb);

    return true;
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

    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r' ||
                     buf[n - 1] == ' '  || buf[n - 1] == '\t')) {
        n--;
    }

    buf[n] = '\0';
    return true;
}