#include "globals.h"
#include "functions.h"

// Initialize Controller
controller ct = controller();

void axisChangedL() {
    // LEFT JOYSTICK
    int ax3 = ct.Axis3.position(); // UD for LEFT JOYSTICK
    int ax4 = ct.Axis4.position(); // RL for LEFT JOYSTICK

    double axToVolt = ax3/12.0;

    mgL.spin(fwd, axToVolt, voltageUnits::volt);

}

void axisChangedR() {
    // RIGHT JOYSTICK
    int ax1 = ct.Axis1.position(); // RL for RIGHT JOYSTICK
    int ax2 = ct.Axis2.position(); // UD for RIGHT JOYSTICK

    double axToVolt = ax2/12.0;

    mgR.spin(fwd, axToVolt, voltageUnits::volt);

}