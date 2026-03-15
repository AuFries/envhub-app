#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <syslog.h>
#include <time.h>

#include <lvgl/lvgl.h>
#include <lvgl/src/drivers/display/drm/lv_linux_drm.h>

#include "sensors.h"
#include "power_ctrl.h"
#include "status.h"
#include "data_logger.h"
#include "ui/ui_envhub.h"
#include "log.h"
#include "touch_input.h"

static volatile bool g_run = true;
static volatile bool g_shutdown_in_progress = false;
static uint32_t g_shutdown_popup_started_ms = 0;

static void *tick_thread(void *arg);
static int64_t parse_i64(const char *s, int64_t def);
static int lvgl_drm_init(int argc, char **argv);
static uint64_t get_monotonic_time_ms(void);
static void lvgl_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data);

static void app_sensor_timer_cb(lv_timer_t *t)
{
    (void)t;

    if (g_shutdown_in_progress)
    {
        return;
    }

    sensor_snapshot_t snap;
    if (!sensor_service_get_snapshot(&snap))
        return;

    ui_scd30_data_t ui_scd30 = {
        .co2_ppm = snap.scd30.co2_ppm,
        .temperature_c = snap.scd30.temperature_c,
        .humidity_rh = snap.scd30.humidity_rh,
    };

    ui_bmp580_data_t ui_bmp580 = {
        .temperature_c = snap.bmp580.temperature_c,
        .pressure_hpa = snap.bmp580.pressure_hpa,
    };

    ui_sgp30_data_t ui_sgp30 = {
        .tvoc_ppb = snap.sgp30.tvoc_ppb,
        .eco2_ppm = snap.sgp30.eco2_ppm,
    };

    ui_bq27441_data_t ui_bq27441 = {
        .capacity_percent = snap.bq27441.capacity_percent,
        .voltage_v = snap.bq27441.voltage_v,
        .current_ma = snap.bq27441.current_ma,
    };

    if (snap.scd30.status == SENSOR_STATUS_OK)
    {
        ui_envhub_set_scd30(&ui_scd30);
    }
    if (snap.bmp580.status == SENSOR_STATUS_OK)
    {
        ui_envhub_set_bmp580(&ui_bmp580);
    }
    if (snap.sgp30.status == SENSOR_STATUS_OK)
    {
        ui_envhub_set_sgp30(&ui_sgp30);
    }

    ui_envhub_set_bq27441(&ui_bq27441);

    uint64_t now_ms = get_monotonic_time_ms();

    data_logger_log_snapshot(now_ms, &snap);

    status_service_set_sensor_status(STATUS_SENSOR_BQ27441, snap.bq27441.status, now_ms);
    status_service_set_sensor_status(STATUS_SENSOR_SCD30, snap.scd30.status, now_ms);
    status_service_set_sensor_status(STATUS_SENSOR_BMP580, snap.bmp580.status, now_ms);
    status_service_set_sensor_status(STATUS_SENSOR_SGP30, snap.sgp30.status, now_ms);

    ui_envhub_set_status_summary(status_service_get_overall_severity(),
                                 status_service_get_summary());
}

int main(int argc, char **argv)
{
    if (lvgl_drm_init(argc, argv) != 0)
    {
        return 1;
    }

    openlog("envhub-app", LOG_PID | LOG_CONS, LOG_DAEMON);

    if (!power_service_init())
    {
        syslog(LOG_ERR, "failed to init power service");
        closelog();
        return 1;
    }

    if (!power_service_start())
    {
        syslog(LOG_ERR, "failed to start power service");
        power_service_deinit();
        closelog();
        return 1;
    }

    if (!data_logger_init())
    {
        syslog(LOG_ERR, "failed to initialize data logger");
    }

    sensor_service_init();
    sensor_service_start();

    ui_envhub_init();
    lv_timer_create(app_sensor_timer_cb, 1000, NULL);

    if (!touch_input_init("/dev/input/event2", 320, 240))
    {
        LOGW("touch input init failed");
    }
    else
    {
        lv_indev_t *touch_indev = lv_indev_create();
        lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(touch_indev, lvgl_touch_read_cb);
    }

    while (g_run)
    {
        lv_timer_handler();

        if (!g_shutdown_in_progress && power_service_shutdown_requested())
        {
            LOGI("shutdown requested from power button");

            g_shutdown_in_progress = true;
            ui_envhub_show_shutdown_popup();
            g_shutdown_popup_started_ms = lv_tick_get();
        }

        if (g_shutdown_in_progress)
        {
            if (lv_tick_elaps(g_shutdown_popup_started_ms) >= 750)
            {
                g_run = false;
            }
        }

        usleep(5000);
    }

    syslog(LOG_INFO, "stopping services for shutdown");

    power_service_stop();
    sensor_service_stop();
    touch_input_deinit();

    sync();
    usleep(200000);

    if (!power_service_power_cut())
    {
        syslog(LOG_ERR, "failed to assert OFF gpio");
        power_service_deinit();
        closelog();
        return 1;
    }

    /*
     * Stay alive so the GPIO line remains asserted until the MK2 removes power.
     */
    syslog(LOG_INFO, "waiting for hardware power cut");
    for (;;)
    {
        pause();
    }
}

static void *tick_thread(void *arg)
{
    (void)arg;
    while (g_run)
    {
        lv_tick_inc(1);
        usleep(1000);
    }
    return NULL;
}

static int64_t parse_i64(const char *s, int64_t def)
{
    if (!s || !*s)
        return def;
    char *end = NULL;
    long long v = strtoll(s, &end, 0);
    if (end == s)
        return def;
    return (int64_t)v;
}

static int lvgl_drm_init(int argc, char **argv)
{
    const char *card = "/dev/dri/card0";
    int64_t connector_id = -1;

    if (argc >= 2)
        card = argv[1];
    if (argc >= 3)
        connector_id = parse_i64(argv[2], -1);

    lv_init();

    lv_display_t *disp = lv_linux_drm_create();
    if (!disp)
    {
        fprintf(stderr, "ERROR: lv_linux_drm_create() returned NULL\n");
        return -1;
    }

    lv_result_t r = lv_linux_drm_set_file(disp, card, connector_id);
    if (r != LV_RESULT_OK)
    {
        fprintf(stderr, "ERROR: lv_linux_drm_set_file failed\n");
        return -1;
    }

    lv_display_set_default(disp);

    pthread_t th;
    if (pthread_create(&th, NULL, tick_thread, NULL) != 0)
    {
        fprintf(stderr, "ERROR: pthread_create failed: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}

static uint64_t get_monotonic_time_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((uint64_t)ts.tv_sec * 1000ULL) + ((uint64_t)ts.tv_nsec / 1000000ULL);
}

static void lvgl_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;

    bool pressed = false;
    int x = 0;
    int y = 0;

    if (!touch_input_get_state(&pressed, &x, &y))
    {
        data->state = LV_INDEV_STATE_RELEASED;
        data->point.x = 0;
        data->point.y = 0;
        return;
    }

    // TODO: REMOVE THIS LOGGING
    if (pressed)
    {
        LOGD("touch x=%d y=%d", x, y);
    }

    data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->point.x = x;
    data->point.y = y;
}