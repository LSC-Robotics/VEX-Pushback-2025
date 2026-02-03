#include "main.h"

using namespace pros;

#pragma once

// Master Controller
extern Controller ct;

// LEFT MOTORS
extern Motor mtL1;
extern Motor mtL2;
extern Motor mtL3;

// RIGHT MOTORS
extern Motor mtR1;
extern Motor mtR2;
extern Motor mtR3;

// INTAKE MOTORS
extern Motor mtIN1;
extern Motor mtIN2;
extern Motor mtIN3;
extern Motor mtIN4;

// MOTOR GROUPS
extern MotorGroup mgL;
extern MotorGroup mgR;
extern MotorGroup mgIN;

// PNEUMATICS
extern adi::Pneumatics pn0;
extern adi::Pneumatics pn1;
extern adi::Pneumatics pn2;
extern adi::Pneumatics pn4;

// FUNCTIONS
extern void drive();
extern void intake();

// BASIC VARIABLES
extern bool R1_PUSHED;
extern bool R1_RELEASED;

extern bool R2_PUSHED;
extern bool R2_RELEASED;

extern bool L1_PUSHED;
extern bool L1_RELEASED;

extern bool L2_PUSHED;
extern bool L2_RELEASED;

extern bool A_PUSHED;
extern bool B_PUSHED;
extern bool X_PUSHED;
extern bool Y_PUSHED;