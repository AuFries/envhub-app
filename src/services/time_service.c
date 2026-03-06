#include <lvgl/lvgl.h>
#include <stdio.h>
#include "services/time_service.h"
#include "ui/ui_envhub.h"

static void tick_cb(lv_timer_t *t)
{
    (void)t;
    static uint32_t sec = 0;
    sec++;

    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%02u:%02u:%02u",
                (unsigned)((sec / 3600) % 24),
                (unsigned)((sec / 60) % 60),
                (unsigned)(sec % 60));

    ui_envhub_set_time_text(buf);
}

void time_service_start(void)
{
    lv_timer_create(tick_cb, 1000, NULL);
}