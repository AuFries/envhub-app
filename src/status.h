#ifndef STATUS_SERVICE_H
#define STATUS_SERVICE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sensor_service.h"

typedef enum
{
    STATUS_SEV_INFO = 0,
    STATUS_SEV_WARNING,
    STATUS_SEV_CRITICAL,
} status_severity_t;

typedef enum
{
    STATUS_SENSOR_BQ27441 = 0,
    STATUS_SENSOR_SCD30,
    STATUS_SENSOR_BMP580,
    STATUS_SENSOR_SGP30,
    STATUS_SENSOR_COUNT
} status_sensor_id_t;

typedef struct
{
    status_sensor_id_t sensor_id;
    sensor_status_t sensor_status;
    status_severity_t severity;
    bool active;
    uint64_t since_ms;
    char summary[32];
    char detail[96];
} sensor_status_entry_t;

void status_service_init(void);

void status_service_set_sensor_status(status_sensor_id_t sensor_id, sensor_status_t sensor_status,
                                      uint64_t now_ms);

bool status_service_get_sensor_status(status_sensor_id_t sensor_id, sensor_status_entry_t *out);

size_t status_service_get_active_sensor_statuses(sensor_status_entry_t *out, size_t max_entries);

size_t status_service_get_active_count(void);
status_severity_t status_service_get_overall_severity(void);
const char *status_service_get_summary(void);

const char *status_service_sensor_name(status_sensor_id_t sensor_id);

#endif /* STATUS_SERVICE_H */