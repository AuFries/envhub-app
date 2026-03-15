/**
 * @file ui_envhub.c
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ui_envhub.h"

static lv_obj_t *status_bar = NULL;
static lv_obj_t *status_label = NULL;

static lv_obj_t *time_label = NULL;
static lv_obj_t *battery_capacity_label = NULL;
static lv_obj_t *co2_label = NULL;
static lv_obj_t *temp_label = NULL;
static lv_obj_t *humidity_label = NULL;
static lv_obj_t *pressure_label = NULL;
static lv_obj_t *eco2_label = NULL;
static lv_obj_t *tvoc_label = NULL;

static lv_obj_t *power_label = NULL;
static lv_obj_t *current_label = NULL;
static lv_obj_t *voltage_label = NULL;

static void bind_labels(lv_obj_t *screen);
static void time_update_cb(lv_timer_t *t);

static lv_obj_t *find_by_name_dfs(lv_obj_t *root, const char *target)
{
    if (!root)
        return NULL;

    const char *n = lv_obj_get_name(root); /* LVGL 9 */
    if (n && strcmp(n, target) == 0)
        return root;

    uint32_t cnt = lv_obj_get_child_cnt(root);
    for (uint32_t i = 0; i < cnt; i++)
    {
        lv_obj_t *hit = find_by_name_dfs(lv_obj_get_child(root, i), target);
        if (hit)
            return hit;
    }
    return NULL;
}

static lv_obj_t *find_tiles_wrap_anywhere(void)
{
    lv_display_t *d = lv_display_get_default();
    if (!d)
        return NULL;

    lv_obj_t *roots[] = {
        lv_display_get_screen_active(d),
        lv_display_get_layer_top(d),
        lv_display_get_layer_sys(d),
        lv_display_get_layer_bottom(d),
    };

    for (size_t i = 0; i < sizeof(roots) / sizeof(roots[0]); i++)
    {
        lv_obj_t *hit = find_by_name_dfs(roots[i], "ui_tiles_wrap");
        if (hit)
            return hit;
    }
    return NULL;
}

static void apply_tiles_grid(lv_obj_t *wrap)
{
    if (!wrap)
        return;

    lv_obj_clear_flag(wrap, LV_OBJ_FLAG_SCROLLABLE);

    static lv_coord_t col_dsc[] = {110, 110, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {70, 70, 70, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_style_grid_column_dsc_array(wrap, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(wrap, row_dsc, 0);
    lv_obj_set_layout(wrap, LV_LAYOUT_GRID);

    lv_obj_set_style_pad_left(wrap, 4, 0);
    lv_obj_set_style_pad_right(wrap, 4, 0);
    lv_obj_set_style_pad_top(wrap, 1, 0);
    lv_obj_set_style_pad_row(wrap, 6, 0);
    lv_obj_set_style_pad_column(wrap, 8, 0);

    lv_obj_clear_flag(wrap, LV_OBJ_FLAG_SCROLLABLE);

    uint32_t n = lv_obj_get_child_cnt(wrap);
    if (n < 6)
        return;

    for (uint32_t i = 0; i < 6; i++)
    {
        lv_obj_t *tile = lv_obj_get_child(wrap, i);
        uint32_t col = i % 2;
        uint32_t row = i / 2;

        lv_obj_set_grid_cell(tile, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
    }

    lv_obj_update_layout(wrap);
}

static void remove_scrollbars(lv_obj_t *scr)
{
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
    for (uint32_t i = 0; i < lv_obj_get_child_cnt(scr); i++)
    {
        lv_obj_t *tile = lv_obj_get_child(scr, i);
        lv_obj_clear_flag(tile, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_scrollbar_mode(tile, LV_SCROLLBAR_MODE_OFF);
    }
}

static void apply_grid_cb(lv_timer_t *t)
{
    lv_obj_t *wrap = find_tiles_wrap_anywhere();
    if (wrap)
    {
        remove_scrollbars(wrap);
        apply_tiles_grid(wrap);
        lv_timer_del(t); /* done */
    }
}

void ui_envhub_init(void)
{
    // lv_xml_register_font(NULL, "plex_sans_12", plex_sans_12);
    ui_envhub_init_gen(NULL);

    lv_obj_t *screen = screen_main_create();
    lv_screen_load(screen);

    time_label = find_by_name_dfs(screen, "time");

    bind_labels(screen);

    ui_envhub_set_time_text(NULL);
    lv_timer_create(time_update_cb, 60000, NULL);

    /* Layout fixup */
    lv_timer_create(apply_grid_cb, 1, NULL);
}

static void bind_labels(lv_obj_t *screen)
{
    if (!screen)
        return;

    status_bar = find_by_name_dfs(screen, "status_bar");
    status_label = find_by_name_dfs(screen, "status_label");

    battery_capacity_label = find_by_name_dfs(screen, "battery_capacity");
    co2_label = find_by_name_dfs(screen, "co2_value");
    temp_label = find_by_name_dfs(screen, "temp_value");
    humidity_label = find_by_name_dfs(screen, "humidity_value");
    pressure_label = find_by_name_dfs(screen, "pressure_value");
    eco2_label = find_by_name_dfs(screen, "eco2_value");
    tvoc_label = find_by_name_dfs(screen, "tvoc_value");

    // power_label    = find_by_name_dfs(screen, "power_value");
    // current_label  = find_by_name_dfs(screen, "current_value");
    // voltage_label  = find_by_name_dfs(screen, "voltage_value");
}

static void time_update_cb(lv_timer_t *t)
{
    (void)t;
    ui_envhub_set_time_text(NULL);
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

void ui_envhub_set_status_summary(status_severity_t severity, const char *text)
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
        case STATUS_SEV_CRITICAL:
            bg_color = lv_palette_main(LV_PALETTE_RED);
            text_color = lv_color_white();
            break;

        case STATUS_SEV_WARNING:
            bg_color = lv_palette_main(LV_PALETTE_ORANGE);
            text_color = lv_color_black();
            break;

        case STATUS_SEV_INFO:
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