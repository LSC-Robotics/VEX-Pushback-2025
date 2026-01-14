#include <globals.h>
#include <functions.h>

void infwd() {
   mt7.spin(fwd, 100, percent);
   mt8.spin(fwd, 100, percent);
   mt9.spin(vex::directionType::rev, 100, percent);
}

void infwdRel() {
   mt7.spin(vex::directionType::rev, 0, volt);
   mt8.spin(vex::directionType::rev, 0, volt);
   mt9.spin(fwd, 0, volt);

}

void inrev() {
    mt7.spin(vex::directionType::rev, 100, percent);
    mt8.spin(vex::directionType::rev, 100, percent);
    mt9.spin(fwd, 100, percent);
}

void inrevRel() {
    mt7.spin(vex::directionType::rev, 0, volt);
    mt8.spin(vex::directionType::rev, 0, volt);
    mt9.spin(fwd, 0, volt);
}