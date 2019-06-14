/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator -- A C++ testing API for the Kaleidoscope keyboard 
 *                         firmware.
 * Copyright (C) 2019  noseglasses (shinynoseglasses@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "MultiReport/Mouse.h"
#include "Report_.h"

// Undefine some macros defined by Arduino
//
#undef min
#undef max

#include <vector>
#include <stdint.h>
#include <ostream>

namespace kaleidoscope {
namespace simulator {
   
class Simulator;
  
/// @brief An interface hat facilitates analyzing mouse reports.
///
class MouseReport : public Report_ {
   
   public:
      
      typedef HID_MouseReport_Data_t ReportDataType;
      
      static constexpr uint8_t hid_report_type_ = HID_REPORTID_MOUSE;
      
      MouseReport() {}
      
      MouseReport(const void *data);
      
      /// @brief Constructs based on a report data object.
      /// @param report_data The report data object to read.
      ///
      MouseReport(const HID_MouseReport_Data_t &report_data);
      
      /// @brief Checks equality with another report.
      /// @param other Another report to compare with.
      /// @returns [bool] True if both reports are equal.
      ///
      bool operator==(const MouseReport &other) const;
      
      /// @brief Checks if a set of buttons is pressed.
      /// @param button_state The state of the mouse buttons to check.
      /// @returns True if the button state matches the given one.
      ///
      bool areButtonsPressed(uint8_t button_state) const;
      
      /// @brief Queries if the left button is pressed.
      /// @returns True if the left button is pressed.
      ///
      bool isLeftButtonPressed() const; 
      
      /// @brief Queries if the middle button is pressed.
      /// @returns True if the middle button is pressed.
      ///
      bool isMiddleButtonPressed() const;  
      
      /// @brief Queries if the right button is pressed.
      /// @returns True if the right button is pressed.
      ///
      bool isRightButtonPressed() const;
      
      /// @brief Queries the x-movement stored in the report.
      /// @returns The x-movement.
      ///
      signed char getMovementX() const;
      
      /// @brief Queries the y-movement stored in the report.
      /// @returns The y-movement.
      ///
      signed char getMovementY() const;
      
      /// @brief Queries the verical wheel movement.
      /// @returns The vertical wheel movement.
      ///
      signed char getVerticalWheel() const;  
      
      /// @brief Queries the horizontal wheel movement.
      /// @returns The horizontal wheel movement.
      ///
      signed char getHorizontalWheel() const;
          
      /// @brief Checks if the report is empty.
      /// @details Empty means that no buttons are active and
      ///        no motion or wheel activity is present.
      ///
      virtual bool isEmpty() const override;
      
      /// @brief Writes a formatted representation of the keyboard report 
      ///        to the simulator's log stream.
      /// @param add_indent An additional indentation string.
      ///
      virtual void dump(const Simulator &simulator, const char *add_indent = "") const override;
      
      /// @brief Associates the object with new report data.
      /// @param report_data The new report data struct.
      ///
      void setReportData(const HID_MouseReport_Data_t &report_data);
      
      static const char *typeString() { return "mouse"; }
      virtual const char *getTypeString() const override { return typeString(); }
      
   private:
   
      HID_MouseReport_Data_t report_data_;
};

} // namespace simulator
} // namespace kaleidoscope