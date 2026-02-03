#include "globals.h"
#include "adi.hpp"
#include <vector>

using namespace pros;
using namespace std;

// FOR PORTS: +NUM = FWD | -NUM = REV

// LEFT MOTOR PORTS
#define L1 -1 
#define L2 2
#define L3 3

// RIGHT MOTOR PORTS
#define R1 -4
#define R2 5
#define R3 6

// INTAKE PORTS
#define IN1 7
#define IN2 -8

// Launch Port
#define IN3 9

// PNEUMATICS PORTS
#define PN0 'A'
#define PN1 'B'


// Set the master controller
Controller ct(pros::E_CONTROLLER_MASTER);

// LEFT MOTORS
Motor mtL1(L1);
Motor mtL2(L2);
Motor mtL3(L3);

// RIGHT MOTORS
Motor mtR1(R1);
Motor mtR2(R2);
Motor mtR3(R3);

// INTAKE MOTORS
Motor mtIN1(IN1);
Motor mtIN2(IN2);

// Launch Port
Motor mtIN3(IN3);

// MOTOR GROUPS
// Setup vector for ports & then initialize
std::vector<std::int8_t> portsL = {L1, L2, L3};
MotorGroup mgL (portsL);

vector<std::int8_t> portsR = {R1, R2, R3};
MotorGroup mgR (portsR);

adi::Pneumatics pn0(PN0, false);
adi::Pneumatics pn1(PN1, false);

// BASIC VARIABLES
bool R1_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R1);
bool R1_RELEASED = ct.get_digital_new_release(E_CONTROLLER_DIGITAL_R1);

bool R2_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R2);
bool R2_RELEASED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_R2);

bool L1_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_L1);
bool L1_RELEASED = ct.get_digital_new_release(E_CONTROLLER_DIGITAL_L1);

bool L2_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_L2);
bool L2_RELEASED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_L2);

bool A_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_A);
bool B_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_B);
bool X_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_X);
bool Y_PUSHED = ct.get_digital_new_press(E_CONTROLLER_DIGITAL_Y);
