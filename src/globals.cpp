#include "globals.h"

// LEFT MOTORS
motor mt1 = motor(PORT1);
motor mt2 = motor(PORT2);
motor mt3 = motor(PORT3);

// RIGHT MOTORS
motor mt4 = motor(PORT11);
motor mt5 = motor(PORT12);
motor mt6 = motor(PORT13);

// Vision Sensor
vision vis1 = vision(PORT19);

// Color Signatures
vision::signature redSig = vision::signature(1, 14769, 15277, 15023, 697, 1137, 917, 11743288, 0);

// Motor Groups
motor_group mgL = motor_group(mt1, mt2, mt3);
motor_group mgR = motor_group(mt4, mt5, mt6);

