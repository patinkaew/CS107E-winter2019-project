#include "bme280.h"
#include "i2c.h"
#include "printf.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static const unsigned int bme280_slave_addr = 0x76;

static void bme280_write_regs(unsigned char reg, unsigned char v) {
	char data[2] = {reg, v};
	i2c_write(bme280_slave_addr, data, 2);
}

static void bme280_read_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len){
	i2c_write(bme280_slave_addr, (void*) &reg_addr, 1);
	i2c_read(bme280_slave_addr, (void*) reg_data, len);
}

static int32_t compensate_temperature(int32_t uncomp_temp){
    int32_t var1, var2, result;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;
	return result;
}

void bme280_init(){
	i2c_init();
	uint8_t reg_data[BME280_P_T_H_DATA_LEN] = { 0 };
	bme280_read_regs(DATA, reg_data, BME280_P_T_H_DATA_LEN);

	for(int i = 0; i < BME280_P_T_H_DATA_LEN; i++){
		printf("%d\n", reg_data[i]);
	}
	printf("done\04");
}
