#include "vex.h"
#include "vex_competition.h"
#include "vex_task.h"
#include "vex_units.h"

// Include files
#include "globals.h"
#include "functions.h"

using namespace vex;
using namespace std;

// Declares that this program is used for competition use
competition Competition;

// Autonomous mode
void autonomous() {
    mt1.setVelocity(40, velocityUnits::pct);
    mt1.spin(vex::directionType::fwd);
}

// Teleoperated mode
void teleop() {
    ct.Axis2.changed(axisChangedR); // RIGHT JOYSTICK
    ct.Axis3.changed(axisChangedL); // LEFT JOYSTICK

    ct.ButtonR1.pressed(infwd);
    ct.ButtonR1.released(infwdRel);

    ct.ButtonR2.pressed(inrev);
    ct.ButtonR2.released(inrevRel);

    
}


int main() {
    br.Screen.print("this is the pushback");

    // Initializes function for autonomous
    //Competition.autonomous(autonomous);
    Competition.drivercontrol(teleop);

    tempCheck(); // While loop, might use threads later
}
