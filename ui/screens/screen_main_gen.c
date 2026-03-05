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
        lv_style_set_border_color(&panel, lv_color_hex(0x224455));
        lv_style_set_radius(&panel, 12);
        lv_style_set_pad_left(&panel, 8);
        lv_style_set_pad_right(&panel, 8);
        lv_style_set_pad_top(&panel, 6);
        lv_style_set_pad_bottom(&panel, 6);

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
    lv_obj_set_height(lv_obj_1, 34);
    lv_obj_set_align(lv_obj_1, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(lv_obj_1, 2);
    lv_obj_set_y(lv_obj_1, 0);
    lv_obj_add_style(lv_obj_1, &panel, 0);
    lv_obj_t * lv_label_0 = lv_label_create(lv_obj_1);
    lv_label_set_text(lv_label_0, "Wall Time");
    lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
    lv_obj_add_style(lv_label_0, &value_text, 0);
    
    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_2, 80);
    lv_obj_set_height(lv_obj_2, 34);
    lv_obj_set_align(lv_obj_2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_obj_2, -2);
    lv_obj_set_y(lv_obj_2, 0);
    lv_obj_add_style(lv_obj_2, &panel, 0);
    lv_obj_t * lv_label_1 = lv_label_create(lv_obj_2);
    lv_label_set_text(lv_label_1, "🔋 ##%");
    lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
    lv_obj_add_style(lv_label_1, &value_text, 0);
    
    lv_obj_t * ui_tiles_wrap = lv_obj_create(lv_obj_0);
    lv_obj_set_name(ui_tiles_wrap, "ui_tiles_wrap");
    lv_obj_set_width(ui_tiles_wrap, 240);
    lv_obj_set_height(ui_tiles_wrap, 230);
    lv_obj_set_align(ui_tiles_wrap, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(ui_tiles_wrap, 0);
    lv_obj_set_y(ui_tiles_wrap, 44);
    lv_obj_t * tile_temp = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_temp, "tile_temp");
    lv_obj_set_width(tile_temp, 110);
    lv_obj_set_height(tile_temp, 70);
    lv_obj_add_style(tile_temp, &panel, 0);
    lv_obj_t * lv_label_2 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_2, "Temperature");
    lv_obj_set_align(lv_label_2, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_2, 2);
    lv_obj_add_style(lv_label_2, &label_text, 0);
    
    lv_obj_t * lv_label_3 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_3, "0°F");
    lv_obj_set_align(lv_label_3, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_3, -14);
    lv_obj_set_y(lv_label_3, 6);
    lv_obj_add_style(lv_label_3, &value_text, 0);
    
    lv_obj_t * lv_label_4 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_4, "mn 0");
    lv_obj_set_align(lv_label_4, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_4, -4);
    lv_obj_set_y(lv_label_4, 22);
    lv_obj_add_style(lv_label_4, &small_text, 0);
    
    lv_obj_t * lv_label_5 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_5, "mx 0");
    lv_obj_set_align(lv_label_5, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_5, -4);
    lv_obj_set_y(lv_label_5, 38);
    lv_obj_add_style(lv_label_5, &small_text, 0);
    
    lv_obj_t * lv_label_6 = lv_label_create(tile_temp);
    lv_label_set_text(lv_label_6, "av 0");
    lv_obj_set_align(lv_label_6, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_6, -4);
    lv_obj_set_y(lv_label_6, 54);
    lv_obj_add_style(lv_label_6, &small_text, 0);
    
    lv_obj_t * tile_humidity = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_humidity, "tile_humidity");
    lv_obj_set_width(tile_humidity, 110);
    lv_obj_set_height(tile_humidity, 70);
    lv_obj_add_style(tile_humidity, &panel, 0);
    lv_obj_t * lv_label_7 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_7, "Humidity");
    lv_obj_set_align(lv_label_7, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_7, 2);
    lv_obj_add_style(lv_label_7, &label_text, 0);
    
    lv_obj_t * lv_label_8 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_8, "0%");
    lv_obj_set_align(lv_label_8, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_8, -14);
    lv_obj_set_y(lv_label_8, 6);
    lv_obj_add_style(lv_label_8, &value_text, 0);
    
    lv_obj_t * lv_label_9 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_9, "mn 0");
    lv_obj_set_align(lv_label_9, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_9, -4);
    lv_obj_set_y(lv_label_9, 22);
    lv_obj_add_style(lv_label_9, &small_text, 0);
    
    lv_obj_t * lv_label_10 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_10, "mx 0");
    lv_obj_set_align(lv_label_10, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_10, -4);
    lv_obj_set_y(lv_label_10, 38);
    lv_obj_add_style(lv_label_10, &small_text, 0);
    
    lv_obj_t * lv_label_11 = lv_label_create(tile_humidity);
    lv_label_set_text(lv_label_11, "av 0");
    lv_obj_set_align(lv_label_11, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_11, -4);
    lv_obj_set_y(lv_label_11, 54);
    lv_obj_add_style(lv_label_11, &small_text, 0);
    
    lv_obj_t * tile_co2 = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_co2, "tile_co2");
    lv_obj_set_width(tile_co2, 110);
    lv_obj_set_height(tile_co2, 70);
    lv_obj_add_style(tile_co2, &panel, 0);
    lv_obj_t * lv_label_12 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_12, "CO2");
    lv_obj_set_align(lv_label_12, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_12, 2);
    lv_obj_add_style(lv_label_12, &label_text, 0);
    
    lv_obj_t * lv_label_13 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_13, "0 ppm");
    lv_obj_set_align(lv_label_13, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_13, -14);
    lv_obj_set_y(lv_label_13, 6);
    lv_obj_add_style(lv_label_13, &value_text, 0);
    
    lv_obj_t * lv_label_14 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_14, "mn 0");
    lv_obj_set_align(lv_label_14, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_14, -4);
    lv_obj_set_y(lv_label_14, 22);
    lv_obj_add_style(lv_label_14, &small_text, 0);
    
    lv_obj_t * lv_label_15 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_15, "mx 0");
    lv_obj_set_align(lv_label_15, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_15, -4);
    lv_obj_set_y(lv_label_15, 38);
    lv_obj_add_style(lv_label_15, &small_text, 0);
    
    lv_obj_t * lv_label_16 = lv_label_create(tile_co2);
    lv_label_set_text(lv_label_16, "av 0");
    lv_obj_set_align(lv_label_16, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_16, -4);
    lv_obj_set_y(lv_label_16, 54);
    lv_obj_add_style(lv_label_16, &small_text, 0);
    
    lv_obj_t * tile_pressure = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_pressure, "tile_pressure");
    lv_obj_set_width(tile_pressure, 110);
    lv_obj_set_height(tile_pressure, 70);
    lv_obj_add_style(tile_pressure, &panel, 0);
    lv_obj_t * lv_label_17 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_17, "Pressure");
    lv_obj_set_align(lv_label_17, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_17, 2);
    lv_obj_add_style(lv_label_17, &label_text, 0);
    
    lv_obj_t * lv_label_18 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_18, "0 hPa");
    lv_obj_set_align(lv_label_18, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_18, -14);
    lv_obj_set_y(lv_label_18, 6);
    lv_obj_add_style(lv_label_18, &value_text, 0);
    
    lv_obj_t * lv_label_19 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_19, "mn 0");
    lv_obj_set_align(lv_label_19, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_19, -4);
    lv_obj_set_y(lv_label_19, 22);
    lv_obj_add_style(lv_label_19, &small_text, 0);
    
    lv_obj_t * lv_label_20 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_20, "mx 0");
    lv_obj_set_align(lv_label_20, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_20, -4);
    lv_obj_set_y(lv_label_20, 38);
    lv_obj_add_style(lv_label_20, &small_text, 0);
    
    lv_obj_t * lv_label_21 = lv_label_create(tile_pressure);
    lv_label_set_text(lv_label_21, "av 0");
    lv_obj_set_align(lv_label_21, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_21, -4);
    lv_obj_set_y(lv_label_21, 54);
    lv_obj_add_style(lv_label_21, &small_text, 0);
    
    lv_obj_t * tile_eco2 = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_eco2, "tile_eco2");
    lv_obj_set_width(tile_eco2, 110);
    lv_obj_set_height(tile_eco2, 70);
    lv_obj_add_style(tile_eco2, &panel, 0);
    lv_obj_t * lv_label_22 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_22, "eCO2");
    lv_obj_set_align(lv_label_22, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_22, 2);
    lv_obj_add_style(lv_label_22, &label_text, 0);
    
    lv_obj_t * lv_label_23 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_23, "0 ppm");
    lv_obj_set_align(lv_label_23, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_23, -14);
    lv_obj_set_y(lv_label_23, 6);
    lv_obj_add_style(lv_label_23, &value_text, 0);
    
    lv_obj_t * lv_label_24 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_24, "mn 0");
    lv_obj_set_align(lv_label_24, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_24, -4);
    lv_obj_set_y(lv_label_24, 22);
    lv_obj_add_style(lv_label_24, &small_text, 0);
    
    lv_obj_t * lv_label_25 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_25, "mx 0");
    lv_obj_set_align(lv_label_25, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_25, -4);
    lv_obj_set_y(lv_label_25, 38);
    lv_obj_add_style(lv_label_25, &small_text, 0);
    
    lv_obj_t * lv_label_26 = lv_label_create(tile_eco2);
    lv_label_set_text(lv_label_26, "av 0");
    lv_obj_set_align(lv_label_26, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_26, -4);
    lv_obj_set_y(lv_label_26, 54);
    lv_obj_add_style(lv_label_26, &small_text, 0);
    
    lv_obj_t * tile_tvoc = lv_obj_create(ui_tiles_wrap);
    lv_obj_set_name(tile_tvoc, "tile_tvoc");
    lv_obj_set_width(tile_tvoc, 110);
    lv_obj_set_height(tile_tvoc, 70);
    lv_obj_add_style(tile_tvoc, &panel, 0);
    lv_obj_t * lv_label_27 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_27, "TVOC");
    lv_obj_set_align(lv_label_27, LV_ALIGN_TOP_MID);
    lv_obj_set_y(lv_label_27, 2);
    lv_obj_add_style(lv_label_27, &label_text, 0);
    
    lv_obj_t * lv_label_28 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_28, "0 ppb");
    lv_obj_set_align(lv_label_28, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_28, -14);
    lv_obj_set_y(lv_label_28, 6);
    lv_obj_add_style(lv_label_28, &value_text, 0);
    
    lv_obj_t * lv_label_29 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_29, "mn 0");
    lv_obj_set_align(lv_label_29, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_29, -4);
    lv_obj_set_y(lv_label_29, 22);
    lv_obj_add_style(lv_label_29, &small_text, 0);
    
    lv_obj_t * lv_label_30 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_30, "mx 0");
    lv_obj_set_align(lv_label_30, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_30, -4);
    lv_obj_set_y(lv_label_30, 38);
    lv_obj_add_style(lv_label_30, &small_text, 0);
    
    lv_obj_t * lv_label_31 = lv_label_create(tile_tvoc);
    lv_label_set_text(lv_label_31, "av 0");
    lv_obj_set_align(lv_label_31, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_x(lv_label_31, -4);
    lv_obj_set_y(lv_label_31, 54);
    lv_obj_add_style(lv_label_31, &small_text, 0);
    
    lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_3, 240);
    lv_obj_set_height(lv_obj_3, 34);
    lv_obj_set_align(lv_obj_3, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_x(lv_obj_3, 0);
    lv_obj_set_y(lv_obj_3, 0);
    lv_obj_add_style(lv_obj_3, &panel, 0);
    lv_obj_t * lv_label_32 = lv_label_create(lv_obj_3);
    lv_label_set_text(lv_label_32, "Status: {status message}");
    lv_obj_set_align(lv_label_32, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(lv_label_32, 6);
    lv_obj_add_style(lv_label_32, &label_text, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

