/**
 * @file ui_envhub.c
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <syslog.h>

#include "ui_envhub.h"
#include "ui_helpers.h"

/* Main screen objects */
static lv_obj_t *main_screen = NULL;
static lv_obj_t *time_panel = NULL;
static lv_obj_t *battery_panel = NULL;
static lv_obj_t *time_label = NULL;
static lv_obj_t *battery_capacity_label = NULL;
static lv_obj_t *temp_label = NULL;
static lv_obj_t *humidity_label = NULL;
static lv_obj_t *co2_label = NULL;
static lv_obj_t *pressure_label = NULL;
static lv_obj_t *eco2_label = NULL;
static lv_obj_t *tvoc_label = NULL;
static lv_obj_t *status_bar = NULL;
static lv_obj_t *status_label = NULL;

/* Secondary screen objects */
static lv_obj_t *secondary_screen = NULL;

static lv_obj_t *secondary_back_btn = NULL;
static lv_obj_t *secondary_back_label = NULL;

/* Main screen functions */
static void ui_envhub_show_main_screen(void);
static void bind_main_screen(lv_obj_t *screen);
static void main_top_panel_event_cb(lv_event_t *e);
static void time_update_cb(lv_timer_t *t);

/* Secondary screen functions */
static void ui_envhub_show_secondary_screen(void);
static void bind_secondary_screen(lv_obj_t *screen);
static void secondary_back_event_cb(lv_event_t *e);

void ui_envhub_init(void)
{
    ui_envhub_init_gen(NULL);

    main_screen = screen_main_create();
    secondary_screen = screen_secondary_create();

    if (!main_screen || !secondary_screen)
    {
        syslog(LOG_ERR, "Failed to create UI screens");
        return;
    }

    disable_scroll_recursive(main_screen);
    disable_scroll_recursive(secondary_screen);

    bind_main_screen(main_screen);
    bind_secondary_screen(secondary_screen);

    ui_envhub_set_time_text(NULL);
    lv_timer_create(time_update_cb, 60000, NULL);

    lv_screen_load(main_screen);
}

void ui_envhub_set_time_text(const char *s)
{
    char buf[32];

    if (!time_label)
        return;

    if (!lv_obj_is_valid(time_label))
    {
        time_label = NULL;
        return;
    }

    if (lv_obj_get_screen(time_label) == NULL)
    {
        return;
    }

    if (s)
    {
        lv_label_set_text(time_label, s);
        return;
    }

    time_t now = time(NULL);
    if (now == (time_t)-1)
    {
        lv_label_set_text(time_label, "--");
        return;
    }

    struct tm tm_now;
    if (!localtime_r(&now, &tm_now))
    {
        lv_label_set_text(time_label, "--");
        return;
    }

    if (strftime(buf, sizeof(buf), "%b %e %H:%M", &tm_now) == 0)
    {
        lv_label_set_text(time_label, "--");
        return;
    }

    lv_label_set_text(time_label, buf);
}

void ui_envhub_set_bq27441(const ui_bq27441_data_t *bq27441)
{
    char buf[32];

    if (battery_capacity_label)
    {
        snprintf(buf, sizeof(buf), "%u%%", bq27441->capacity_percent);
        lv_label_set_text(battery_capacity_label, buf);
    }
}

void ui_envhub_set_scd30(const ui_scd30_data_t *scd30)
{
    char buf[32];

    if (co2_label)
    {
        snprintf(buf, sizeof(buf), "%.0f", scd30->co2_ppm);
        lv_label_set_text(co2_label, buf);
    }

    if (temp_label)
    {
        snprintf(buf, sizeof(buf), "%.1f", scd30->temperature_c);
        lv_label_set_text(temp_label, buf);
    }

    if (humidity_label)
    {
        snprintf(buf, sizeof(buf), "%.1f", scd30->humidity_rh);
        lv_label_set_text(humidity_label, buf);
    }
}

void ui_envhub_set_bmp580(const ui_bmp580_data_t *bmp580)
{

    char buf[32];

    if (pressure_label)
    {
        snprintf(buf, sizeof(buf), "%.1f", bmp580->pressure_hpa);
        lv_label_set_text(pressure_label, buf);
    }
}

