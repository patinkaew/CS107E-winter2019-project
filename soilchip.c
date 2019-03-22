#include "timer.h"
#include "printf.h"
#include "i2c.h"
#include "soilchip.h"

const unsigned soilChip_address = 0x36; 

void soilChip_write_reg(unsigned char reg, unsigned char v) {
	char data[2] = {reg, v};
	i2c_write(soilChip_address, data, 2);
}

unsigned soilChip_read_reg(unsigned char reg) {
	i2c_write(soilChip_address, (void*) &reg, 1);
	unsigned char uc = 0;
	i2c_read(soilChip_address, (void*) &uc, 1);
	return uc;
}
