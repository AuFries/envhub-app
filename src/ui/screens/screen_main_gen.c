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
    lv_label_set_text(time, "00:00:00");
    lv_obj_set_align(time, LV_ALIGN_CENTER);
    lv_obj_add_style(time, &value_text, 0);
    
    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_2, 80);
    lv_obj_set_height(lv_obj_2, 40);
    lv_obj_set_align(lv_obj_2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_obj_2, -2);
    lv_obj_set_y(lv_obj_2, 0);
    lv_obj_add_style(lv_obj_2, &panel, 0);
    lv_obj_t * lv_label_0 = lv_label_create(lv_obj_2);
    lv_label_set_text(lv_label_0, "🔋 ##%");
    lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
    lv_obj_add_style(lv_label_0, &value_text, 0);
    
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
    lv_obj_t * lv_label_1 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_1, "Temp (°C)");
    lv_obj_set_align(lv_label_1, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_1, -10);
    lv_obj_add_style(lv_label_1, &label_text, 0);
    
    lv_obj_t * temp_value = lv_label_create(tile_temp);
    lv_obj_set_name(temp_value, "temp_value");
    lv_label_set_text(temp_value, "0");
    lv_obj_set_align(temp_value, LV_ALIGN_CENTER);
    lv_obj_set_x(temp_value, -30);
    lv_obj_set_y(temp_value, 0);
    lv_obj_add_style(temp_value, &value_text, 0);
    
    lv_obj_t * lv_label_2 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_2, "mn 0");
    lv_obj_set_align(lv_label_2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_2, -15);
    lv_obj_set_y(lv_label_2, 6);
    lv_obj_add_style(lv_label_2, &small_text, 0);
    
    lv_obj_t * lv_label_3 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_3, "mx 0");
    lv_obj_set_align(lv_label_3, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_3, -15);
    lv_obj_set_y(lv_label_3, 20);
    lv_obj_add_style(lv_label_3, &small_text, 0);
    
    lv_obj_t * lv_label_4 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_4, "av 0");
    lv_obj_set_align(lv_label_4, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_4, -15);
    lv_obj_set_y(lv_label_4, 34);
    lv_obj_add_style(lv_label_4, &small_text, 0);
    
    lv_obj_t * tile_humidity = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_humidity, "tile_humidity");
    lv_obj_set_width(tile_humidity, 108);
    lv_obj_set_height(tile_humidity, 70);
    lv_obj_add_style(tile_humidity, &panel, 0);
    lv_obj_t * lv_label_5 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_5, "Humidity (%)");
    lv_obj_set_align(lv_label_5, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_5, -10);
    lv_obj_add_style(lv_label_5, &label_text, 0);
    
    lv_obj_t * humidity_value = lv_label_create(tile_humidity);
    lv_obj_set_name(humidity_value, "humidity_value");
    lv_label_set_text(humidity_value, "0%");
    lv_obj_set_align(humidity_value, LV_ALIGN_CENTER);
    lv_obj_set_x(humidity_value, -14);
    lv_obj_set_y(humidity_value, 0);
    lv_obj_add_style(humidity_value, &value_text, 0);
    
    lv_obj_t * lv_label_6 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_6, "mn 0");
    lv_obj_set_align(lv_label_6, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_6, -4);
    lv_obj_set_y(lv_label_6, 22);
    lv_obj_add_style(lv_label_6, &small_text, 0);
    
    lv_obj_t * lv_label_7 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_7, "mx 0");
    lv_obj_set_align(lv_label_7, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_7, -4);
    lv_obj_set_y(lv_label_7, 38);
    lv_obj_add_style(lv_label_7, &small_text, 0);
    
    lv_obj_t * lv_label_8 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_8, "av 0");
    lv_obj_set_align(lv_label_8, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_8, -4);
    lv_obj_set_y(lv_label_8, 54);
    lv_obj_add_style(lv_label_8, &small_text, 0);
    
    lv_obj_t * tile_co2 = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_co2, "tile_co2");
    lv_obj_set_width(tile_co2, 108);
    lv_obj_set_height(tile_co2, 70);
    lv_obj_add_style(tile_co2, &panel, 0);
    lv_obj_t * lv_label_9 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_9, "CO2 (ppm)");
    lv_obj_set_align(lv_label_9, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_9, -10);
    lv_obj_add_style(lv_label_9, &label_text, 0);
    
    lv_obj_t * co2_value = lv_label_create(tile_co2);
    lv_obj_set_name(co2_value, "co2_value");
    lv_label_set_text(co2_value, "0");
    lv_obj_set_align(co2_value, LV_ALIGN_CENTER);
    lv_obj_set_x(co2_value, -14);
    lv_obj_set_y(co2_value, 0);
    lv_obj_add_style(co2_value, &value_text, 0);
    
    lv_obj_t * lv_label_10 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_10, "mn 0");
    lv_obj_set_align(lv_label_10, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_10, -4);
    lv_obj_set_y(lv_label_10, 22);
    lv_obj_add_style(lv_label_10, &small_text, 0);
    
    lv_obj_t * lv_label_11 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_11, "mx 0");
    lv_obj_set_align(lv_label_11, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_11, -4);
    lv_obj_set_y(lv_label_11, 38);
    lv_obj_add_style(lv_label_11, &small_text, 0);
    
    lv_obj_t * lv_label_12 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_12, "av 0");
    lv_obj_set_align(lv_label_12, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_12, -4);
    lv_obj_set_y(lv_label_12, 54);
    lv_obj_add_style(lv_label_12, &small_text, 0);
    
    lv_obj_t * tile_pressure = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_pressure, "tile_pressure");
    lv_obj_set_width(tile_pressure, 108);
    lv_obj_set_height(tile_pressure, 70);
    lv_obj_add_style(tile_pressure, &panel, 0);
    lv_obj_t * lv_label_13 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_13, "Press (hPa)");
    lv_obj_set_align(lv_label_13, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_13, -10);
    lv_obj_add_style(lv_label_13, &label_text, 0);
    
    lv_obj_t * lv_label_14 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_14, "0");
    lv_obj_set_align(lv_label_14, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_14, -14);
    lv_obj_set_y(lv_label_14, 0);
    lv_obj_add_style(lv_label_14, &value_text, 0);
    
    lv_obj_t * lv_label_15 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_15, "mn 0");
    lv_obj_set_align(lv_label_15, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_15, -4);
    lv_obj_set_y(lv_label_15, 22);
    lv_obj_add_style(lv_label_15, &small_text, 0);
    
    lv_obj_t * lv_label_16 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_16, "mx 0");
    lv_obj_set_align(lv_label_16, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_16, -4);
    lv_obj_set_y(lv_label_16, 38);
    lv_obj_add_style(lv_label_16, &small_text, 0);
    
    lv_obj_t * lv_label_17 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_17, "av 0");
    lv_obj_set_align(lv_label_17, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_17, -4);
    lv_obj_set_y(lv_label_17, 54);
    lv_obj_add_style(lv_label_17, &small_text, 0);
    
    lv_obj_t * tile_eco2 = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_eco2, "tile_eco2");
    lv_obj_set_width(tile_eco2, 108);
    lv_obj_set_height(tile_eco2, 70);
    lv_obj_add_style(tile_eco2, &panel, 0);
    lv_obj_t * lv_label_18 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_18, "eCO2 (ppm)");
    lv_obj_set_align(lv_label_18, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_18, -10);
    lv_obj_add_style(lv_label_18, &label_text, 0);
    
    lv_obj_t * lv_label_19 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_19, "0");
    lv_obj_set_align(lv_label_19, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_19, -14);
    lv_obj_set_y(lv_label_19, 0);
    lv_obj_add_style(lv_label_19, &value_text, 0);
    
    lv_obj_t * lv_label_20 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_20, "mn 0");
    lv_obj_set_align(lv_label_20, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_20, -4);
    lv_obj_set_y(lv_label_20, 22);
    lv_obj_add_style(lv_label_20, &small_text, 0);
    
    lv_obj_t * lv_label_21 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_21, "mx 0");
    lv_obj_set_align(lv_label_21, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_21, -4);
    lv_obj_set_y(lv_label_21, 38);
    lv_obj_add_style(lv_label_21, &small_text, 0);
    
    lv_obj_t * lv_label_22 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_22, "av 0");
    lv_obj_set_align(lv_label_22, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_22, -4);
    lv_obj_set_y(lv_label_22, 54);
    lv_obj_add_style(lv_label_22, &small_text, 0);
    
    lv_obj_t * tile_tvoc = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_tvoc, "tile_tvoc");
    lv_obj_set_width(tile_tvoc, 108);
    lv_obj_set_height(tile_tvoc, 70);
    lv_obj_add_style(tile_tvoc, &panel, 0);
    lv_obj_t * lv_label_23 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_23, "TVOC (ppb)");
    lv_obj_set_align(lv_label_23, LV_ALIGN_TOP_MID);
    lv_obj_set_x(lv_label_23, 0);
    lv_obj_set_y(lv_label_23, -10);
    lv_obj_add_style(lv_label_23, &label_text, 0);
    
    lv_obj_t * lv_label_24 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_24, "0");
    lv_obj_set_align(lv_label_24, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_24, -25);
    lv_obj_set_y(lv_label_24, 0);
    lv_obj_add_style(lv_label_24, &value_text, 0);
    
    lv_obj_t * lv_label_25 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_25, "mn 0");
    lv_obj_set_align(lv_label_25, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_25, -15);
    lv_obj_set_y(lv_label_25, 22);
    lv_obj_add_style(lv_label_25, &small_text, 0);
    
    lv_obj_t * lv_label_26 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_26, "mx 0");
    lv_obj_set_align(lv_label_26, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_26, -15);
    lv_obj_set_y(lv_label_26, 38);
    lv_obj_add_style(lv_label_26, &small_text, 0);
    
    lv_obj_t * lv_label_27 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_27, "av 0");
    lv_obj_set_align(lv_label_27, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_27, -15);
    lv_obj_set_y(lv_label_27, 54);
    lv_obj_add_style(lv_label_27, &small_text, 0);
    
    lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_3, 240);
    lv_obj_set_height(lv_obj_3, 34);
    lv_obj_set_align(lv_obj_3, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_x(lv_obj_3, 0);
    lv_obj_set_y(lv_obj_3, 0);
    lv_obj_add_style(lv_obj_3, &panel, 0);
    lv_obj_t * lv_label_28 = lv_label_create(lv_obj_3);
    lv_label_set_text(lv_label_28, "Status: {status message}");
    lv_obj_set_align(lv_label_28, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(lv_label_28, 6);
    lv_obj_add_style(lv_label_28, &label_text, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

