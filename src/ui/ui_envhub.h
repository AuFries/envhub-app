/**
 * @file ui_envhub.h
 */

#ifndef UI_ENVHUB_H
#define UI_ENVHUB_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

#include "ui_envhub_gen.h"

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    typedef enum
    {
        UI_STATUS_SEV_INFO = 0,
        UI_STATUS_SEV_WARNING,
        UI_STATUS_SEV_CRITICAL,
    } ui_status_severity_t;

    typedef struct
    {
        float co2_ppm;
        float temperature_c;
        float humidity_rh;
    } ui_scd30_data_t;

    typedef struct
    {
        uint8_t capacity_percent;
        float voltage_v;
        float current_ma;
    } ui_bq27441_data_t;

    typedef struct
    {
        float temperature_c;
        float pressure_hpa;
    } ui_bmp580_data_t;

    typedef struct
    {
        float tvoc_ppb;
        float eco2_ppm;
    } ui_sgp30_data_t;

    /**********************
     * GLOBAL VARIABLES
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/

    void ui_envhub_init(void);

    void ui_envhub_set_status_summary(ui_status_severity_t severity, const char *text);
    void ui_envhub_show_shutdown_popup(void);

    void ui_envhub_set_time_text(const char *s);

    void ui_envhub_set_scd30(const ui_scd30_data_t *scd30);
    void ui_envhub_set_bq27441(const ui_bq27441_data_t *bq27441);
    void ui_envhub_set_bmp580(const ui_bmp580_data_t *bmp580);
    void ui_envhub_set_sgp30(const ui_sgp30_data_t *sgp30);

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* UI_ENVHUB_H */