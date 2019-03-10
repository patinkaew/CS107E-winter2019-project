#include "bme280.h"
#include "i2c.h"
#include <stdbool.h>

static bool is_on = false;

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
