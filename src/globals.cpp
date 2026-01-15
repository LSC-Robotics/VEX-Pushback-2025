#include "main.h"
#include <vector>

using namespace pros;
using namespace std;

// FOR PORTS: +NUM = FWD | -NUM = REV

// LEFT MOTOR PORTS
#define L1 10
#define L2 9
#define L3 -8

// RIGHT MOTOR PORTS
#define R1 20
#define R2 19
#define R3 -18

// INTAKE PORTS
#define IN1 16
#define IN2 14
#define IN3 -13

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
Motor mtIN3(IN3);

// MOTOR GROUPS
// Setup vector for ports & then initialize
std::vector<std::int8_t> portsL = {L1, L2, L3};
MotorGroup mgL (portsL);

vector<std::int8_t> portsR = {R1, R2, R3};
MotorGroup mgR (portsR);

vector<std::int8_t> portsIN = {IN1, IN2};
MotorGroup mgIN (portsIN);

// test for classes
class drivetrain {
    public:
        MotorGroup groupLeft;
        MotorGroup groupRight;

        void move() {

        }

        void turn() {

        }
};

class intake {
    public:
        Motor mt1;
};