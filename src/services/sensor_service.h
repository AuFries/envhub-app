#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H

#include <stdbool.h>
#include <stdint.h>


typedef enum {
    SENSOR_STATUS_UNINITIALIZED,
    SENSOR_STATUS_OK,
    SENSOR_STATUS_ERROR,
} sensor_status_t;

typedef enum {
    BATTERY_STATUS_UNKNOWN,
    BATTERY_STATUS_CHARGING,
    BATTERY_STATUS_DISCHARGING,
    BATTERY_STATUS_FULL,
} battery_status_t;

typedef struct {
    uint8_t capacity_percent;
    float voltage_v;
    float current_ma;
    battery_status_t status;
} fuel_gauge_bq27441_t;

typedef struct {
    float humidity_rh;
    float temperature_c;
    float co2_ppm;
    sensor_status_t status;
} sensor_scd30_t;

typedef struct {
    float temperature_c;
    float pressure_hpa;
    sensor_status_t status;
} sensor_bmp580_t;

typedef struct {
    float tvoc_ppb;
    float eco2_ppm;
    sensor_status_t status;
} sensor_sgp30_t;

typedef struct {
    fuel_gauge_bq27441_t bq27441;
    sensor_scd30_t scd30;
    sensor_bmp580_t bmp580;
    sensor_sgp30_t sgp30;
    // uint64_t update_ms; // timestamp instead?
} sensor_snapshot_t;

bool sensor_service_init(void);
bool sensor_service_start(void);
void sensor_service_stop(void);
bool sensor_service_get_snapshot(sensor_snapshot_t *out);

#endif /* SENSOR_SERVICE_H */