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
void ui_envhub_set_time_text(const char *s);

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