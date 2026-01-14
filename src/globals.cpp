#include "globals.h"
#include "vex_motor.h"
#include "vex_motorgroup.h"

// Brain
brain br;

// Controller
controller ct;

// LEFT MOTORS
motor mt1 = motor(PORT16, true);
motor mt2 = motor(PORT15, true);
motor mt3 = motor(PORT11, true); // Back Motors

// RIGHT MOTORS
motor mt4 = motor(PORT18);
motor mt5 = motor(PORT17);
motor mt6 = motor(PORT12); // Back Motors

// INTAKE
motor mt7 = motor(PORT1, true);
motor mt8 = motor(PORT19);
motor mt9 = motor(PORT20);


// Vision Sensor
vision vis1 = vision(PORT7);

// Color Signatures
vision::signature redSig =
    vision::signature(1, 14769, 15277, 15023, 697, 1137, 917, 11743288, 0);

// Motor Groups
motor_group dtL = motor_group(mt1, mt2);
motor_group dtR = motor_group(mt4, mt5);
motor_group mgI = motor_group(mt7, mt8);
