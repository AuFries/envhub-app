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
#include "sensors.h"
#include "system_usage.h"
#include "status.h"

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/
    typedef struct
    {
        sensor_snapshot_t sensors;
        system_usage_t system;
    } ui_envhub_snapshot_t;

    /**********************
     * GLOBAL VARIABLES
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/

    void ui_envhub_init(void);

    void ui_envhub_set_status_summary(status_severity_t severity, const char *text);
    void ui_envhub_show_shutdown_popup(void);

    void ui_envhub_set_time_text(const char *s);

    void ui_envhub_update_snapshot(const ui_envhub_snapshot_t *snapshot);

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* UI_ENVHUB_H */