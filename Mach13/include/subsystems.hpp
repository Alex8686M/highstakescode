#pragma once

#include "api.h"

// Your motors, sensors, etc. should go here.  Below are examples

// Inveyor
    inline pros::MotorGroup inveyor ({21, 16}, pros::MotorGears::green , pros::MotorUnits::rotations);
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

inline pros::Motor ladybrown ({14}, pros::MotorGears::green, pros::MotorUnits::rotations);