#include <lpc_types.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/keypad.h"
#include "../libs/lcd.h"
#include "../libs/scanner/grid.h"
#include "../libs/scanner/motors.h"
#include "../libs/scanner/sensor.h"
#include "../libs/serial.h"
#include "../libs/systick_delay.h"
#include "../libs/timer.h"

void task_D2_pc_coodrinate_colour() {
    lcd_clear_display();
    char buf[32];
    while (strcmp(buf, "hello") != 0) {
        memset(buf, 0, sizeof(buf));
        serial_read_blocking(buf, 5);
    }
    char bufs[8][8];
    memset(bufs, 0, sizeof(bufs));

    serial_read_blocking(bufs[0], 4);
    serial_read_blocking(bufs[1], 4);

    int coords[2];
    coords[0] = atoi(bufs[0]);
    coords[1] = atoi(bufs[1]);
    lcd_printf(0x0, "%i - %i", coords[0], coords[1]);
    grid_home();
    grid_move_to_point(coords[0], coords[1]);
    uint16_t rgb[3];
    memset(rgb, 0, sizeof(rgb));
    sensor_read_rgb(rgb, rgb + 1, rgb + 2);
    serial_printf("[Task D2]: Sending rgb values\r\n");
    serial_printf("%i|%i|%i\r\n", rgb[0], rgb[1], rgb[2]);
    for (;;)
        ;
    // TODO - go to coordinates, read rgb, send that to python.
}
