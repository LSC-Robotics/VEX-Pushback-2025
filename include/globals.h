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

// MOTOR GROUPS
extern MotorGroup mgL;
extern MotorGroup mgR;
extern MotorGroup mgIN;

// FUNCTIONS
extern void drive();
extern void intake();