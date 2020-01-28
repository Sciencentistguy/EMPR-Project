#include <lpc17xx_gpio.h>

#include <math.h>
#include <string.h>

#include "libs/i2c.h"
#include "libs/lcd.h"
#include "libs/keypad.h"
#include "libs/serial.h"
#include "libs/pinsel.h"
#include "libs/systick_delay.h"
#include "libs/motors.h"
#include "libs/grid.h"

uint8_t menu[64][32];
uint8_t menu_index;

volatile uint8_t keypad_pressed_flag = 0;
volatile uint32_t adc_val;
volatile uint8_t read = 0;

void menu_init() {
    memset(menu, 0, sizeof(menu));
}

void menu_add_option(char* name, uint8_t index) {
    strcpy(menu[index], name);
}

void menu_draw(int8_t position) {
    position = position < 0 ? 0 : position;
    menu_index = position;
    serial_printf("menu drawn at %i\r\n", position);
    lcd_clear_display();
    lcd_printf(0x00, "^");
    lcd_printf(0x40, "}");
    lcd_printf(0x00 + 2, "%s", menu[position]);
    lcd_printf(0x40 + 2, "%s", menu[position + 1]);
}

void EINT3_IRQHandler() {
    if (GPIO_GetIntStatus(KEYPAD_INT_PORT, KEYPAD_INT_PIN, KEYPAD_INT_EDGE)) {
        GPIO_ClearInt(KEYPAD_INT_PORT, 1 << KEYPAD_INT_PIN);
        serial_printf("keypad int\r\n");
        keypad_pressed_flag = 1;
    }
}

int main() {
    serial_init();
    i2c_init();
    lcd_init();
    menu_init();
    systick_init();
    serial_printf("hello\r\n");
    GPIO_IntCmd(0, 1 << 23, 1);
    NVIC_EnableIRQ(EINT3_IRQn);
    keypad_set_as_inputs();
    menu_add_option("Opt1", 0);
    menu_add_option("Opt2", 1);
    menu_add_option("Opt3", 2);
    menu_add_option("Opt4", 3);
    menu_add_option("Opt5", 4);
    menu_draw(0);
    keypad_pressed_flag = 0;
    systick_delay_flag_init(5);

    for (;;) {
        if (keypad_pressed_flag == 0) {
            continue;
        }

        if (systick_flag() == 0)
            continue;

        char k = keypad_read();
        serial_printf("Read '%c'\r\n", k);

        switch (k) {
            case 'A':
                menu_draw(menu_index - 1);
                break;

            case 'B':
                menu_draw(menu_index + 1);
                break;

            default:
                break;
        }

        keypad_set_as_inputs();
        systick_delay_flag_reset();
        keypad_pressed_flag = 0;
    }
}
