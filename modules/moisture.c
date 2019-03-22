#include "gpio.c"
#include "timer.c"
#include "i2c.c"


void main (void) {

}
//Read i2c fo the soil moisture sensor
//How to build upon reference implementation


//Starts the seesaw
bool begin(uint8_t addr, int8_t flow, bool reset)
{
    _i2caddr = addr;
    _flow = flow;

    if(_flow != -1) ::pinMode(_flow, INPUT);

    _i2c_init();

    if(reset){
        SWReset();
        delay(500);
    }

    uint8_t c = this->read8(SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID);
    if (c != SEESAW_HW_ID_CODE) {
      return false;
    }
    return true;
}

// Reads values from the first port
uint32_t digitalReadBulk(uint32_t pins)
{
    uint8_t buf[4];
    this->read(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK, buf, 4);
    uint32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    return ret & pins;
}

// Reads values from the second port
uint32_t digitalReadBulkB(uint32_t pins)
{
    uint8_t buf[8];
    this->read(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK, buf, 8);
    uint32_t ret = ((uint32_t)buf[4] << 24) | ((uint32_t)buf[5] << 16) | ((uint32_t)buf[6] << 8) | (uint32_t)buf[7];
    return ret & pins;
}


// Reads I2C Value for the Seesaw
uint8_t getI2CAddr()
{
  return this->read8(SEESAW_EEPROM_BASE, SEESAW_EEPROM_I2C_ADDR);
}


//reads Temperature value of the sensor
float getTemp()
{
    uint8_t buf[4];
    this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP, buf, 4, 1000);
    int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    return (1.0/(1UL << 16)) * ret;
}


