#include "gpio.h"
#include "timer.h"
#include "i2c.h"
#include "printf.h"


void readSoil (void) {
    gpio_init();
    timer_init();
    uart_init();

    i2c_init();

    while(1) { 
        int value;
        value = soilChip_read_reg(0x07);
        printf ("Moisture: %d", value);
    }


}
//Read i2c fo the soil moisture sensor
//How to build upon reference implementation


//Starts the seesaw
bool begin(uint8_t addr, int8_t flow, bool reset)
{
    _i2caddr = addr;
    _flow = flow;

    if(_flow != -1) ::pinMode(_flow, INPUT);


    if(reset){
        delay(500);
    }

    uint8_t c = 8;
    if (c != SEESAW_HW_ID_CODE) {
      return false;
    }
    return true;
}

// Reads values from the first port
uint32_t digitalReadBulk(uint32_t pins)
{
    uint8_t buf[4];
    uint32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    return ret & pins;
}

// Reads values from the second port
uint32_t digitalReadBulkB(uint32_t pins)
{
    uint8_t buf[8];
    uint32_t ret = ((uint32_t)buf[4] << 24) | ((uint32_t)buf[5] << 16) | ((uint32_t)buf[6] << 8) | (uint32_t)buf[7];
    return ret & pins;
}


//reads Temperature value of the sensor
float getTemp()
{
    uint8_t buf[4];
    int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    return (1.0/(1UL << 16)) * ret;
}


