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

// TODO: this is likely variable and should be auto-detected
#define SCD30_BASE "/sys/bus/iio/devices/iio:device2"

#define SCD30_CO2_RAW_PATH   SCD30_BASE "/in_concentration_co2_raw"
#define SCD30_TEMP_PATH      SCD30_BASE "/in_temp_input"
#define SCD30_HUMID_PATH     SCD30_BASE "/in_humidityrelative_input"
#define SCD30_CO2_SCALE_PATH SCD30_BASE "/in_concentration_co2_scale"

static pthread_t sensor_thread;
static pthread_mutex_t sensor_mutex = PTHREAD_MUTEX_INITIALIZER;
static sensor_snapshot_t snapshot;
static bool running = false;

static void *sensor_thread_main(void *arg);
static bool read_scd30(sensor_scd30_t *out);
static bool read_int_from_file(const char *path, int *out);
static bool read_float_from_file(const char *path, float *out);
static bool read_file_text(const char *path, char *buf, size_t buf_sz);

bool sensor_service_init(void)
{
    pthread_mutex_lock(&sensor_mutex);

    memset(&snapshot, 0, sizeof(snapshot));
    snapshot.scd30.status = SENSOR_STATUS_UNINITIALIZED;

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

        if (next.scd30.status != SENSOR_STATUS_ERROR) {
            pthread_mutex_lock(&sensor_mutex);
            snapshot = next;
            pthread_mutex_unlock(&sensor_mutex);
        }

        sleep(2);
    }

    return NULL;
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

    buf[n] = '\0';
    return true;
}