void ui_envhub_set_sgp30(const ui_sgp30_data_t *sgp30)
{
    char buf[32];

    if (eco2_label)
    {
        snprintf(buf, sizeof(buf), "%.0f", sgp30->eco2_ppm);
        lv_label_set_text(eco2_label, buf);
    }

    if (tvoc_label)
    {
        snprintf(buf, sizeof(buf), "%.0f", sgp30->tvoc_ppb);
        lv_label_set_text(tvoc_label, buf);
    }
}

void ui_envhub_show_shutdown_popup(void)
{
    lv_obj_t *scr = lv_screen_active();

    lv_obj_t *overlay = lv_obj_create(scr);
    lv_obj_remove_style_all(overlay);
    lv_obj_set_size(overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(overlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(overlay, LV_OPA_60, 0);
    lv_obj_center(overlay);

    lv_obj_t *panel = lv_obj_create(overlay);
    lv_obj_set_size(panel, 220, 100);
    lv_obj_center(panel);

    lv_obj_t *label = lv_label_create(panel);
    lv_label_set_text(label, "Shutting down...");
    lv_obj_center(label);
}

void ui_envhub_set_status_summary(ui_status_severity_t severity, const char *text)
{
    lv_color_t bg_color;
    lv_color_t text_color;

    if (!text)
        text = "OK";

    if (status_label)
    {
        lv_label_set_text(status_label, text);
    }

    switch (severity)
    {
        case UI_STATUS_SEV_CRITICAL:
            bg_color = lv_palette_main(LV_PALETTE_RED);
            text_color = lv_color_white();
            break;

        case UI_STATUS_SEV_WARNING:
            bg_color = lv_palette_main(LV_PALETTE_ORANGE);
            text_color = lv_color_black();
            break;

        case UI_STATUS_SEV_INFO:
        default:
            bg_color = lv_palette_main(LV_PALETTE_BLUE_GREY);
            text_color = lv_color_white();
            break;
    }

    if (status_bar)
    {
        lv_obj_set_style_bg_color(status_bar, bg_color, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(status_bar, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_width(status_bar, 0, LV_PART_MAIN);
    }

    if (status_label)
    {
        lv_obj_set_style_text_color(status_label, text_color, LV_PART_MAIN);
    }
}

static void bind_main_screen(lv_obj_t *screen)
{
    time_label = find_by_name_dfs(screen, "time");
    time_panel = find_by_name_dfs(screen, "time_panel");
    battery_panel = find_by_name_dfs(screen, "battery_panel");
    battery_capacity_label = find_by_name_dfs(screen, "battery_capacity");
    temp_label = find_by_name_dfs(screen, "temp_value");
    humidity_label = find_by_name_dfs(screen, "humidity_value");
    co2_label = find_by_name_dfs(screen, "co2_value");
    pressure_label = find_by_name_dfs(screen, "pressure_value");
    eco2_label = find_by_name_dfs(screen, "eco2_value");
    tvoc_label = find_by_name_dfs(screen, "tvoc_value");
    status_bar = find_by_name_dfs(screen, "status_bar");
    status_label = find_by_name_dfs(screen, "status_label");

    if (time_panel)
    {
        lv_obj_add_flag(time_panel, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(time_panel, main_top_panel_event_cb, LV_EVENT_CLICKED, NULL);
    }

    if (battery_panel)
    {
        lv_obj_add_flag(battery_panel, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(battery_panel, main_top_panel_event_cb, LV_EVENT_CLICKED, NULL);
    }
}

static void bind_secondary_screen(lv_obj_t *screen)
{
    secondary_back_btn = find_by_name_dfs(screen, "secondary_back_btn");
    secondary_back_label = find_by_name_dfs(screen, "secondary_back_label");

    if (secondary_back_label)
    {
        lv_label_set_text(secondary_back_label, LV_SYMBOL_LEFT);
    }

    if (secondary_back_btn)
    {
        lv_obj_add_event_cb(secondary_back_btn, secondary_back_event_cb, LV_EVENT_CLICKED, NULL);
    }
}

static void time_update_cb(lv_timer_t *t)
{
    (void)t;
    ui_envhub_set_time_text(NULL);
}

static void main_top_panel_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        ui_envhub_show_secondary_screen();
    }
}

static void secondary_back_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        ui_envhub_show_main_screen();
    }
}

static void ui_envhub_show_main_screen(void)
{
    if (main_screen)
    {
        lv_screen_load(main_screen);
    }
}

static void ui_envhub_show_secondary_screen(void)
{
    if (secondary_screen)
    {
        lv_screen_load(secondary_screen);
    }
}
