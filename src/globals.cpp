#include "globals.h"
#include "vex_motorgroup.h"

// Brain
brain br;

// LEFT MOTORS
motor mt1 = motor(PORT1, false);
motor mt2 = motor(PORT2, false);
motor mt3 = motor(PORT3, true); // Back Motors //needs to be spinning in opposite direction...false reflects that

// RIGHT MOTORS
motor mt4 = motor(PORT11, true);
motor mt5 = motor(PORT12, true);
motor mt6 = motor(PORT13, false); // Back Motors //needs to be spinning in opposite direction...false reflects that

// Vision Sensor
vision vis1 = vision(PORT19);

// Color Signatures
vision::signature redSig = vision::signature(1, 14769, 15277, 15023, 697, 1137, 917, 11743288, 0);

// Motor Groups
motor_group mgL = motor_group(mt1, mt2);
motor_group mgR = motor_group(mt4, mt5);
