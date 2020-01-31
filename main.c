#include <math.h>
#include <string.h>
#include <stdio.h>
#include <lpc17xx_gpio.h>

#include "libs/i2c.h"
#include "libs/serial.h"
#include "libs/pinsel.h"
#include "libs/systick_delay.h"
#include "libs/motors.h"
#include "libs/grid.h"
#include "libs/keypad.h"
#include "libs/lcd.h"

volatile uint8_t keypad_pressed_flag = 0;
void EINT3_IRQHandler() {
    if (GPIO_GetIntStatus(KEYPAD_INT_PORT, KEYPAD_INT_PIN, KEYPAD_INT_EDGE)) {
        GPIO_ClearInt(KEYPAD_INT_PORT, 1 << KEYPAD_INT_PIN);
        serial_printf("keypad int\r\n");
        keypad_pressed_flag = 1;
    }
}


int main() {
    serial_init();
    systick_init();
    i2c_init();
    setup_switches();
    lcd_init();
    
    GPIO_IntCmd(0, 1 << 23, 1);
    NVIC_EnableIRQ(EINT3_IRQn);
    keypad_set_as_inputs();

    serial_printf("hello\r\n");
    lcd_clear_display();
    Grid_t grid = {
        700, 700, 10, 260, 0, 0
    };

    grid_home(&grid);
    int co_x = 0, co_y = 0, co_z = 0;
    
    keypad_pressed_flag = 0;
    while (1) {
        if (keypad_pressed_flag == 0) {
            continue;
        }

        char k = keypad_read();
        switch (k)
        {
        case '1':
            grid_x_steps(&grid, 5);
            if (co_x != 900){
                co_x = co_x + 5;
            }
            break;
        case '4':
            grid_x_steps(&grid, -5);
            if (co_x != 0){
                co_x = co_x - 5;
            }
            break;
        case '2':
            grid_y_steps(&grid, 5);
            if (co_y != 900){
                co_y = co_y + 5;
            }
            break;
        case '5':
            grid_y_steps(&grid, -5);
            if (co_y != 0){
                co_y = co_y - 5;
            }
            break;
        case '3':
            movez(5);
            if (co_z != 900){
                co_z = co_z + 5;
            }
            break;
        case '6':
            movez(-5);
            if (co_z != 0){
                co_z = co_z - 5;
            }
            break;
        default:
            break;
        }
        char co_send[15];
        lcd_clear_display();
        sprintf(co_send,"%i,%i,%i,co_x,co_y,co_z);
        lcd_send_string(co_send,0x00);
                
    }

    return 0;
}
