#include "main.h"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {-8, 9, -10},     // Left Chassis Ports (negative port will reverse it!)
    {18, -19, 20},  // Right Chassis Ports (negative port will reverse it!)

    17,      // IMU Port
    3.25,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    450);   // Wheel RPM

void on_center_button() { // Toggle the clamp
  if (clampstate == 0) { // If the clamp is open//
    set_clamp(2); // Close the clamp
  } else {
    set_clamp(0); // Open the clamp
  }
}

#pragma region Initialize
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);  // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0);    // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0, 0);     // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
      Auton("Red Left", redleft),
      Auton("Red Right", redright),
      Auton("Blue Right", blueright),
      Auton("Blue Left", blueleft),
      Auton("Solo AWP", soloawp),
      Auton("Skills", skills),
      // Auton("Example Drive\n\nDrive forward and come back.", drive_example),
      // Auton("Example Turn\n\nTurn 3 times.", turn_example),
      // Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
      // Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
      // Auton("Swing Example\n\nSwing in an 'S' curve", swing_example),
      // Auton("Motion Chaining\n\nDrive forward, turn, and come back, but blend everything together :D", motion_chaining),
      // Auton("Combine all 3 movements", combining_movements),
      // Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example)

  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");

  // Initialize device properties
  ladybrown.set_brake_mode_all(MOTOR_BRAKE_HOLD);
}
#pragma endregion
#pragma region Disabled
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}
#pragma endregion
#pragma region Comp Initialize
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}
#pragma endregion
#pragma region Autonomous
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency

  ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
}
#pragma endregion
#pragma region Driver Control
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // This is preference to what you like to drive on
  pros::motor_brake_mode_e_t driver_preference_brake = MOTOR_BRAKE_COAST;

  chassis.drive_brake_set(driver_preference_brake);

  while (true) {
    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    if (!pros::competition::is_connected()) {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      if (master.get_digital_new_press(DIGITAL_X))
        chassis.pid_tuner_toggle();

      // Trigger the selected autonomous routine
      if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
        autonomous();
        chassis.drive_brake_set(driver_preference_brake);
      }

      chassis.pid_tuner_iterate();  // Allow PID Tuner to iterate
    }
    
    // chassis.opcontrol_tank();  // Tank control
    chassis.opcontrol_arcade_standard(ez::SPLIT);   // Standard split arcade
    // chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade

    if (1==0) {
      pros::lcd::print(0, "Efficiency: %d", inveyor.get_efficiency());
    }

    if (master.get_digital(DIGITAL_R2)) {
      inveyor.move_velocity(600);
    } else if (master.get_digital(DIGITAL_R1)) {
      inveyor.move_velocity(-600);
    } else {
      inveyor.move_velocity(0);
    }

    if (master.get_digital(DIGITAL_L1)) {
      if (ldb_pct() < MAX_ANGLE) {
        ladystate = -1;
        ladybrown.move_velocity(LDB_SPEED);
      } else {
        ladybrown.brake();
      }
    } else if (master.get_digital(DIGITAL_L2)) {
      if (ldb_pct() > MIN_ANGLE) {
        ladystate = -1;
        ladybrown.move_velocity(-1*LDB_SPEED);
      } else {
        ladybrown.brake();
      }
    } else {
      if (ladystate == 0 and ldb_pct() > MIN_ANGLE) {
          ladybrown.move_velocity(-1*LDB_SPEED);
      } else if (ladystate == 1 and ldb_pct() < LOAD_ANGLE) {
        ladybrown.move_velocity(50);
      } else if (ladystate == 2) {
        if (master.get_digital(DIGITAL_DOWN)) {
          if (ldb_pct() < MAX_ANGLE) {
            ladybrown.move_velocity(LDB_SPEED);
          } else {
            ladybrown.brake();
          }
        } else {
          ladystate = 0;
        }
      } else {
        ladybrown.brake();
      }
    }

    if (master.get_digital_new_press(DIGITAL_B)) { // Toggle the clamp
      if (clampstate == 0) { // If the clamp is open
        set_clamp(2); // Close the clamp
      } else {
        set_clamp(0); // Open the clamp//
      }
    }

    if (master.get_digital_new_press(DIGITAL_DOWN)) { // Toggle Lady Brown
      if (ladystate == -1) {
        ladystate = 0;
      } else if (ladystate == 0) {
        ladystate = 1;
      } else if (ladystate == 1) {
        ladystate = 2;
      }
    }

    if (master.get_digital_new_press(DIGITAL_A)) {
      rush.toggle();
    }   
    // Old Clamp Arming Code
    /* if (master.get_digital_new_press(DIGITAL_L2)) { // Arm the clamp
      set_clamp(1);
    }

    if (btn.get_new_press() && clampstate == 1) { // Automatically close the clamp
      set_clamp(2);
    } */

    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
    }
  } 
#pragma endregion