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

#include "KaleidoscopeSimulator.h"

#include "Kaleidoscope.h"

namespace kaleidoscope {
namespace simulator {
   
unsigned long millis = 0;
   
void SimulatorCore::init()
{
   kaleidoscope::hid::initializeKeyboard();
}

void SimulatorCore::getKeyMatrixDimensions(uint8_t &rows, uint8_t &cols)
{
   row = KeyboardHardware.matrix_rows;
   cols = KeyboardHardware.matrix_columns;
}

void SimulatorCore::pressKey(uint8_t row, uint8_t col)
{
   KeyboardHardware.setKeystate(row, col, Virtual::PRESSED);
}

void SimulatorCore::releaseKey(uint8_t row, uint8_t col)
{
   KeyboardHardware.setKeystate(row, col, Virtual::NOT_PRESSED);
}

void SimulatorCore::tapKey(uint8_t row, uint8_t col)
{
   KeyboardHardware.setKeystate(row, col, Virtual::TAP);
}

bool SimulatorCore::isKeyPressed(uint8_t row, uint8_t col)
{
   return KeyboardHardware.getKeystate(row, col) == Virtual::PRESSED;
}

void SimulatorCore::getCurrentKeyLEDColor(uint8_t row, uint8_t col, 
                                  uint8_t &red, uint8_t &green, uint8_t &blue)
{
   auto color = KeyboardHardware.getCrgbAt(row, col);
   
   red = color.r;
   green = color.g;
   blue = color.b;
}

void SimulatorCore::getCurrentKeyLabel(uint8_t row, uint8_t col,
                                      std::string &label_string)
{
   auto key = Layer.lookupOnActiveLayer(row, col);
         
   if(key.flags == KEY_FLAGS) {
      
      // Map the keycode to a string that matches the key
      //            
      auto it = hid_code_to_string.find(key.keyCode);
      if(it != hid_code_to_string.end()) {
         label_string = it->second;
      }
   }
}

void SimulatorCore::setTime(uint32_t time)
{
   millis = time;
}

namespace keycodes {
   
#define FOR_ALL_KEYBOARD(FUNC) \
   FUNC(A) \
   FUNC(B) \
   FUNC(C) \
   FUNC(D) \
   FUNC(E) \
   FUNC(F) \
   FUNC(G) \
   FUNC(H) \
   FUNC(I) \
   FUNC(J) \
   FUNC(K) \
   FUNC(L) \
   FUNC(M) \
   FUNC(N) \
   FUNC(O) \
   FUNC(P) \
   FUNC(Q) \
   FUNC(R) \
   FUNC(S) \
   FUNC(T) \
   FUNC(U) \
   FUNC(V) \
   FUNC(W) \
   FUNC(X) \
   FUNC(Y) \
   FUNC(Z) \
   FUNC(1) \
   FUNC(2) \
   FUNC(3) \
   FUNC(4) \
   FUNC(5) \
   FUNC(6) \
   FUNC(7) \
   FUNC(8) \
   FUNC(9) \
   FUNC(0) \
   FUNC(Enter) \
   FUNC(Escape) \
   FUNC(Backspace) \
   FUNC(Tab) \
   FUNC(Spacebar) \
   FUNC(Minus) \
   FUNC(Equals) \
   FUNC(LeftBracket) \
   FUNC(RightBracket) \
   FUNC(Backslash) \
   FUNC(NonUsPound) \
   FUNC(Semicolon) \
   FUNC(Quote) \
   FUNC(Backtick) \
   FUNC(Comma) \
   FUNC(Period) \
   FUNC(Slash) \
   FUNC(CapsLock) \
   FUNC(F1) \
   FUNC(F2) \
   FUNC(F3) \
   FUNC(F4) \
   FUNC(F5) \
   FUNC(F6) \
   FUNC(F7) \
   FUNC(F8) \
   FUNC(F9) \
   FUNC(F10) \
   FUNC(F11) \
   FUNC(F12) \
   FUNC(PrintScreen) \
   FUNC(ScrollLock) \
   FUNC(Pause) \
   FUNC(Insert) \
   FUNC(Home) \
   FUNC(PageUp) \
   FUNC(Delete) \
   FUNC(End) \
   FUNC(PageDown) \
   FUNC(RightArrow) \
   FUNC(LeftArrow) \
   FUNC(DownArrow) \
   FUNC(UpArrow) \
   FUNC(KeypadNumLock) \
   FUNC(KeypadDivide) \
   FUNC(KeypadMultiply) \
   FUNC(KeypadSubtract) \
   FUNC(KeypadAdd) \
   FUNC(KeypadEnter) \
   FUNC(Keypad1) \
   FUNC(Keypad2) \
   FUNC(Keypad3) \
   FUNC(Keypad4) \
   FUNC(Keypad5) \
   FUNC(Keypad6) \
   FUNC(Keypad7) \
   FUNC(Keypad8) \
   FUNC(Keypad9) \
   FUNC(Keypad0) \
   FUNC(KeypadDot) \
   FUNC(NonUsBackslashAndPipe) \
   FUNC(PcApplication) \
   FUNC(Power) \
   FUNC(KeypadEquals) \
   FUNC(F13) \
   FUNC(F14) \
   FUNC(F15) \
   FUNC(F16) \
   FUNC(F17) \
   FUNC(F18) \
   FUNC(F19) \
   FUNC(F20) \
   FUNC(F21) \
   FUNC(F22) \
   FUNC(F23) \
   FUNC(F24) \
   FUNC(Execute) \
   FUNC(Help) \
   FUNC(Menu) \
   FUNC(Select) \
   FUNC(Stop) \
   FUNC(Again) \
   FUNC(Undo) \
   FUNC(Cut) \
   FUNC(Copy) \
   FUNC(Paste) \
   FUNC(Find) \
   FUNC(Mute) \
   FUNC(VolumeUp) \
   FUNC(VolumeDown) \
   FUNC(LockingCapsLock) \
   FUNC(LockingNumLock) \
   FUNC(LockingScrollLock) \
   FUNC(KeypadComma) \
   FUNC(KeypadEqualSign) \
   FUNC(International1) \
   FUNC(International2) \
   FUNC(International3) \
   FUNC(International4) \
   FUNC(International5) \
   FUNC(International6) \
   FUNC(International7) \
   FUNC(International8) \
   FUNC(International9) \
   FUNC(Lang1) \
   FUNC(Lang2) \
   FUNC(Lang3) \
   FUNC(Lang4) \
   FUNC(Lang5) \
   FUNC(Lang6) \
   FUNC(Lang7) \
   FUNC(Lang8) \
   FUNC(Lang9) \
   FUNC(AlternateErase) \
   FUNC(Sysreq) \
   FUNC(Cancel) \
   FUNC(Clear) \
   FUNC(Prior) \
   FUNC(Return) \
   FUNC(Separator) \
   FUNC(Out) \
   FUNC(Oper) \
   FUNC(ClearSlashAgain) \
   FUNC(CrselSlashProps) \
   FUNC(Exsel) \
   FUNC(Keypad00) \
   FUNC(Keypad000) \
   FUNC(ThousandsSeparator) \
   FUNC(DecimalSeparator) \
   FUNC(CurrencyUnit) \
   FUNC(CurrencySubunit) \
   FUNC(KeypadLeftParen) \
   FUNC(KeypadRightParen) \
   FUNC(KeypadLeftCurlyBrace) \
   FUNC(KeypadRightCurlyBrace) \
   FUNC(KeypadTab) \
   FUNC(KeypadBackspace) \
   FUNC(KeypadA) \
   FUNC(KeypadB) \
   FUNC(KeypadC) \
   FUNC(KeypadD) \
   FUNC(KeypadE) \
   FUNC(KeypadF) \
   FUNC(KeypadXor) \
   FUNC(KeypadCarat) \
   FUNC(KeypadPercent) \
   FUNC(KeypadLessThan) \
   FUNC(KeypadGreaterThan) \
   FUNC(KeypadAmpersand) \
   FUNC(KeypadDoubleampersand) \
   FUNC(KeypadPipe) \
   FUNC(KeypadDoublepipe) \
   FUNC(KeypadColon) \
   FUNC(KeypadPoundSign) \
   FUNC(KeypadSpace) \
   FUNC(KeypadAtSign) \
   FUNC(KeypadExclamationPoint) \
   FUNC(KeypadMemoryStore) \
   FUNC(KeypadMemoryRecall) \
   FUNC(KeypadMemoryClear) \
   FUNC(KeypadMemoryAdd) \
   FUNC(KeypadMemorySubtract) \
   FUNC(KeypadMemoryMultiply) \
   FUNC(KeypadMemoryDivide) \
   FUNC(KeypadPlusSlashMinus) \
   FUNC(KeypadClear) \
   FUNC(KeypadClearEntry) \
   FUNC(KeypadBinary) \
   FUNC(KeypadOctal) \
   FUNC(KeypadDecimal) \
   FUNC(KeypadHexadecimal) \
   FUNC(LeftControl) \
   FUNC(LeftShift) \
   FUNC(LeftAlt) \
   FUNC(LeftGui) \
   FUNC(RightControl) \
   FUNC(RightShift) \
   FUNC(RightAlt) \
   FUNC(RightGui)

const char *SimulatorCore::keycodeToName(uint8_t keycode)
   
   switch(keycode) {
      
#define DEFINE_KEY_CASE(KEY) \
      case (Key_##KEY).keyCode: \
         return #KEY; \
         break;
      
   FOR_ALL_KEYBOARD(DEFINE_KEY_CASE) 
   }

   return "";
}
      
} // namespace simulator
} // namespace kaleidoscope

unsigned long millis(void) {
  return kaleidoscope::simulator::millis;
}
