#include "main.h"
#include <cmath>
#include <algorithm>

// ============================================================
//                 ✅ EDIT THESE TWO SECTIONS
// ============================================================

// ---- Vision smart port (YOU MUST SET THIS) ----
static constexpr int VISION_PORT = 11;   // <-- change to your actual Vision port

// ---- Pneumatics wiring ----
// If solenoids are plugged directly into Brain 3-wire ports A-H:
static constexpr char PNEU_LIFT_PORT     = 'A';  // scissor ladder (both cylinders via T-fitting)
static constexpr char PNEU_DESCORER_PORT = 'B';  // descorer solenoid

// If you are using an ADI Expander plugged into Smart Port 10:
//   pros::ADIExpander expander(10);
//   pros::ADIDigitalOut lift(expander, 'A'); etc.

// ============================================================
//                 Motor Ports (from your message)
// ============================================================

// Left drive: 1 top rear (gear inverted), 2 bottom rear, 3 bottom front
static constexpr int L_TOP_PORT   = 1; // gear inverted
static constexpr int L_REAR_PORT  = 2;
static constexpr int L_FRONT_PORT = 3;

// Right drive: 4 top rear (gear inverted), 5 bottom rear, 6 bottom front
static constexpr int R_TOP_PORT   = 4; // gear inverted
static constexpr int R_REAR_PORT  = 5;
static constexpr int R_FRONT_PORT = 6;

// Intake: 7 left, 8 right
static constexpr int INTAKE_L_PORT = 7;
static constexpr int INTAKE_R_PORT = 8;

// Flick: 9
static constexpr int FLICK_PORT = 9;

// ============================================================
//                 Tunables
// ============================================================

static constexpr int DRIVE_MAX_MV = 12000;
static constexpr int INTAKE_MV    = 12000;

static constexpr double FLICK_DEG = 120.0;   // amount to jog per press
static constexpr int    FLICK_RPM = 200;

// Simple vision align parameters (tune later)
static constexpr int VISION_SIG_GOAL = 1;   // change to your signature IDs
static constexpr int VISION_CENTER_X = 158; // for 316px wide camera image
static constexpr int VISION_TOL      = 12;

// ============================================================
//                 Devices
// ============================================================

