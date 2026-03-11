#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <stdbool.h>
#include <stdint.h>

#include "sensor_service.h"
#include "status_service.h"

bool data_logger_init(void);
void data_logger_shutdown(void);

bool data_logger_log_bq27441(uint64_t monotonic_ms,
                             const fuel_gauge_bq27441_t *sample);

bool data_logger_log_scd30(uint64_t monotonic_ms,
                           const sensor_scd30_t *sample);

bool data_logger_log_bmp580(uint64_t monotonic_ms,
                            const sensor_bmp580_t *sample);

bool data_logger_log_sgp30(uint64_t monotonic_ms,
                           const sensor_sgp30_t *sample);

bool data_logger_log_snapshot(uint64_t monotonic_ms,
                              const sensor_snapshot_t *snapshot);

bool data_logger_log_status_event(uint64_t monotonic_ms,
                                  status_sensor_id_t sensor_id,
                                  sensor_status_t old_status,
                                  sensor_status_t new_status,
                                  const char *message);

#endif /* DATA_LOGGER_H */