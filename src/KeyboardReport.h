/* -*- mode: c++ -*-
 * Kaleidoscope-Testing -- A C++ testing API for the Kaleidoscope keyboard 
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

#include "kaleidoscope/key_defs.h"

// Undefine some macros defined by Arduino
//
#undef min
#undef max

#include <vector>
#include <stdint.h>
#include <ostream>

namespace kaleidoscope {
namespace testing {
   
class Simulator;
  
/// @brief An interface hat facilitates analyzing keyboard reports.
///
class KeyboardReport {
   
   public:
      
      /// @brief Checks if a keycode is active in the keyboard report.
      /// @param keycode The keycode to check for.
      /// @returns [bool] True if the given keycode is active.
      ///
      bool isKeycodeActive(uint8_t keycode) const;
      
      /// @brief Checks if the keycode of a given Key is active in the keyboard report.
      /// @details Please note that the flags part of the Key is ignored.
      /// @param key The Key whose keycode to check for.
      /// @returns [bool] True if the given Key's keycode is active.
      ///
      bool isKeyActive(const Key &key) const;
      
      /// @brief Retreives a list of all keycodes that are active in the
      ///        keyboard report.
      /// @returns A vector of keycodes.
      ///
      std::vector<uint8_t> getActiveKeycodes() const;
      
      /// @brief Checks if a modifier keycode is active in the keyboard report.
      /// @param keycode The modifier keycode to check for.
      /// @returns [bool] True if the given modifier keycode is active.
      ///
      bool isModifierKeycodeActive(uint8_t modifier) const;
      
      /// @brief Checks if the modifier keycode of a given Key is active in the keyboard report.
      /// @details Please note that the flags part of the Key is ignored.
      /// @param key The Key whose modifier keycode to check for.
      /// @returns [bool] True if the given Key's modifier keycode is active.
      ///
      bool isModifierKeyActive(const Key &key) const;
      
      /// @brief Checks if any modifier keycode is active.
      /// @returns [bool] True if any modifier keycode is active, false otherwise.
      ///
      bool isAnyModifierActive() const;
      
      /// @brief Checks if any key keycode is active.
      /// @returns [bool] True if any key keycode is active, false otherwise.
      ///
      bool isAnyKeyActive() const;
      
      /// @brief Retreives a list of active modifier keycodes.
      /// @returns A list of active modifier keycodes.
      ///
      std::vector<uint8_t> getActiveModifiers() const;
      
      /// @brief Checks if the report is empty.
      /// @details Empty means neither key nor modifier keycodes are active.
      ///
      bool isEmpty() const;
      
      /// @brief Writes a formatted representation of the keyboard report 
      ///        to the simulator's log stream.
      /// @param add_indent An additional indentation string.
      ///
      void dump(const Simulator &simulator, const char *add_indent = "") const;
      
      /// @brief Associates the object with new report data.
      /// @param report_data The new report data struct.
      ///
      void setReportData(const HID_KeyboardReport_Data_t &report_data);
      
   private:
   
      HID_KeyboardReport_Data_t report_data_;
};

} // namespace testing
} // namespace kaleidoscope
