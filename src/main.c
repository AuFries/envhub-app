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

#include <lvgl/lvgl.h>
#include <lvgl/src/drivers/display/drm/lv_linux_drm.h>

#include "services/sensor_service.h"
#include "services/power_service.h"
#include "ui/ui_envhub.h"
#include "log.h"

static volatile bool g_run = true;

static void *tick_thread(void *arg);
static int64_t parse_i64(const char *s, int64_t def);
static int lvgl_drm_init(int argc, char **argv);

static void app_sensor_timer_cb(lv_timer_t *t)
{
    (void)t;

    sensor_snapshot_t snap;
    if (!sensor_service_get_snapshot(&snap))
        return;

    if (snap.scd30.status == SENSOR_STATUS_OK) {
        ui_envhub_set_scd30(snap.scd30.co2_ppm, snap.scd30.temperature_c, snap.scd30.humidity_rh);
    }
    if (snap.bmp580.status == SENSOR_STATUS_OK) {
        ui_envhub_set_bmp580(snap.bmp580.pressure_hpa, snap.bmp580.temperature_c);
    }
    if (snap.sgp30.status == SENSOR_STATUS_OK) {
        ui_envhub_set_sgp30(snap.sgp30.eco2_ppm, snap.sgp30.tvoc_ppb);
    }
    ui_envhub_set_bq27441(snap.bq27441.capacity_percent, snap.bq27441.voltage_v, snap.bq27441.current_ma);
}

int main(int argc, char **argv)
{
    if (lvgl_drm_init(argc, argv) != 0) {
        return 1;
    }

    openlog("envhub-app", LOG_PID | LOG_CONS, LOG_DAEMON);

    if (!power_service_init()) {
        syslog(LOG_ERR, "failed to init power service");
        closelog();
        return 1;
    }

    if (!power_service_start()) {
        syslog(LOG_ERR, "failed to start power service");
        power_service_deinit();
        closelog();
        return 1;
    }

    sensor_service_init();
    sensor_service_start();

    ui_envhub_init();
    lv_timer_create(app_sensor_timer_cb, 1000, NULL);

    while (g_run) {
        lv_timer_handler();

        if (power_service_shutdown_requested()) {
            syslog(LOG_INFO, "shutdown requested from power button");
            g_run = false;
        }

        usleep(5000);
    }

    power_service_stop();
    sensor_service_stop();

    sync();
    usleep(200000);

    if (!power_service_power_cut()) {
        syslog(LOG_ERR, "failed to assert OFF gpio");
        power_service_deinit();
        closelog();
        return 1;
    }

    /*
     * Stay alive so the GPIO line remains asserted until the MK2 removes power.
     */
    syslog(LOG_INFO, "waiting for hardware power cut");
    for (;;) {
        pause();
    }
}

static void *tick_thread(void *arg)
{
    (void)arg;
    while (g_run) {
        lv_tick_inc(1);
        usleep(1000);
    }
    return NULL;
}

static int64_t parse_i64(const char *s, int64_t def)
{
    if (!s || !*s) return def;
    char *end = NULL;
    long long v = strtoll(s, &end, 0);
    if (end == s) return def;
    return (int64_t)v;
}

static int lvgl_drm_init(int argc, char **argv)
{
    const char *card = "/dev/dri/card0";
    int64_t connector_id = -1;

    if (argc >= 2) card = argv[1];
    if (argc >= 3) connector_id = parse_i64(argv[2], -1);

    lv_init();

    lv_display_t *disp = lv_linux_drm_create();
    if (!disp) {
        fprintf(stderr, "ERROR: lv_linux_drm_create() returned NULL\n");
        return -1;
    }

    lv_result_t r = lv_linux_drm_set_file(disp, card, connector_id);
    if (r != LV_RESULT_OK) {
        fprintf(stderr, "ERROR: lv_linux_drm_set_file failed\n");
        return -1;
    }

    lv_display_set_default(disp);

    pthread_t th;
    if (pthread_create(&th, NULL, tick_thread, NULL) != 0) {
        fprintf(stderr, "ERROR: pthread_create failed: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}