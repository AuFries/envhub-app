/**
 * @file screen_secondary_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_secondary_gen.h"
#include "ui_envhub.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *screen_secondary_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_main;
    static lv_style_t header_panel;
    static lv_style_t tile_panel;
    static lv_style_t status_panel;
    static lv_style_t button_panel;
    static lv_style_t wrap_plain;
    static lv_style_t value_text;
    static lv_style_t label_text;
    static lv_style_t small_text;
    static lv_style_t header_text;
    static lv_style_t status_text;

    static bool style_inited = false;

    if (!style_inited)
    {
        lv_style_init(&style_main);
        lv_style_set_bg_color(&style_main, lv_color_hex(0xE9EFF2));

        lv_style_init(&header_panel);
        lv_style_set_bg_color(&header_panel, lv_color_hex(0x2C5D6C));
        lv_style_set_border_width(&header_panel, 0);
        lv_style_set_radius(&header_panel, 16);

        lv_style_init(&tile_panel);
        lv_style_set_bg_color(&tile_panel, lv_color_hex(0xFFFFFF));
        lv_style_set_border_width(&tile_panel, 1);
        lv_style_set_border_color(&tile_panel, lv_color_hex(0xD7E0E5));
        lv_style_set_radius(&tile_panel, 14);

        lv_style_init(&status_panel);
        lv_style_set_bg_color(&status_panel, lv_color_hex(0x2C5D6C));
        lv_style_set_border_width(&status_panel, 0);
        lv_style_set_radius(&status_panel, 16);

        lv_style_init(&button_panel);
        lv_style_set_bg_color(&button_panel, lv_color_hex(0xFFFFFF));
        lv_style_set_border_width(&button_panel, 1);
        lv_style_set_border_color(&button_panel, lv_color_hex(0xD7E0E5));
        lv_style_set_radius(&button_panel, 12);

        lv_style_init(&wrap_plain);
        lv_style_set_bg_opa(&wrap_plain, 0);
        lv_style_set_border_width(&wrap_plain, 0);
        lv_style_set_radius(&wrap_plain, 0);

        lv_style_init(&value_text);
        lv_style_set_text_color(&value_text, lv_color_hex(0x182126));
        lv_style_set_text_font(&value_text, plex_sans_24);

        lv_style_init(&label_text);
        lv_style_set_text_color(&label_text, lv_color_hex(0x4F5D65));

        lv_style_init(&small_text);
        lv_style_set_text_color(&small_text, lv_color_hex(0x4F5D65));
        lv_style_set_text_font(&small_text, plex_sans_12);

        lv_style_init(&header_text);
        lv_style_set_text_color(&header_text, lv_color_hex(0xFFFFFF));
        lv_style_set_text_font(&header_text, plex_sans_24);

        lv_style_init(&status_text);
        lv_style_set_text_color(&status_text, lv_color_hex(0xFFFFFF));

        style_inited = true;
    }

    lv_obj_t *lv_obj_0 = lv_obj_create(NULL);
    lv_obj_set_name_static(lv_obj_0, "screen_secondary_#");

    lv_obj_add_style(lv_obj_0, &style_main, 0);
    lv_obj_t *secondary_top_row = lv_obj_create(lv_obj_0);
    lv_obj_set_name(secondary_top_row, "secondary_top_row");
    lv_obj_set_width(secondary_top_row, 236);
    lv_obj_set_height(secondary_top_row, 42);
    lv_obj_set_align(secondary_top_row, LV_ALIGN_TOP_MID);
    lv_obj_set_x(secondary_top_row, 0);
    lv_obj_set_y(secondary_top_row, 2);
    lv_obj_set_scrollbar_mode(secondary_top_row, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(secondary_top_row, &header_panel, 0);
    lv_obj_t *secondary_back_btn = lv_button_create(secondary_top_row);
    lv_obj_set_name(secondary_back_btn, "secondary_back_btn");
    lv_obj_set_width(secondary_back_btn, 60);
    lv_obj_set_height(secondary_back_btn, 30);
    lv_obj_set_align(secondary_back_btn, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(secondary_back_btn, -5);
    lv_obj_set_y(secondary_back_btn, 0);
    lv_obj_set_scrollbar_mode(secondary_back_btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(secondary_back_btn, &button_panel, 0);
    lv_obj_t *secondary_back_label = lv_label_create(secondary_back_btn);
    lv_obj_set_name(secondary_back_label, "secondary_back_label");
    lv_label_set_text(secondary_back_label, "");
    lv_obj_set_align(secondary_back_label, LV_ALIGN_CENTER);
    lv_obj_add_style(secondary_back_label, &label_text, 0);

    lv_obj_t *lv_label_0 = lv_label_create(secondary_top_row);
    lv_label_set_text(lv_label_0, "System Stats");
    lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_0, 25);
    lv_obj_set_y(lv_label_0, 0);
    lv_obj_add_style(lv_label_0, &header_text, 0);

    lv_obj_t *secondary_status_row = lv_obj_create(lv_obj_0);
    lv_obj_set_name(secondary_status_row, "secondary_status_row");
    lv_obj_set_width(secondary_status_row, 236);
    lv_obj_set_height(secondary_status_row, 44);
    lv_obj_set_align(secondary_status_row, LV_ALIGN_TOP_MID);
    lv_obj_set_x(secondary_status_row, 0);
    lv_obj_set_y(secondary_status_row, 48);
    lv_obj_set_scrollbar_mode(secondary_status_row, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(secondary_status_row, &header_panel, 0);
    lv_obj_t *lv_label_1 = lv_label_create(secondary_status_row);
    lv_label_set_text(lv_label_1, "Presence (%)");
    lv_obj_set_align(lv_label_1, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(lv_label_1, 7);
    lv_obj_set_y(lv_label_1, -10);
    lv_obj_add_style(lv_label_1, &status_text, 0);

    lv_obj_t *presence_value = lv_label_create(secondary_status_row);
    lv_obj_set_name(presence_value, "presence_value");
    lv_label_set_text(presence_value, "0");
    lv_obj_set_align(presence_value, LV_ALIGN_CENTER);
    lv_obj_set_x(presence_value, -55);
    lv_obj_set_y(presence_value, 8);
    lv_obj_add_style(presence_value, &header_text, 0);

    lv_obj_t *lv_label_2 = lv_label_create(secondary_status_row);
    lv_label_set_text(lv_label_2, "Uptime");
    lv_obj_set_align(lv_label_2, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(lv_label_2, -20);
    lv_obj_set_y(lv_label_2, -10);
    lv_obj_add_style(lv_label_2, &status_text, 0);

    lv_obj_t *uptime_value = lv_label_create(secondary_status_row);
    lv_obj_set_name(uptime_value, "uptime_value");
    lv_label_set_text(uptime_value, "00:00:00");
    lv_obj_set_align(uptime_value, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(uptime_value, -5);
    lv_obj_set_y(uptime_value, 8);
    lv_obj_add_style(uptime_value, &header_text, 0);

    lv_obj_t *secondary_power_wrap = lv_obj_create(lv_obj_0);
    lv_obj_set_name(secondary_power_wrap, "secondary_power_wrap");
    lv_obj_set_width(secondary_power_wrap, 236);
    lv_obj_set_height(secondary_power_wrap, 112);
    lv_obj_set_align(secondary_power_wrap, LV_ALIGN_TOP_MID);
    lv_obj_set_x(secondary_power_wrap, 0);
    lv_obj_set_y(secondary_power_wrap, 96);
    lv_obj_set_scrollbar_mode(secondary_power_wrap, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(secondary_power_wrap, &wrap_plain, 0);
    lv_obj_t *tile_batt_voltage = lv_obj_create(secondary_power_wrap);
    lv_obj_set_name(tile_batt_voltage, "tile_batt_voltage");
    lv_obj_set_width(tile_batt_voltage, 112);
    lv_obj_set_height(tile_batt_voltage, 50);
    lv_obj_set_align(tile_batt_voltage, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(tile_batt_voltage, -10);
    lv_obj_set_y(tile_batt_voltage, -10);
    lv_obj_set_scrollbar_mode(tile_batt_voltage, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(tile_batt_voltage, &tile_panel, 0);
    lv_obj_t *lv_label_3 = lv_label_create(tile_batt_voltage);
    lv_label_set_text(lv_label_3, "Batt V");
    lv_obj_set_align(lv_label_3, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_3, 0);
    lv_obj_set_y(lv_label_3, -10);
    lv_obj_add_style(lv_label_3, &small_text, 0);

    lv_obj_t *battery_voltage_value = lv_label_create(tile_batt_voltage);
    lv_obj_set_name(battery_voltage_value, "battery_voltage_value");
    lv_label_set_text(battery_voltage_value, "0.00");
    lv_obj_set_align(battery_voltage_value, LV_ALIGN_CENTER);
    lv_obj_set_x(battery_voltage_value, 0);
    lv_obj_set_y(battery_voltage_value, 10);
    lv_obj_add_style(battery_voltage_value, &label_text, 0);

    lv_obj_t *tile_system_current = lv_obj_create(secondary_power_wrap);
    lv_obj_set_name(tile_system_current, "tile_system_current");
    lv_obj_set_width(tile_system_current, 112);
    lv_obj_set_height(tile_system_current, 50);
    lv_obj_set_align(tile_system_current, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(tile_system_current, 10);
    lv_obj_set_y(tile_system_current, -10);
    lv_obj_set_scrollbar_mode(tile_system_current, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(tile_system_current, &tile_panel, 0);
    lv_obj_t *lv_label_4 = lv_label_create(tile_system_current);
    lv_label_set_text(lv_label_4, "System I (mA)");
    lv_obj_set_align(lv_label_4, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_4, 0);
    lv_obj_set_y(lv_label_4, -10);
    lv_obj_add_style(lv_label_4, &small_text, 0);

    lv_obj_t *system_current_value = lv_label_create(tile_system_current);
    lv_obj_set_name(system_current_value, "system_current_value");
    lv_label_set_text(system_current_value, "0");
    lv_obj_set_align(system_current_value, LV_ALIGN_CENTER);
    lv_obj_set_x(system_current_value, 0);
    lv_obj_set_y(system_current_value, 10);
    lv_obj_add_style(system_current_value, &label_text, 0);

    lv_obj_t *tile_system_power = lv_obj_create(secondary_power_wrap);
    lv_obj_set_name(tile_system_power, "tile_system_power");
    lv_obj_set_width(tile_system_power, 112);
    lv_obj_set_height(tile_system_power, 50);
    lv_obj_set_align(tile_system_power, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_x(tile_system_power, -10);
    lv_obj_set_y(tile_system_power, 10);
    lv_obj_set_scrollbar_mode(tile_system_power, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(tile_system_power, &tile_panel, 0);
    lv_obj_t *lv_label_5 = lv_label_create(tile_system_power);
    lv_label_set_text(lv_label_5, "System P (mW)");
    lv_obj_set_align(lv_label_5, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_5, 0);
    lv_obj_set_y(lv_label_5, -10);
    lv_obj_add_style(lv_label_5, &small_text, 0);

    lv_obj_t *system_power_value = lv_label_create(tile_system_power);
    lv_obj_set_name(system_power_value, "system_power_value");
    lv_label_set_text(system_power_value, "0.00");
    lv_obj_set_align(system_power_value, LV_ALIGN_CENTER);
    lv_obj_set_x(system_power_value, 0);
    lv_obj_set_y(system_power_value, 10);
    lv_obj_add_style(system_power_value, &label_text, 0);

    lv_obj_t *tile_sensor_reads = lv_obj_create(secondary_power_wrap);
    lv_obj_set_name(tile_sensor_reads, "tile_sensor_reads");
    lv_obj_set_width(tile_sensor_reads, 112);
    lv_obj_set_height(tile_sensor_reads, 50);
    lv_obj_set_align(tile_sensor_reads, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_set_x(tile_sensor_reads, 10);
    lv_obj_set_y(tile_sensor_reads, 10);
    lv_obj_set_scrollbar_mode(tile_sensor_reads, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(tile_sensor_reads, &tile_panel, 0);
    lv_obj_t *lv_label_6 = lv_label_create(tile_sensor_reads);
    lv_label_set_text(lv_label_6, "Reads");
    lv_obj_set_align(lv_label_6, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_6, 0);
    lv_obj_set_y(lv_label_6, -10);
    lv_obj_add_style(lv_label_6, &small_text, 0);

    lv_obj_t *sensor_reads_value = lv_label_create(tile_sensor_reads);
    lv_obj_set_name(sensor_reads_value, "sensor_reads_value");
    lv_label_set_text(sensor_reads_value, "0");
    lv_obj_set_align(sensor_reads_value, LV_ALIGN_CENTER);
    lv_obj_set_x(sensor_reads_value, 0);
    lv_obj_set_y(sensor_reads_value, 10);
    lv_obj_add_style(sensor_reads_value, &label_text, 0);

    lv_obj_t *secondary_stats_panel = lv_obj_create(lv_obj_0);
    lv_obj_set_name(secondary_stats_panel, "secondary_stats_panel");
    lv_obj_set_width(secondary_stats_panel, 236);
    lv_obj_set_height(secondary_stats_panel, 104);
    lv_obj_set_align(secondary_stats_panel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_x(secondary_stats_panel, 0);
    lv_obj_set_y(secondary_stats_panel, -4);
    lv_obj_set_scrollbar_mode(secondary_stats_panel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(secondary_stats_panel, &tile_panel, 0);
    lv_obj_t *lv_label_7 = lv_label_create(secondary_stats_panel);
    lv_label_set_text(lv_label_7, "Peripherals (mA)");
    lv_obj_set_align(lv_label_7, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(lv_label_7, 12);
    lv_obj_set_y(lv_label_7, 0);
    lv_obj_add_style(lv_label_7, &small_text, 0);

    lv_obj_t *peripheral_current_value = lv_label_create(secondary_stats_panel);
    lv_obj_set_name(peripheral_current_value, "peripheral_current_value");
    lv_label_set_text(peripheral_current_value, "0");
    lv_obj_set_align(peripheral_current_value, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(peripheral_current_value, 12);
    lv_obj_set_y(peripheral_current_value, 16);
    lv_obj_add_style(peripheral_current_value, &value_text, 0);

    lv_obj_t *lv_label_8 = lv_label_create(secondary_stats_panel);
    lv_label_set_text(lv_label_8, "Compute (mA)");
    lv_obj_set_align(lv_label_8, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(lv_label_8, 12);
    lv_obj_set_y(lv_label_8, 48);
    lv_obj_add_style(lv_label_8, &small_text, 0);

    lv_obj_t *compute_current_value = lv_label_create(secondary_stats_panel);
    lv_obj_set_name(compute_current_value, "compute_current_value");
    lv_label_set_text(compute_current_value, "0");
    lv_obj_set_align(compute_current_value, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(compute_current_value, 12);
    lv_obj_set_y(compute_current_value, 64);
    lv_obj_add_style(compute_current_value, &value_text, 0);

    lv_obj_t *lv_label_9 = lv_label_create(secondary_stats_panel);
    lv_label_set_text(lv_label_9, "CPU (%)");
    lv_obj_set_align(lv_label_9, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(lv_label_9, 132);
    lv_obj_set_y(lv_label_9, 0);
    lv_obj_add_style(lv_label_9, &small_text, 0);

    lv_obj_t *cpu_util_value = lv_label_create(secondary_stats_panel);
    lv_obj_set_name(cpu_util_value, "cpu_util_value");
    lv_label_set_text(cpu_util_value, "0");
    lv_obj_set_align(cpu_util_value, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(cpu_util_value, 132);
    lv_obj_set_y(cpu_util_value, 16);
    lv_obj_add_style(cpu_util_value, &value_text, 0);

    lv_obj_t *lv_label_10 = lv_label_create(secondary_stats_panel);
    lv_label_set_text(lv_label_10, "Memory (%)");
    lv_obj_set_align(lv_label_10, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(lv_label_10, 132);
    lv_obj_set_y(lv_label_10, 48);
    lv_obj_add_style(lv_label_10, &small_text, 0);

    lv_obj_t *memory_util_value = lv_label_create(secondary_stats_panel);
    lv_obj_set_name(memory_util_value, "memory_util_value");
    lv_label_set_text(memory_util_value, "0");
    lv_obj_set_align(memory_util_value, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(memory_util_value, 132);
    lv_obj_set_y(memory_util_value, 64);
    lv_obj_add_style(memory_util_value, &value_text, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
