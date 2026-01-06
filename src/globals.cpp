#include "globals.h"
#include "vex_motor.h"
#include "vex_motorgroup.h"

// Brain
brain br;

// LEFT MOTORS
motor mt1 = motor(PORT11);
motor mt2 = motor(PORT12);
motor mt3 = motor(PORT13); // Back Motors

// RIGHT MOTORS
motor mt4 = motor(PORT14);
motor mt5 = motor(PORT15);
motor mt6 = motor(PORT16); // Back Motors

// INTAKE
motor mt7 = motor(PORT19);
motor mt8 = motor(PORT20);

// Vision Sensor
vision vis1 = vision(PORT19);

// Color Signatures
vision::signature redSig =
    vision::signature(1, 14769, 15277, 15023, 697, 1137, 917, 11743288, 0);

// Motor Groups
motor_group dtL = motor_group(mt1, mt2);
motor_group dtR = motor_group(mt4, mt5);
motor_group mgI = motor_group(mt7, mt8);
