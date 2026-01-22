#include "globals.h"

void drive() {
    int dir = ct.get_analog(ANALOG_LEFT_Y); 
	int turn = ct.get_analog(ANALOG_RIGHT_X);

    bool IF_LEFT_CHANGE = (ct.get_analog(ANALOG_LEFT_Y) != 0);
    bool IF_RIGHT_CHANGE = (ct.get_analog(ANALOG_RIGHT_X) != 0);

    bool RIGHT_GREATER = (ct.get_analog(ANALOG_RIGHT_X) > 0);
    bool RIGHT_LESSER = (ct.get_analog(ANALOG_RIGHT_X) < 0);


    if(IF_LEFT_CHANGE) {
        mgL.move(ct.get_analog(ANALOG_LEFT_Y));
        mgR.move(ct.get_analog(ANALOG_LEFT_Y));
    } else {
        mgL.move(0);
        mgR.move(0);
    }

    int X_LR = ct.get_analog(ANALOG_RIGHT_X);

    if(IF_RIGHT_CHANGE) {
        if(RIGHT_GREATER) {
            mgL.move(X_LR);
        } else if (RIGHT_LESSER) {
            mgR.move(-1 * X_LR);
        }
    }

}

void intake(){
	if(ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)) {
		mgIN.move_voltage(-12000);
		mtIN3.move_voltage(12000);
	} else if (ct.get_digital_new_release(E_CONTROLLER_DIGITAL_R1)) {
		mgIN.move_voltage(0);
		mtIN3.move_voltage(0);
	}

	if(ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)) {
		mgIN.move_voltage(12000);
		mtIN3.move_voltage(-12000);
	} else if (ct.get_digital_new_release(E_CONTROLLER_DIGITAL_R2)) {
		mgIN.move_voltage(0);
		mtIN3.move_voltage(0);
	}

    if(ct.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
        mtIN4.move_voltage(12000);
    } else if (ct.get_digital_new_release(E_CONTROLLER_DIGITAL_B)) {
        mtIN4.move(0);
    }

    if(ct.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
        mtIN4.move_voltage(-12000);
    } else if (ct.get_digital_new_release(E_CONTROLLER_DIGITAL_A)) {
        mtIN4.move(0);
    }

}