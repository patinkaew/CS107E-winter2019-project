#ifndef BME280_H
#define BME280_H

#include <stdbool.h>
#include <stdint.h>

#define SEA_LEVEL_PRESSURE 10

enum reg_addr{
    //chip & measurement control
    CHIP_ID     = 0xd0;
    RESET       = 0xe0;
    STATUS      = 0xf3;
    CONFIG      = 0xf5;
    CTRL_HUM    = 0xf2;
    CTRL_MEAS   = 0xf4;

    //raw data
    PRESS_MSB   = 0xf7;
    PRESS_LSB   = 0xf8;
    PRESS_XLSB  = 0xf9;
    TEMP_MSB    = 0xfa;
    TEMP_LSB    = 0xfb;
    TEMP_XLSB   = 0xfc;
    HUM_MSB     = 0xfd;
    HUM_LSB     = 0xfe;

    //calibration
    DIG_T1      = 0x88;
    DIG_T2      = 0x8a;
    DIG_T3      = 0x8c;
    DIG_P1      = 0x8e;
    DIG_P2      = 0x90;
    DIG_P3      = 0x92;
    DIG_P4      = 0x94;
    DIG_P5      = 0x96;
    DIG_P6      = 0x98;
    DIG_P7      = 0x9a;
    DIG_P8      = 0x9c;
    DIG_P9      = 0x9e;
    DIG_H1      = 0xa1;
    DIG_H2      = 0xe1;
    DIG_H3      = 0xe3;
    DIG_H4      = 0xe4;
    DIG_H5      = 0xe5;
    DIG_H6      = 0xe7;
};

enum write_value{
    SOFT_RESET_VAL          = 0xb6;

    OVERSAMPLING_SKIP       = 0b000;
    OVERSAMPLING_X1         = 0b001;
    OVERSAMPLING_X2         = 0b010;
    OVERSAMPLING_X4         = 0b011;
    OVERSAMPLING_X8         = 0b100;
    OVERSAMPLING_X16        = 0b101;

    MODE_SLEEP              = 0b00;
    MODE_FORCED1            = 0b01;
    MODE_FORCED2            = 0b10;
    MODE_NORMAL             = 0b11;
};

struct uncomp_data{
    int32_t temperature;
    int32_t pressure;
    int32_t humidity;
};

struct calib_data{
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
    int32_t t_fine;
};

void bme280_init();

void bme280_enable();

void bme280_disable();

bool bme280_status();

/*float bme280_get_temperature_c();

float bme280_get_temperature_f();

float bme280_get_humidity();

float bme280_get_pressure();

float bme280_get_altitude(); */

#endif BME280_H
