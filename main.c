#include <math.h>
#include <string.h>

#include <lpc17xx_gpio.h>

#include "libs/i2c.h"
#include "libs/serial.h"
#include "libs/pinsel.h"
#include "libs/systick_delay.h"
#include "libs/motors.h"
#include "libs/grid.h"
#include "libs/keypad.h"

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

    GPIO_IntCmd(0, 1 << 23, 1);
    NVIC_EnableIRQ(EINT3_IRQn);
    keypad_set_as_inputs();

    serial_printf("hello\r\n");

    Grid_t grid = {
        950, 950, 10, 260, 0, 0
    };

    grid_home(&grid);

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
            break;
        case '4':
            grid_x_steps(&grid, -5);
            break;
        case '2':
            grid_y_steps(&grid, 5);
            break;
        case '5':
            grid_y_steps(&grid, -5);
            break;
        case '3':
            movez(5);
            break;
        case '6':
            movez(-5);
            break;
        default:
            break;
        }
    }

    return 0;
}
