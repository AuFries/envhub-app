/**
 * @file screen_main_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen_main_gen.h"
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

lv_obj_t * screen_main_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t style_main;
    static lv_style_t panel;
    static lv_style_t value_text;
    static lv_style_t label_text;
    static lv_style_t small_text;

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&style_main);
        lv_style_set_bg_color(&style_main, lv_color_hex(0x00688a));

        lv_style_init(&panel);
        lv_style_set_bg_color(&panel, lv_color_hex(0xFFFFFF));
        lv_style_set_border_width(&panel, 1);
        lv_style_set_border_color(&panel, lv_color_hex(0x6C8794));
        lv_style_set_radius(&panel, 16);

        lv_style_init(&value_text);
        lv_style_set_text_color(&value_text, lv_color_hex(0x000000));
        lv_style_set_text_font(&value_text, plex_sans_24);

        lv_style_init(&label_text);
        lv_style_set_text_color(&label_text, lv_color_hex(0x000000));

        lv_style_init(&small_text);
        lv_style_set_text_color(&small_text, lv_color_hex(0x000000));
        lv_style_set_text_font(&small_text, plex_sans_12);

        style_inited = true;
    }

    lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
    lv_obj_set_name_static(lv_obj_0, "screen_main_#");

    lv_obj_add_style(lv_obj_0, &style_main, 0);
    lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_1, 160);
    lv_obj_set_height(lv_obj_1, 40);
    lv_obj_set_align(lv_obj_1, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(lv_obj_1, 2);
    lv_obj_set_y(lv_obj_1, 0);
    lv_obj_add_style(lv_obj_1, &panel, 0);
    lv_obj_t * time = lv_label_create(lv_obj_1);
    lv_obj_set_name(time, "time");
    lv_label_set_text(time, "Jan 1 00:00");
    lv_obj_set_align(time, LV_ALIGN_CENTER);
    lv_obj_add_style(time, &value_text, 0);
    
    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_2, 80);
    lv_obj_set_height(lv_obj_2, 40);
    lv_obj_set_align(lv_obj_2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_obj_2, -2);
    lv_obj_set_y(lv_obj_2, 0);
    lv_obj_add_style(lv_obj_2, &panel, 0);
    lv_obj_t * battery_capacity = lv_label_create(lv_obj_2);
    lv_obj_set_name(battery_capacity, "battery_capacity");
    lv_label_set_text(battery_capacity, "0%");
    lv_obj_set_align(battery_capacity, LV_ALIGN_CENTER);
    lv_obj_add_style(battery_capacity, &value_text, 0);
    
    lv_obj_t * ui_tiles_wrap = lv_obj_create(lv_obj_0);
    lv_obj_set_name(ui_tiles_wrap, "ui_tiles_wrap");
    lv_obj_set_width(ui_tiles_wrap, 240);
    lv_obj_set_height(ui_tiles_wrap, 240);
    lv_obj_set_align(ui_tiles_wrap, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(ui_tiles_wrap, 0);
    lv_obj_set_y(ui_tiles_wrap, 43);
    lv_obj_t * tile_temp = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_temp, "tile_temp");
    lv_obj_set_width(tile_temp, 108);
    lv_obj_set_height(tile_temp, 70);
    lv_obj_add_style(tile_temp, &panel, 0);
    lv_obj_t * lv_label_0 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_0, "Temp (°C)");
    lv_obj_set_align(lv_label_0, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_0, -12);
    lv_obj_add_style(lv_label_0, &label_text, 0);
    
    lv_obj_t * temp_value = lv_label_create(tile_temp);
    lv_obj_set_name(temp_value, "temp_value");
    lv_label_set_text(temp_value, "0");
    lv_obj_set_align(temp_value, LV_ALIGN_CENTER);
    lv_obj_set_x(temp_value, 0);
    lv_obj_set_y(temp_value, 5);
    lv_obj_add_style(temp_value, &value_text, 0);
    
    lv_obj_t * tile_humidity = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_humidity, "tile_humidity");
    lv_obj_set_width(tile_humidity, 108);
    lv_obj_set_height(tile_humidity, 70);
    lv_obj_add_style(tile_humidity, &panel, 0);
    lv_obj_t * lv_label_1 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_1, "Humidity (%)");
    lv_obj_set_align(lv_label_1, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_1, -12);
    lv_obj_add_style(lv_label_1, &label_text, 0);
    
    lv_obj_t * humidity_value = lv_label_create(tile_humidity);
    lv_obj_set_name(humidity_value, "humidity_value");
    lv_label_set_text(humidity_value, "0");
    lv_obj_set_align(humidity_value, LV_ALIGN_CENTER);
    lv_obj_set_x(humidity_value, 0);
    lv_obj_set_y(humidity_value, 5);
    lv_obj_add_style(humidity_value, &value_text, 0);
    
    lv_obj_t * tile_co2 = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_co2, "tile_co2");
    lv_obj_set_width(tile_co2, 108);
    lv_obj_set_height(tile_co2, 70);
    lv_obj_add_style(tile_co2, &panel, 0);
    lv_obj_t * lv_label_2 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_2, "CO2 (ppm)");
    lv_obj_set_align(lv_label_2, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_2, -12);
    lv_obj_add_style(lv_label_2, &label_text, 0);
    
    lv_obj_t * co2_value = lv_label_create(tile_co2);
    lv_obj_set_name(co2_value, "co2_value");
    lv_label_set_text(co2_value, "0");
    lv_obj_set_align(co2_value, LV_ALIGN_CENTER);
    lv_obj_set_x(co2_value, 0);
    lv_obj_set_y(co2_value, 5);
    lv_obj_add_style(co2_value, &value_text, 0);
    
    lv_obj_t * tile_pressure = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_pressure, "tile_pressure");
    lv_obj_set_width(tile_pressure, 108);
    lv_obj_set_height(tile_pressure, 70);
    lv_obj_add_style(tile_pressure, &panel, 0);
    lv_obj_t * lv_label_3 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_3, "Press (hPa)");
    lv_obj_set_align(lv_label_3, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_3, -12);
    lv_obj_add_style(lv_label_3, &label_text, 0);
    
    lv_obj_t * pressure_value = lv_label_create(tile_pressure);
    lv_obj_set_name(pressure_value, "pressure_value");
    lv_label_set_text(pressure_value, "0");
    lv_obj_set_align(pressure_value, LV_ALIGN_CENTER);
    lv_obj_set_x(pressure_value, 0);
    lv_obj_set_y(pressure_value, 5);
    lv_obj_add_style(pressure_value, &value_text, 0);
    
    lv_obj_t * tile_eco2 = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_eco2, "tile_eco2");
    lv_obj_set_width(tile_eco2, 108);
    lv_obj_set_height(tile_eco2, 70);
    lv_obj_add_style(tile_eco2, &panel, 0);
    lv_obj_t * lv_label_4 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_4, "eCO2 (ppm)");
    lv_obj_set_align(lv_label_4, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_4, -12);
    lv_obj_add_style(lv_label_4, &label_text, 0);
    
    lv_obj_t * eco2_value = lv_label_create(tile_eco2);
    lv_obj_set_name(eco2_value, "eco2_value");
    lv_label_set_text(eco2_value, "0");
    lv_obj_set_align(eco2_value, LV_ALIGN_CENTER);
    lv_obj_set_x(eco2_value, 0);
    lv_obj_set_y(eco2_value, 5);
    lv_obj_add_style(eco2_value, &value_text, 0);
    
    lv_obj_t * tile_tvoc = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_tvoc, "tile_tvoc");
    lv_obj_set_width(tile_tvoc, 108);
    lv_obj_set_height(tile_tvoc, 70);
    lv_obj_add_style(tile_tvoc, &panel, 0);
    lv_obj_t * lv_label_5 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_5, "TVOC (ppb)");
    lv_obj_set_align(lv_label_5, LV_ALIGN_TOP_MID);
    lv_obj_set_x(lv_label_5, 0);
    lv_obj_set_y(lv_label_5, -12);
    lv_obj_add_style(lv_label_5, &label_text, 0);
    
    lv_obj_t * tvoc_value = lv_label_create(tile_tvoc);
    lv_obj_set_name(tvoc_value, "tvoc_value");
    lv_label_set_text(tvoc_value, "0");
    lv_obj_set_align(tvoc_value, LV_ALIGN_CENTER);
    lv_obj_set_x(tvoc_value, 0);
    lv_obj_set_y(tvoc_value, 5);
    lv_obj_add_style(tvoc_value, &value_text, 0);
    
    lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_3, 240);
    lv_obj_set_height(lv_obj_3, 34);
    lv_obj_set_align(lv_obj_3, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_x(lv_obj_3, 0);
    lv_obj_set_y(lv_obj_3, 0);
    lv_obj_add_style(lv_obj_3, &panel, 0);
    lv_obj_t * lv_label_6 = lv_label_create(lv_obj_3);
    lv_label_set_text(lv_label_6, "Status: {status message}");
    lv_obj_set_align(lv_label_6, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(lv_label_6, 6);
    lv_obj_add_style(lv_label_6, &label_text, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

