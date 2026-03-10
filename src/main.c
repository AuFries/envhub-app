#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include <lvgl/lvgl.h>
#include <lvgl/src/drivers/display/drm/lv_linux_drm.h>

#include "services/sensor_service.h"
#include "ui/ui_envhub.h"

static volatile bool g_run = true;

static void *tick_thread(void *arg);
static int64_t parse_i64(const char *s, int64_t def);
static int lvgl_drm_init(int argc, char **argv);

void ui_envhub_set_scd30(float co2_ppm, float temp_c, float humidity_rh, ui_sensor_state_t state)
{
    char buf[32];

    if (state == UI_SENSOR_STATE_OK) {
        if (co2_label) {
            snprintf(buf, sizeof(buf), "%.0f", co2_ppm);
            lv_label_set_text(co2_label, buf);
        }

        if (temp_label) {
            snprintf(buf, sizeof(buf), "%.1f", temp_c);
            lv_label_set_text(temp_label, buf);
        }

        if (humidity_label) {
            snprintf(buf, sizeof(buf), "%.1f", humidity_rh);
            lv_label_set_text(humidity_label, buf);
        }
    } else {
        if (co2_label) lv_label_set_text(co2_label, "--");
        if (temp_label) lv_label_set_text(temp_label, "--");
        if (humidity_label) lv_label_set_text(humidity_label, "--");
    }
}

static void app_sensor_timer_cb(lv_timer_t *t)
{
    (void)t;

    sensor_snapshot_t snap;
    if (!sensor_service_get_snapshot(&snap))
        return;

    ui_envhub_set_scd30(
        snap.scd30.co2_ppm,
        snap.scd30.temperature_c,
        snap.scd30.humidity_rh,
    );
}

int main(int argc, char **argv)
{
    if(lvgl_drm_init(argc, argv) != 0) {
        return 1;
    }

    sensor_service_init();
    sensor_service_start();

    ui_envhub_init();

    app_sensor_timer = lv_timer_create(app_sensor_timer_cb, 1000, NULL);

    while(g_run) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

static void *tick_thread(void *arg)
{
    (void)arg;
    while(g_run) {
        lv_tick_inc(1);
        usleep(1000);
    }
    return NULL;
}

static int64_t parse_i64(const char *s, int64_t def)
{
    if(!s || !*s) return def;
    char *end = NULL;
    long long v = strtoll(s, &end, 0);
    if(end == s) return def;
    return (int64_t)v;
}

static int lvgl_drm_init(int argc, char **argv)
{
    const char *card = "/dev/dri/card0";
    int64_t connector_id = -1;

    if(argc >= 2) card = argv[1];
    if(argc >= 3) connector_id = parse_i64(argv[2], -1);

    lv_init();

    lv_display_t *disp = lv_linux_drm_create();
    if(!disp) {
        fprintf(stderr, "ERROR: lv_linux_drm_create() returned NULL\n");
        return -1;
    }

    lv_result_t r = lv_linux_drm_set_file(disp, card, connector_id);
    if(r != LV_RESULT_OK) {
        fprintf(stderr, "ERROR: lv_linux_drm_set_file failed\n");
        return -1;
    }

    lv_display_set_default(disp);

    pthread_t th;
    if(pthread_create(&th, NULL, tick_thread, NULL) != 0) {
        fprintf(stderr, "ERROR: pthread_create failed: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}