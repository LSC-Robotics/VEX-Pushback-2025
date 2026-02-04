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

// ADI port mappings for lift and descorer cylinders
static constexpr char PNEU_LIFT_UP   = 'A';
static constexpr char PNEU_LIFT_DOWN = 'B';
static constexpr char PNEU_DESC_UP   = 'C';
static constexpr char PNEU_DESC_DOWN = 'D';

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

static constexpr double FLICK_DEG = 468.0;   // amount for one flick (1.3 rotations)
static constexpr int    FLICK_RPM = 425;
static constexpr int    FLICK_MV  = 10000;  // voltage for flick movement
static constexpr int    FLICK_TIME_MS = 175; // time to move 1.3 rotations at 200 RPM

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

// Pneumatics (ADI 3-pin solenoids on expander)
pros::ADIDigitalOut liftUp(PNEU_LIFT_UP);
pros::ADIDigitalOut liftDown(PNEU_LIFT_DOWN);
pros::ADIDigitalOut descorerUp(PNEU_DESC_UP);
pros::ADIDigitalOut descorerDown(PNEU_DESC_DOWN);

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
  flick.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  flick.move_absolute(FLICK_DEG, FLICK_RPM);
}
void flickDown() {
  flick.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  flick.move_absolute(0, FLICK_RPM);
}

// Kept for autonomous sequence (one up+down cycle)
void flickOnce() {
  flick.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  flick.move_absolute(FLICK_DEG, FLICK_RPM);
  pros::delay(250);
  flick.move_absolute(0, FLICK_RPM);
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

  liftUp.set_value(false);      // default lift down
  liftDown.set_value(false);
  descorerUp.set_value(false);  // default descorer down
  descorerDown.set_value(false);
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

  // 1) Drive forward 3 feet while intaking
  intakeOn();
  driveTimed(9000, 9000, 1200); // ~3 feet forward

  // 2) Turn right 90 degrees
  driveTankMV(5000, -5000);
  pros::delay(600); // adjust timing for 90° turn
  stopDrive();

  // 3) Drive 3 feet over more balls with intake on
  driveTimed(9000, 9000, 1200); // ~3 feet forward

  // 4) Turn around 135 degrees (right)
  driveTankMV(5000, -5000);
  pros::delay(800); // adjust timing for 135° turn
  stopDrive();

  // 5) Drive 4 feet
  driveTimed(9000, 9000, 1600); // ~4 feet forward

  // 6) Flick into medium goal
  intakeOff();
  flickOnce();
  pros::delay(300);

  // 7) Turn around 180 degrees
  driveTankMV(5000, -5000);
  pros::delay(1100); // adjust timing for 180° turn
  stopDrive();

  // 8) Drive 6 feet
  driveTimed(9000, 9000, 2400); // ~6 feet forward

  // 9) Turn right 45 degrees
  driveTankMV(5000, -5000);
  pros::delay(300); // adjust timing for 45° turn
  stopDrive();

  // 10) Engage lift and drive slowly into loader
  liftUp.set_value(true);
  liftDown.set_value(false);
  pros::delay(300); // wait for lift to engage

  intakeOn();
  driveTimed(6000, 6000, 800); // slow drive into loader

  // 11) Intake 6 balls from loader
  intakeOn();
  pros::delay(2000); // intake time for 6 balls

  // 12) Reverse 3 feet
  driveTimed(-9000, -9000, 1200); // ~3 feet backward

  // 13) Flick into high goal while engaging descorer
  descorerUp.set_value(true);
  descorerDown.set_value(false);
  pros::delay(200);

  for (int i = 0; i < 6; i++) {
    flickOnce();
    pros::delay(120);
  }

  // Cleanup
  intakeOff();
  descorerUp.set_value(false);
  descorerDown.set_value(false);
  liftUp.set_value(false);
  liftDown.set_value(false);
  stopDrive();
}

// ============================================================
//                 DRIVER CONTROL (Tank: explicit mapping)
// ============================================================
// Left joystick  -> ports 1, 2, 3 (left drive)
// Right joystick -> ports 4, 5, 6 (right drive)
// L1 -> flicker up, L2 -> flicker down
// Y -> lift toggle (up/down)
// R1 -> INTAKE,  R2 -> OUTTAKE
// A -> descorer toggle

void opcontrol() {
  setDriveBrake(pros::E_MOTOR_BRAKE_COAST);

  bool liftState = false;     // false = down, true = up
  bool descorerState = false;

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

    // Flicker: L1 up, L2 down (press to move fixed amount with voltage)
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
      flick.move_voltage(FLICK_MV);
      pros::delay(FLICK_TIME_MS);
      flick.move_voltage(0);
    }
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
      flick.move_voltage(-FLICK_MV);
      pros::delay(FLICK_TIME_MS);
      flick.move_voltage(0);
    }

    // Lift toggle: Y (press once to up, again to down)
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
      if (liftState) {
        // Lift is up, bring it down
        liftUp.set_value(false);
        liftDown.set_value(true);
        liftState = false;
      } else {
        // Lift is down, bring it up
        liftUp.set_value(true);
        liftDown.set_value(false);
        liftState = true;
      }
    }

    // Descorer toggle: A
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
          descorerState = !descorerState;
          if (descorerState) {
            descorerDown.set_value(true);
            descorerUp.set_value(false);
          } else {
            descorerDown.set_value(false);
            descorerUp.set_value(false);
          }
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