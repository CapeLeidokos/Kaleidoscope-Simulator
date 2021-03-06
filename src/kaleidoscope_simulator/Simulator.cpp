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

#include "kaleidoscope_simulator/Simulator.h"
#include "kaleidoscope_simulator/SimulatorCore.h"
#include "kaleidoscope_simulator/reports/KeyboardReport.h"
#include "kaleidoscope_simulator/reports/BootKeyboardReport.h"
#include "kaleidoscope_simulator/reports/MouseReport.h"
#include "kaleidoscope_simulator/reports/AbsoluteMouseReport.h"

#include "Kaleidoscope.h"
#include "HIDReportObserver.h"

#include <iostream>

namespace kaleidoscope {
namespace simulator {
   
   Simulator::Simulator(std::ostream &out)
   :  papilio::Simulator{out}
{
   this->setCore(
      std::shared_ptr<SimulatorCore>{
                        new SimulatorCore{}
      }
   );
   
   HIDReportObserver::resetHook(&Simulator::processHIDReport);
   
   Kaleidoscope.device().keyScanner().setEnableReadMatrix(false);
}

Simulator &Simulator::getInstance() {
   static Simulator sim{std::cout};
   return sim;
}

void Simulator::processHIDReport(uint8_t id, const void* data, 
                                    int len, int result)
{
   auto &simulator = Simulator::getInstance();
   
   switch(id) {
      // TODO: React appropriately on the following
      //
      case HID_REPORTID_GAMEPAD:
      case HID_REPORTID_CONSUMERCONTROL:
      case HID_REPORTID_SYSTEMCONTROL:
         simulator.log() << "***Ignoring hid report with id = " << id;
         break;
      case HID_REPORTID_KEYBOARD:
         {
            simulator.processReport(BootKeyboardReport{data});
         }
         break;
      case HID_REPORTID_MOUSE_ABSOLUTE:
         {
            simulator.processReport(AbsoluteMouseReport{data});
         }
         break;
      case HID_REPORTID_MOUSE:
         {
            simulator.processReport(MouseReport{data});
         }
         break;
      case HID_REPORTID_NKRO_KEYBOARD:
         {
            simulator.processReport(KeyboardReport{data});
         }
         break;
      default:
         simulator.error() << "Encountered unknown HID report with id = " << id;
   }
}

} // namespace simulator
} // namespace kaleidoscope
