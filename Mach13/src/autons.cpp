#include "main.h"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 110;
const int SWING_SPEED = 90;

///
// Constants
///
void default_constants() {
  chassis.pid_heading_constants_set(11.3, 0, 20);
  chassis.pid_drive_constants_set(20.4, 0, 100);
  chassis.pid_turn_constants_set(2.5, 0.05, 20, 15);
  chassis.pid_swing_constants_set(6, 0, 65);

  chassis.pid_turn_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(80_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);

  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  chassis.slew_drive_constants_set(7_in, 80);
}

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

// . . .
// Make your own autonomous functions here!
// // . . .

void redleft() {
  chassis.pid_drive_set(-19_in, 90, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();
  ladybrown.move_relative(-5, 200);
  pros::delay(1000);
  chassis.pid_drive_set(-7_in, 60, true);
  chassis.pid_wait();
  chassis.pid_turn_set(-132_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-41_in, 70, true);
  chassis.pid_wait();
  set_clamp(2);
  chassis.pid_turn_set(5_deg, TURN_SPEED);
  chassis.pid_wait();
  inveyor.move(200);
chassis.pid_drive_set(20_in, 90, true);
chassis.pid_wait();
chassis.pid_drive_set(4.3_in, 60, true);
chassis.pid_wait();
chassis.pid_turn_relative_set(90_deg, TURN_SPEED);
chassis.pid_wait();
chassis.pid_drive_set(10_in, 90, true);
chassis.pid_wait();
chassis.pid_drive_set(-20_in, 70, true);
chassis.pid_wait();
chassis.pid_turn_relative_set(0_deg, TURN_SPEED);
chassis.pid_wait();
chassis.pid_drive_set(32_in, 110, true);
chassis.pid_wait();
pros::delay(99999);
}

void blueright() {
 chassis.pid_drive_set(-19_in, 90, true);
  chassis.pid_wait();
  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();
  ladybrown.move_relative(-5, 200);
  pros::delay(1000);
  chassis.pid_drive_set(-5_in, 60, true);
  chassis.pid_wait();
  chassis.pid_drive_set(2, 70, true);
  chassis.pid_wait();
  chassis.pid_turn_set(132_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-41_in, 70, true);
  chassis.pid_wait();
  set_clamp(2);
  chassis.pid_turn_set(-5_deg, TURN_SPEED);
  chassis.pid_wait();
  inveyor.move(200);
chassis.pid_drive_set(20_in, 90, true);
chassis.pid_wait();
chassis.pid_drive_set(4.3_in, 60, true);
chassis.pid_wait();
chassis.pid_turn_relative_set(-90_deg, TURN_SPEED);
chassis.pid_wait();
chassis.pid_drive_set(10_in, 90, true);
chassis.pid_wait();
chassis.pid_drive_set(-20_in, 70, true);
chassis.pid_wait();
chassis.pid_turn_relative_set(-70_deg, TURN_SPEED);
chassis.pid_wait();
chassis.pid_drive_set(32_in, 110, true);
chassis.pid_wait();
pros::delay(99999);
}

void skills() {
  // Score Preload
  ladybrown.move_relative(-5, 200);
  pros::delay(1000);
  // Get Mogo
  chassis.pid_turn_relative_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_relative_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-10_in, 60, true);
  chassis.pid_wait(); 
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-25.5, 50, true);
  chassis.pid_wait();
  inveyor.move(200);
  set_clamp(2);
  // Get and score first Ring
  chassis.pid_drive_set(-3, 60, true);
  chassis.pid_wait();
  chassis.pid_turn_relative_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(24_in, 40, true);
  chassis.pid_wait();
  pros::delay(1000);
  // Get and score second Ring
  chassis.pid_turn_relative_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(21.25_in, 40, true);
  chassis.pid_wait();
  pros::delay(1000);
  // Get and score third and fourth ring 
  chassis.pid_turn_relative_set(88_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(20_in, 60, true);
  chassis.pid_wait();
  pros::delay(875);
  chassis.pid_drive_set(16_in, 40, true);
  chassis.pid_wait();
  pros::delay(1000);
  // Score Mogo
   chassis.pid_turn_relative_set(122_deg, TURN_SPEED);
  chassis.pid_wait();
  set_clamp(0);
  chassis.pid_drive_set(-12_in, 40, true);
  chassis.pid_wait();
  chassis.pid_drive_set(24.5_in, 80, true);
  chassis.pid_wait();
  // Get 2nd mogo
chassis.pid_turn_relative_set(145_deg, TURN_SPEED);
chassis.pid_wait();
chassis.pid_drive_set(-50.5_in, 75, true );
chassis.pid_wait();
set_clamp(2);
// Mirror
 chassis.pid_drive_set(-3, 60, true);
  chassis.pid_wait();
  chassis.pid_turn_relative_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(25_in, 40, true);
  chassis.pid_wait();
  inveyor.move_relative(10, 200);
  pros::delay(2000);
  // Get and score second Ring
  chassis.pid_turn_relative_set(-90_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(21.25_in, 40, true);
  chassis.pid_wait();
  pros::delay(2000);
  // Get and score third and fourth ring
  chassis.pid_drive_set(3.5_in, 40, true);
  chassis.pid_wait();
  chassis.pid_turn_relative_set(-88_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(20_in, 60, true);
  chassis.pid_wait();
  pros::delay(875);
  chassis.pid_drive_set(14_in, 40, true);
  chassis.pid_wait();
  pros::delay(2500);
  // Score Mogo
   chassis.pid_turn_relative_set(-122_deg, TURN_SPEED);
  chassis.pid_wait();
  set_clamp(0);
  chassis.pid_drive_set(-12_in, 40, true);
  chassis.pid_wait();
  chassis.pid_drive_set(26_in, 80, true);
  chassis.pid_wait();
}
void redleftbackup() {
   // Go to goal and clamp
  chassis.pid_drive_set(-24_in, 50, true);
  chassis.pid_wait();
  if (!btn.get_value() == 1) {;
    chassis.pid_drive_set(1_in, DRIVE_SPEED, true);
  }
  set_clamp(2);
  pros::delay(500);
  inveyor.move_relative(5, 200);
  pros::delay(1000);
  chassis.pid_turn_set(75_deg, TURN_SPEED);
  chassis.pid_wait();
  while (inveyor.get_actual_velocity() > 10) {;
    pros::delay(20);
  }
  inveyor.move_relative(15, 200);
  chassis.pid_drive_set(19_in,55, true);
  chassis.pid_wait();
  inveyor.move_relative(10, 200);
  pros::delay(3000);
  //Go to ladder
  chassis.pid_turn_relative_set(160_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(33_in, 90, true);
  chassis.pid_wait();
}
void nothing(){
chassis.pid_drive_set(5_in, 60, true);
chassis.pid_wait();
}