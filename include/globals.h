#include "vex.h"
#include "vex_competition.h"
#include "vex_drivetrain.h"
#include "vex_global.h"
#include "vex_motorgroup.h"
#include "vex_task.h"
#include "vex_units.h"
#include "vex_vision.h"
#include <iostream>


#ifndef GLOBALS_H
#define GLOBALS_H

using namespace vex;

extern brain br;

extern controller ct;

// Left Motors
extern motor mt1;
extern motor mt2;
extern motor mt3;

// Right Motors
extern motor mt4;
extern motor mt5;
extern motor mt6;

// Vision Sensors
extern vision vis1;

// Color Signatures
extern vision::signature redSig;

// drivetrain motor group
extern motor_group dtL;
extern motor_group dtR;

extern motor_group mgI;

extern motor mt7;
extern motor mt8;
extern motor mt9;

#endif
