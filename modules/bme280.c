#include "bme280.h"
#include "i2c.h"
#include <stdbool.h>
#include <stdint.h>

static const unsigned bme280_addr = 0x76;
static bool is_on = false;


static void bme280_write_reg(unsigned char reg, unsigned char v) {
	char data[2] = {reg, v};
	i2c_write(bme280_addr, data, 2);
}

static unsigned char bme280_read_reg(unsigned char reg) {
	i2c_write(bme280_addr, (void*) &reg, 1);
	unsigned char value = 0;
	i2c_read(lbme280_addr, (void*) &value, 1);
	return value;
}

static int32_t compensate_temperature(int32_t uncomp_temp){
    int32_t var1, var2, result;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

}

void bme280_init(){

}

void bme280_enable(){
    is_on = true;
}

void bme280_disable(){
    is_on = false;
}

bool bme280_status(){
    return is_on;
}
