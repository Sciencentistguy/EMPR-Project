#include <lpc_types.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "libs/keypad.h"
#include "libs/lcd.h"
#include "libs/scanner/grid.h"
#include "libs/scanner/motors.h"
#include "libs/scanner/sensor.h"
#include "libs/serial.h"
#include "libs/systick_delay.h"
#include "libs/timer.h"

void task_D2_pc_coodrinate_colour() {
    serial_printf("[Task]: D2 Move and Measure\r\n");
    grid_home();

    lcd_clear_display();
    lcd_printf(0x00, "Waiting for");
    lcd_printf(0x40, "input");

    char bufs[8][8];
    memset(bufs, 0, sizeof(bufs));

    serial_read_blocking(bufs[0], 4);
    serial_read_blocking(bufs[1], 4);

    int coords[2];
    coords[0] = atoi(bufs[0]);
    coords[1] = atoi(bufs[1]);

    lcd_clear_display();
    lcd_printf(0x0, "POS: (%3d, %3d)", coords[0], coords[1]);
    grid_move_to_point(coords[0], coords[1]);

    uint16_t colours[4];
    sensor_read_all_colours(colours);
    sensor_normalize_colours(colours);

    serial_printf("[Task D2]: Sending rgb values\r\n");
    serial_printf("%i|%i|%i\r\n", colours[1], colours[2], colours[3]);
    lcd_printf(0x40, "(%3d, %3d, %3d)", colours[1], colours[2], colours[3]);

    keypad_wait_key('#', 10);
}
