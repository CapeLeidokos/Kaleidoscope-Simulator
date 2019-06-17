/* -*- mode: c++ -*-
 * Kaleidoscope-Simulator -- A C++ testing API for the Kaleidoscope absolute mouse 
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

#include "actions/Action_.h"
#include "reports/Report_.h"

#include <cassert>

namespace kaleidoscope {
namespace simulator {
   
/// @brief Generates an event that has the same effect as the report being
///        processed by the host.
///
template<typename _ReportType>
class GenerateHostEvent : public ReportAction<_ReportType>
{
   public:

      KT_ACTION_STD_CONSTRUCTOR(AssertAnyKeycodeActive)
   
   private:
      
      class Action : public ReportAction<_ReportType> {
   
         public:

            virtual void describe(const char *add_indent = "") const override {
               this->getSimulator()->log() << add_indent << "Generating host event";
            }

            virtual void describeState(const char *add_indent = "") const {
               this->describe(add_indent);
            }

            virtual bool evalInternal() override;
      };
   
   KT_AUTO_DEFINE_ACTION_INVENTORY_TMPL(AssertReportEquals<_ReportType>)
};

} // namespace simulator
} // namespace kaleidoscope
