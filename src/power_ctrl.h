#ifndef POWER_CTRL_H
#define POWER_CTRL_H

#include <stdbool.h>

bool power_service_init(void);
bool power_service_start(void);
void power_service_stop(void);

bool power_service_shutdown_requested(void);
void power_service_clear_shutdown_requested(void);

bool power_service_power_cut(void);
void power_service_deinit(void);

#endif