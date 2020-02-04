#pragma once

#include <lpc_types.h>

#define MOTOR_MIN_DELAY 5
#define MOTOR_MAX_DELAY 30
#define MOTOR_RAMP 2

#define MOTOR_XY_LATCH_ADDRESS 0x3E
#define MOTOR_ZPEN_LATCH_ADDRESS 0x3F

#define MOTOR_STEPX_1 0x09
#define MOTOR_STEPX_2 0x05
#define MOTOR_STEPX_3 0x06
#define MOTOR_STEPX_4 0x0A
#define MOTOR_STEPX \
    { MOTOR_STEPX_1, MOTOR_STEPX_2, MOTOR_STEPX_3, MOTOR_STEPX_4 }

#define MOTOR_STEPY_1 0x90
#define MOTOR_STEPY_2 0x50
#define MOTOR_STEPY_3 0x60
#define MOTOR_STEPY_4 0xA0
#define MOTOR_STEPY \
    { MOTOR_STEPY_1, MOTOR_STEPY_2, MOTOR_STEPY_3, MOTOR_STEPY_4 }

#define MOTOR_STEPZ \
    { MOTOR_STEPX_4, MOTOR_STEPX_3, MOTOR_STEPX_2, MOTOR_STEPX_1 }

// #define MOTOR_STEPZ MOTOR_STEPX

#define SWITCH_ADDRESS 0x3C
#define SWITCH_X_MASK 0x02
#define SWITCH_Y_MASK 0x01
#define SWITCH_Z_MASK 0x04

typedef struct Motor_t {
    uint8_t address;
    uint8_t steps[4];
    uint8_t mask;
    uint8_t step;
    uint8_t step_scaler;
} Motor_t;

/**
 * N.B. Motors use timer 1
 */
void motor_init();
uint8_t motor_moving();
void motor_wake();
void motor_sleep();
uint8_t motor_get_move(Motor_t *motor, uint8_t direction);
void motor_set(int x_steps, int y_steps, int z_steps);

void setup_switches();
int check_switch(uint8_t mask);
int home(Motor_t *motor);
int home_x();
int home_y();
int home_z();
void send_move(Motor_t *motor, uint8_t direction);
int move(Motor_t *motor, int steps);
int movex(int steps);
int movey(int steps);
int movez(int steps);
void movexy_together(int xsteps, int ysteps);