// Drive motors
pros::Motor lTop  (L_TOP_PORT,   pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees); // inverted
pros::Motor lRear (L_REAR_PORT,  pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor lFront(L_FRONT_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

pros::Motor rTop  (R_TOP_PORT,   pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees); // gear inversion cancels mirror
pros::Motor rRear (R_REAR_PORT,  pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
pros::Motor rFront(R_FRONT_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

// Intake
pros::Motor intakeL(INTAKE_L_PORT, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor intakeR(INTAKE_R_PORT, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

// Flick
pros::Motor flick(FLICK_PORT, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);

// Vision
pros::Vision vision(VISION_PORT);

// Pneumatics (3-wire)
pros::ADIDigitalOut liftPneu(PNEU_LIFT_PORT);
pros::ADIDigitalOut descorerPneu(PNEU_DESCORER_PORT);

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// ============================================================
//                 Helpers
// ============================================================

static inline int deadband(int v, int db = 5) {
  return (std::abs(v) < db) ? 0 : v;
}
static inline int clampMV(int mv) {
  return std::max(-DRIVE_MAX_MV, std::min(DRIVE_MAX_MV, mv));
}

void setDriveBrake(pros::motor_brake_mode_e mode) {
  lTop.set_brake_mode(mode); lRear.set_brake_mode(mode); lFront.set_brake_mode(mode);
  rTop.set_brake_mode(mode); rRear.set_brake_mode(mode); rFront.set_brake_mode(mode);
}

void driveTankMV(int left_mv, int right_mv) {
  left_mv  = clampMV(left_mv);
  right_mv = clampMV(right_mv);

  lTop.move_voltage(left_mv);
  lRear.move_voltage(left_mv);
  lFront.move_voltage(left_mv);

  rTop.move_voltage(right_mv);
  rRear.move_voltage(right_mv);
  rFront.move_voltage(right_mv);
}

void stopDrive() { driveTankMV(0, 0); }

void intakeOn(int mv = INTAKE_MV) {
  intakeL.move_voltage(mv);
  intakeR.move_voltage(mv);
}
void intakeReverse(int mv = INTAKE_MV) {
  intakeL.move_voltage(-mv);
  intakeR.move_voltage(-mv);
}
void intakeOff() {
  intakeL.move_voltage(0);
  intakeR.move_voltage(0);
}

// ---- Flicker helpers ----
void flickUp() {
 // flick.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  flick.move_relative(FLICK_DEG, FLICK_RPM);
}
void flickDown() {
 // flick.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  flick.move_relative(-FLICK_DEG, FLICK_RPM);
}

// Kept for autonomous sequence (one up+down cycle)
void flickOnce() {
  flick.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  flick.move_relative(FLICK_DEG, FLICK_RPM);
  pros::delay(250);
  flick.move_relative(-FLICK_DEG, FLICK_RPM);
  pros::delay(250);
}

// Vision align: turns until goal signature is centered
bool visionAlignGoal(int timeout_ms = 1000) {
  const int start = pros::millis();

  while (pros::millis() - start < timeout_ms) {
    auto obj = vision.get_by_sig(0, VISION_SIG_GOAL);

    if (obj.signature == 0 || obj.width < 10) {
      // no target seen -> slow scan
      driveTankMV(1800, -1800);
      pros::delay(20);
      continue;
    }

    int err = obj.x_middle_coord - VISION_CENTER_X;
    if (std::abs(err) <= VISION_TOL) {
      stopDrive();
      return true;
    }

    int turn = (int)std::clamp(err * 35, -3000, 3000);
    driveTankMV(turn, -turn);
    pros::delay(20);
  }

  stopDrive();
  return false;
}

// Simple timed drive helper (reliable for a first auton; tune times)
void driveTimed(int left_mv, int right_mv, int ms) {
  driveTankMV(left_mv, right_mv);
  pros::delay(ms);
  stopDrive();
  pros::delay(50);
}

// ============================================================
//                 PROS Hooks
// ============================================================

void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Push Back PROS Base Ready");

  setDriveBrake(pros::E_MOTOR_BRAKE_BRAKE);

  liftPneu.set_value(false);       // default lift down
  descorerPneu.set_value(false);   // default descorer up
  flick.tare_position();

  // ================================
  // Motor reversal flags
  // ================================
  lTop.set_reversed(false);  // top left (gear-inverted - reverse gearing cancels it out)
  lRear.set_reversed(true);  // ✅ bottom left (port 2) reversed as requested
  lFront.set_reversed(true); // ✅ bottom front left (port 3) reversed as requested

  // Right drive bottom motors reversed; top not
  rTop.set_reversed(true);
  rRear.set_reversed(false);
  rFront.set_reversed(false);

  // (Your current intake reversal as posted)
  intakeR.set_reversed(true);
}

void disabled() {}
void competition_initialize() {}

// ============================================================
//                 AUTONOMOUS (sane “first working” version)
// ============================================================

void autonomous() {
  setDriveBrake(pros::E_MOTOR_BRAKE_BRAKE);

  // 1) Intake and collect initial 3 balls
  intakeOn();
  driveTimed(9000, 9000, 700); // forward ~short distance

  // 2) Go to first goal and align (vision helps if signature exists)
  (void)visionAlignGoal(900);
  driveTimed(8000, 8000, 600);

  // 3) Head toward loader and take ~6 balls
  driveTimed(-8000, -8000, 400);
  driveTimed(7000, -7000, 300);   // turn (tune direction)
  driveTimed(9000, 9000, 900);    // drive to loader
  driveTimed(6000, 6000, 400);    // press into loader and feed
  pros::delay(300);

  // 4) Back into tall goal area, lift up, flick balls
  liftPneu.set_value(true);       // raise scissor ladder (both cylinders together)
  pros::delay(200);

  driveTimed(-9000, -9000, 900);  // back out from loader
  driveTimed(-7000, 7000, 300);   // turn toward tall goal (tune)
  driveTimed(-9000, -9000, 600);  // back into tall goal

  intakeOff();
  for (int i = 0; i < 6; i++) {
    flickOnce();
    pros::delay(120);
  }

  // 5) Drop descorer and push into long goal
  descorerPneu.set_value(true);
  pros::delay(150);

  intakeOn();
  driveTimed(9000, 9000, 800);
  driveTimed(7000, 7000, 400);

  // Cleanup
  intakeOff();
  descorerPneu.set_value(false);
  liftPneu.set_value(false);
  stopDrive();
}

// ============================================================
//                 DRIVER CONTROL (Tank: explicit mapping)
// ============================================================
// Left joystick  -> ports 1, 2, 3 (left drive)
// Right joystick -> ports 4, 5, 6 (right drive)
// L1 -> flick UP, L2 -> flick DOWN
// R1 -> INTAKE,  R2 -> OUTTAKE

void opcontrol() {
  setDriveBrake(pros::E_MOTOR_BRAKE_COAST);

  bool liftUp = false;
  bool descorerDown = false;

  while (true) {
    // Read sticks with deadband
    int leftY  = deadband(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));
    int rightY = deadband(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

    // Scale to millivolts
    int left_mv  = clampMV((leftY  * DRIVE_MAX_MV) / 127);
    int right_mv = clampMV((rightY * DRIVE_MAX_MV) / 127);

    // Explicit mapping by side
    lTop.move_voltage(left_mv);
    lRear.move_voltage(left_mv);
    lFront.move_voltage(left_mv);

    rTop.move_voltage(right_mv);
    rRear.move_voltage(right_mv);
    rFront.move_voltage(right_mv);

    // Intake mapping: R1 in, R2 reverse
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      intakeOn();
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      intakeReverse();
    } else {
      intakeOff();
    }

    // Flick mapping: L1 = up, L2 = down (one jog per press)
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
      flickUp();
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
      flickDown();
    }

    // Lift toggle: UP
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
      liftUp = !liftUp;
      liftPneu.set_value(liftUp);
    }

    // Descorer toggle: A
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
      descorerDown = !descorerDown;
      descorerPneu.set_value(descorerDown);
    }

    // Optional: X toggles brake hold (scrum mode)
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
      static bool hold = false;
      hold = !hold;
      setDriveBrake(hold ? pros::E_MOTOR_BRAKE_HOLD : pros::E_MOTOR_BRAKE_COAST);
    }

    pros::delay(10);
  }
}