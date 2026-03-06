#include <lvgl/lvgl.h>
#include <lvgl/src/drivers/display/drm/lv_linux_drm.h>
#include "services/time_service.h"
#include "ui/ui_envhub.h"

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

static void lvgl_drm_init(void)
{
    const char *card = "/dev/dri/card0";
    int64_t connector_id = -1; /* -1 = auto-select a connected connector */

    if(argc >= 2) card = argv[1];
    if(argc >= 3) connector_id = parse_i64(argv[2], -1);

    printf("[envhub-app] starting\n");
    printf("[envhub-app] card=%s connector_id=%" PRId64 "\n", card, connector_id);
    printf("[envhub-app] LV_RESULT_OK=%d LV_RESULT_INVALID=%d\n",
            (int)LV_RESULT_OK, (int)LV_RESULT_INVALID);

    lv_init();
    printf("[envhub-app] lv_init() ok\n");

    lv_display_t *disp = lv_linux_drm_create();
    if(!disp) {
        fprintf(stderr, "[envhub-app] ERROR: lv_linux_drm_create() returned NULL\n");
        return 1;
    }
    printf("[envhub-app] lv_linux_drm_create() ok: disp=%p\n", (void*)disp);

    lv_result_t r = lv_linux_drm_set_file(disp, card, connector_id);
    printf("[envhub-app] lv_linux_drm_set_file(%s, %" PRId64 ") => %d\n",
           card, connector_id, (int)r);

    if(r != LV_RESULT_OK) {
        fprintf(stderr, "[envhub-app] ERROR: lv_linux_drm_set_file failed\n");
        fprintf(stderr, "[envhub-app] Hint: verify /dev/dri/cardX exists and you have atomic+dumb buffers\n");
        return 1;
    }

    lv_display_set_default(disp);
    printf("[envhub-app] default display set\n");

    int32_t hor = lv_display_get_horizontal_resolution(disp);
    int32_t ver = lv_display_get_vertical_resolution(disp);
    lv_color_format_t cf = lv_display_get_color_format(disp);

    printf("[envhub-app] detected resolution: %" LV_PRId32 " x %" LV_PRId32 "\n", hor, ver);
    printf("[envhub-app] color format enum: %d\n", (int)cf);

    pthread_t th;
    if(pthread_create(&th, NULL, tick_thread, NULL) != 0) {
        fprintf(stderr, "[envhub-app] ERROR: pthread_create failed: %s\n", strerror(errno));
        return 1;
    }
    printf("[envhub-app] tick thread started\n");
}

int main(int argc, char **argv)
{
    time_service_init();
    lvgl_drm_init();
    ui_envhub_init();

    return 0;
}

// sets up logging

// initializes LVGL + input drivers

// initializes services

// creates threads/tasks

// starts the main loops

// defines shutdown order