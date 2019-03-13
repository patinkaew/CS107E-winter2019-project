#include "tsl2561.h"
#include "strings.h"
#include "i2c.h"

const static int slave_addr = 0x39;

void tsl2561_init(){

}

void tsl2561_write_reg(unsigned char reg, unsigned char values[], size_t nbytes){
    unsigned char cmd = (1100<<4) | reg; //1100 = "cmd", "clear interrupt", "not use SMB", "not use SMB block"
    unsigned char data[nbytes + 2];
    data[0] = cmd;      //1st package = command register
    data[1] = nbytes;   //2nd package = byte count
    memcpy(data + 2, values[], nbytes);
    i2c_write(slave_addr, data, nbytes + 2);
}
