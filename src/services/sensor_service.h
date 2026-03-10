#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SENSOR_STATUS_UNINITIALIZED,
    SENSOR_STATUS_OK,
    SENSOR_STATUS_ERROR,
} sensor_status_t;

typedef struct {
    float humidity_rh;
    float temperature_c;
    float co2_ppm;
    sensor_status_t status;
} sensor_scd30_t;

typedef struct {
    sensor_scd30_t scd30;
    // uint64_t update_ms; // timestamp instead?
} sensor_snapshot_t;

bool sensor_service_init(void);
bool sensor_service_start(void);
void sensor_service_stop(void);
bool sensor_service_get_snapshot(sensor_snapshot_t *out);

#endif /* SENSOR_SERVICE_H */