#include "modules/waterlevel.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "printf.h"

void main(void){
    gpio_init();
    uart_init();
    timer_init();
    waterlevel_init(GPIO_PIN3, GPIO_PIN2);
    waterlevel_calibrate_tank_height();
    printf("tank_height: %d %\n", waterlevel_get_tank_height());
    timer_delay(2);
    while(1){
        printf("level: %d percent: %d %\n", waterlevel_get_level(), waterlevel_get_percent());
        timer_delay_ms(250);
    }
}
