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

#include "TestDriver.h"
#include "aux/keycodes.h"
#include "KeyboardReport.h"
#include "assertions/Group.h"
#include "assertions/LayerIsActive.h"
#include "assertions/CycleHasNReports.h"
#include "assertions/keyboard_report/ReportEmpty.h"
#include "assertions/keyboard_report/ModifierActive.h"
#include "assertions/keyboard_report/KeycodeActive.h"
#include "assertions/keyboard_report/DumpReport.h"
#include "assertions/keyboard_report/ModifiersActive.h"
#include "assertions/keyboard_report/AnyKeycodeActive.h"
#include "assertions/keyboard_report/KeycodesActive.h"
#include "assertions/keyboard_report/ReportNthInCycle.h"
#include "assertions/keyboard_report/AnyModifierActive.h"
#include "assertions/_Assertion.h"
#include "assertions/NumOverallReportsGenerated.h"
#include "assertions/CycleIsNth.h"
#include "assertions/ElapsedTimeGreater.h"