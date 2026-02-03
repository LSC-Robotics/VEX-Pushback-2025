#include "adi.hpp"
#include "globals.h"
#include "misc.h"

void drive() {
    int LEFT_Y = ct.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
    int RIGHT_Y = ct.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);

    if (LEFT_Y != 0) {
        mgL.move(LEFT_Y);
    } else if (LEFT_Y == 0) {
        mgL.move(0);
    }

    if (RIGHT_Y != 0) {
        mgR.move(RIGHT_Y);
    } else if (RIGHT_Y == 0) {
        mgR.move(0);
    }
}

void intake(){
   // Push blocks in
   if (R1_PUSHED) {
    mtIN1.move(127);
    mtIN2.move(127);
   } else if (R1_RELEASED) {
    mtIN1.move(0);
    mtIN2.move(0);
   }

   // Push blocks out
   if (R2_PUSHED) {
    mtIN1.move(-127);
    mtIN2.move(-127);
   } else if (R2_RELEASED) {
    mtIN1.move(0);
    mtIN2.move(0);
   }

   // Launch the blocks
   if (L1_PUSHED) {
    mtIN3.move(127);
   } else if (L2_PUSHED) {
    mtIN3.move(-127);
   }

   // Raise the loader
   if (A_PUSHED) {
    pn0.extend();
    pn1.extend();
   // Retract the loader
   } else if (B_PUSHED) {
    pn0.retract();
    pn1.retract();
   }
}