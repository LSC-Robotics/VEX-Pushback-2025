#include "functions.h"
#include "globals.h"
#include "vex_units.h"

void axisChangedL() {
  // LEFT JOYSTICK
  int ax3 = ct.Axis3.position(); // UD for LEFT JOYSTICK
  int ax4 = ct.Axis4.position(); // RL for LEFT JOYSTICK

  double axToVolt = ax3 / 12.0;

  dtL.spin(fwd, axToVolt, voltageUnits::volt);
  mt3.spin(vex::directionType::rev, axToVolt, voltageUnits::volt);
}

void axisChangedR() {
  // RIGHT JOYSTICK
  int ax1 = ct.Axis1.position(); // RL for RIGHT JOYSTICK
  int ax2 = ct.Axis2.position(); // UD for RIGHT JOYSTICK

  double axToVolt = ax2 / 12.0;

  dtR.spin(fwd, axToVolt, voltageUnits::volt);
  mt6.spin(vex::directionType::rev, axToVolt, voltageUnits::volt);
}
