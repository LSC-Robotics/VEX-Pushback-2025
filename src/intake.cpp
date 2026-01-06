#include <globals.h>
#include <functions.h>

void intake() {
    if(ct.ButtonR1.pressing()) {
        mgI.spin(fwd, 12, volt);
    } else if (ct.ButtonR2.pressing()) {
       mgI.spin(vex::directionType::rev, 12, volt);
    }
}