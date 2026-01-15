#include "main.h"
#include "globals.h"

void drive() {
    int dir = ct.get_analog(ANALOG_LEFT_Y); 
	int turn = ct.get_analog(ANALOG_RIGHT_X);
	mgL.move(dir);
	mgR.move(turn);
}

void intake(){
	if(ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R1)) {
		mgIN.move_voltage(12000);
		mtIN3.move_voltage(-12000);
	} else if (ct.get_digital_new_release(E_CONTROLLER_DIGITAL_R1)) {
		mgIN.move_voltage(0);
		mtIN3.move_voltage(0);
	}

	if(ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R2)) {
		mgIN.move_voltage(-12000);
		mtIN3.move_voltage(12000);
	} else if (ct.get_digital_new_release(E_CONTROLLER_DIGITAL_R2)) {
		mgIN.move_voltage(0);
		mtIN3.move_voltage(0);
	}
}