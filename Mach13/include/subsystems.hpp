#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

// Conveyor
    inline pros::Motor inveyor (11, pros::MotorGears::blue , pros::MotorUnits::rotations);
// Ladybrown
    #DEFINE MIN_ANGLE 7
    #DEFINE LOAD_ANGLE 16
    #DEFINE MAX_ANGLE 100
    inline pros::MotorGroup ladybrown ({16, -15}, pros::MotorGears::green , pros::MotorUnits::degrees);
    inline pros::ADIPotentiometer ldb ('H', pros::E_ADI_POT_EDR);
    inline int ladystate = 0; // -1 = Free Spin, 0 = Passthrough, 1 = Load, 2 = Score, 3 = Override

    inline int ldb_pct() {
      return ldb.get_value() / 40.96;
    }

// Mogo Clamp
#pragma region Mogo Clamp
    inline pros::adi::DigitalOut mogo ('A', 0);
    inline pros::adi::DigitalIn btn ('G');
    inline int clampstate = 0; // 0 = Open, 1 = Armed, 2 = Clamped

    inline void set_clamp(int state) {
      clampstate = state; // Update the clamp state
      mogo.set_value(clampstate == 2);
      pros::delay(50); // Wait for controller screen refresh
      // Set the screen text and rumble the controller
      if (clampstate == 0) {
        master.set_text(0, 0, "Open     ");
      } else if (clampstate == 1) {
        master.set_text(0, 0, "Armed     ");
        pros::delay(50);
        master.rumble("-");
      } else if (clampstate == 2) {
        master.set_text(0, 0, "Clamped");
        pros::delay(50);
        master.rumble(".");
      }
    }
#pragma endregion
