/**
 * @file ui_envhub.h
 */

#ifndef UI_ENVHUB_H
#define UI_ENVHUB_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "ui_envhub_gen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void ui_envhub_init(void);

void ui_envhub_set_time_text(const char *s);
void ui_envhub_set_scd30(float co2_ppm, float temp_c, float humidity_rh);
void ui_envhub_set_bmp580(float pressure_hpa, float temperature_c);
void ui_envhub_set_sgp30(float eco2_ppm, float tvoc_ppb);
void ui_envhub_set_bq27441(uint8_t capacity_percent,float voltage_v,float current_ma);


/**
 * Initialize the component library
 */
void ui_envhub_init(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_ENVHUB_H*/