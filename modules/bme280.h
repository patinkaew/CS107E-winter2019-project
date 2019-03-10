#ifndef BME280_H
#define BME280_H

#include<stdbool.h>

#define SEA_LEVEL_PRESSURE 10

void bme280_init();

void bme280_enable();

void bme280_disable();

bool bme280_status();

float bme280_get_temperature_c();

float bme280_get_temperature_f();

float bme280_get_humidity();

float bme280_get_pressure();

float bme280_get_altitude();

#endif BME280_H
