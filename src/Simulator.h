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

#include "ActionContainer.h"
#include "ActionQueueAdaptor.h"
#include "reports/BootKeyboardReport.h"
#include "reports/KeyboardReport.h"
#include "reports/MouseReport.h"
#include "reports/AbsoluteMouseReport.h"
#include "actions/generic_report/ReportAction.h"

#include <vector>
#include <functional>

/// @namespace kaleidoscope
///
namespace kaleidoscope {

/// @namespace simulator
///
namespace simulator {
   
class Simulator;
class Action_;

/// @brief An auxlialary tag template for method selection.
/// @details This class is necessary as C++ does not allow
///        for proper method template specialization.
///        An instance of this class is passed to some method
///        to enable the compiler to select a proper overload.
///        The empty parameter is thereby optimized away by
///        the compiler. No additional runtime costs are 
///        generated by this approach.
///
template<typename _T>
struct ReportType {};

/// @private
///
template<typename _ReportType>
struct ReportTraits {};

enum {
   AnyTypeReportSid = 0,
   BootKeyboardReportSid = 1,
   KeyboardReportSid = 2,
   MouseReportSid = 3,
   AbsoluteMouseReportSid = 4
};

/// @private
///
template<>
struct ReportTraits<BootKeyboardReport>
{
   static constexpr int sid = BootKeyboardReportSid;
};

/// @private
///
template<>
struct ReportTraits<KeyboardReport>
{
   static constexpr int sid = KeyboardReportSid;
};

/// @private
///
template<>
struct ReportTraits<MouseReport>
{
   static constexpr int sid = MouseReportSid;
};

/// @private
///
template<>
struct ReportTraits<AbsoluteMouseReport>
{
   static constexpr int sid = AbsoluteMouseReportSid;
};

/// @brief An abstract simulator output stream.
///
class SimulatorStream_ {
   
   public:
      
      /// @brief An auxiliary class that triggers line breaks.
      /// @details Pass an instance of this class to
      ///        cause line breaks in log/error/header
      ///        stream output.
      ///
      struct Endl {};
      
      SimulatorStream_(const Simulator *simulator) : simulator_(simulator) {}
      
      virtual ~SimulatorStream_() {}

   protected:
      
      std::ostream &getOStream() const;
      
      void checkLineStart();
      
      virtual void reactOnLineStart();
      virtual void reactOnLineEnd();
      
      template<typename _T>
      void output(const _T &t) {
         this->checkLineStart();
         this->getOStream() << t;
      }
      
      void output(const Endl &) {
         line_start_ = true;
         this->reactOnLineEnd();
         this->getOStream() << std::endl;
      }
      
      bool mute() const;
      
   protected:
      
      const Simulator *simulator_;
      
   private:
      
      bool line_start_ = true;
};
   
/// @brief A stream class for error output.
///
class ErrorStream : public SimulatorStream_ {
   
   public:
      
      ErrorStream(const Simulator *simulator);
      
      template<typename _T>
      ErrorStream &operator<<(const _T &t) { 
         this->output(t);
         return *this; 
      }
      
      virtual ~ErrorStream() override;

   private:
   
      virtual void reactOnLineStart() override;
};

/// @brief A stream class for debug output.
///
class DebugStream : public SimulatorStream_ {
   
   public:
      
      DebugStream(const Simulator *simulator);
      
      template<typename _T>
      DebugStream &operator<<(const _T &t) { 
         if(this->mute()) { return *this; }
         this->output(t);
         return *this; 
      }
      
      virtual ~DebugStream() override;

   private:
   
      virtual void reactOnLineStart() override;
};

/// @brief A stream class for log output.
///
class LogStream : public SimulatorStream_ {
   
   public:
      
      LogStream(const Simulator *simulator);
      virtual ~LogStream() override;
      
      template<typename _T>
      LogStream &operator<<(const _T &t) { 
         if(this->mute()) { return *this; }
         this->output(t);
         return *this; 
      }
};

/// @brief A stream class that generates formatted text headers in log output.
///
class HeaderStream : public SimulatorStream_ {
   
   public:
      
      HeaderStream(const Simulator *simulator);
      
      template<typename _T>
      HeaderStream &operator<<(const _T &t) { 
         if(this->mute()) { return *this; }
         this->output(t);
         return *this; 
      }
      
      virtual ~HeaderStream() override;
      
   private:
   
      virtual void reactOnLineStart() override;
};

/// @brief A class that represents an individual test.
///
class Test {
   
   public:
      
      Test(Simulator *simulator, const char *name);
      ~Test();
      
   private:
      
      Simulator *simulator_;
      const char *name_;
      int error_count_start_;
};

        
/// @brief The main simulator object.
///
class Simulator {
   
   public:
      
      typedef unsigned long TimeType;
   
   private:
      
      std::ostream *out_;
      bool debug_;
      bool quiet_ = false;
      int cycle_duration_;
      bool abort_on_first_error_;
      
      bool actions_passed_ = true;
      bool test_success_ = true;
   
      int cycle_id_ = 0;
      TimeType time_ = 0;
      int scan_cycles_default_count_ = 5;
      
      mutable int error_count_ = 0;
      
      bool error_if_report_without_queued_actions_ = false;
      
      int n_typed_reports_in_cycle_[5] = {};
      int n_typed_overall_reports_[5] = {};
      
      int n_reports_in_cycle_ = 0;
      int n_overall_reports_ = 0;
      
      ActionContainer<ReportAction_> queued_report_actions_;
            
      ActionContainer<ReportAction<BootKeyboardReport>> permanent_boot_keyboard_report_actions_;
      ActionContainer<ReportAction<KeyboardReport>> permanent_keyboard_report_actions_;
      ActionContainer<ReportAction<MouseReport>> permanent_mouse_report_actions_;
      ActionContainer<ReportAction<AbsoluteMouseReport>> permanent_absolute_mouse_report_actions_;
      ActionContainer<ReportAction_> permanent_generic_report_actions_;
      
      ActionContainer<Action_> queued_cycle_actions_;
      ActionContainer<Action_> permanent_cycle_actions_;
      
   public:
      
      ~Simulator();
      
      /// @details If the ErrorIfReportWithoutQueuedActions is enabled
      ///          it is considered an error if no actions are queued
      ///          for a report.
      /// @param state The new boolean state of the condition.
      ///
      void setErrorIfReportWithoutQueuedActions(bool state) {
         error_if_report_without_queued_actions_ = state;
      }
      
      /// @brief Retreives the state of the ErrorIfReportWithoutQueuedActions
      ///        condition.
      /// @returns The conditions state.
      ///
      bool getErrorIfReportWithoutQueuedActions() const {
         return error_if_report_without_queued_actions_;
      }
      
      ActionQueueAdaptor<ActionContainer<ReportAction_>> reportActionsQueue() {
         return ActionQueueAdaptor<ActionContainer<ReportAction_>>{queued_report_actions_};
      }
      
      /// @brief Retreives the permanent boot keyboard report actions.
      ///
      ActionContainer<ReportAction<BootKeyboardReport>> &permanentBootKeyboardReportActions() {
         return permanent_boot_keyboard_report_actions_;
      }
      
      /// @brief Retreives the permanent keyboard report actions.
      ///
      ActionContainer<ReportAction<KeyboardReport>> &permanentKeyboardReportActions() {
         return permanent_keyboard_report_actions_;
      }
      
      /// @brief Retreives the permanent mouse report actions.
      ///
      ActionContainer<ReportAction<MouseReport>> &permanentMouseReportActions() {
         return permanent_mouse_report_actions_;
      }
      
      /// @brief Retreives the absolute mouse report actions.
      ///
      ActionContainer<ReportAction<AbsoluteMouseReport>> &permanentAbsoluteMouseReportActions() {
         return permanent_absolute_mouse_report_actions_;
      }
      
      /// @brief Retreives the generic report actions.
      ///
      ActionContainer<ReportAction_> &permanentReportActions() {
         return permanent_generic_report_actions_;
      }
      
      /// @brief Retreives the queued cycle actions.
      /// @details The head of the action queue is applied at the end of
      ///        the next cycle and removed afterwards.
      ///
      ActionQueueAdaptor<ActionContainer<Action_>> cycleActionsQueue() {
         return ActionQueueAdaptor<ActionContainer<Action_>>{queued_cycle_actions_};
      }
      
      /// @brief Retreives the permanent cycle actions.
      /// @details Permanent cycle actions are applied after every cycle.
      ///
      ActionContainer<Action_> &permanentCycleActions() {
         return permanent_cycle_actions_;
      }
      
      /// @brief Registers a key press event.
      ///
      /// @param row The keyboard matrix row.
      /// @param col The keyboard matrix col.
      ///
      void pressKey(uint8_t row, uint8_t col);
       
      /// @brief Registers a key release event. 
      ///
      /// @details Make sure that the key was registered
      ///          as pressed, using pressKey(...), before calling this method.
      ///
      /// @param row The keyboard matrix row.
      /// @param col The keyboard matrix col.
      ///
      void releaseKey(uint8_t row, uint8_t col);
      
      /// brief Registers a key being tapped.
      ///
      /// @param row The keyboard matrix row.
      /// @param col The keyboard matrix col.
      ///
      void tapKey(uint8_t row, uint8_t col);
      
      /// @brief Taps a key a number of times.
      ///
      /// @details After each tap, a specified number of cycles 
      ///          are processed automatically before the next tap is issued.
      ///
      /// @param num_taps The number of taps.
      /// @param row The keyboard matrix row.
      /// @param col The keyboard matrix col.
      /// @param tap_interval_cycles The number of cycles that   
      ///        elapse between individual taps.
      /// @param after_tap_and_cycles_action An action
      ///        that is evaluated after every tap.
      ///
      void multiTapKey(int num_taps, uint8_t row, uint8_t col, 
                       int tap_interval_cycles = 1,
                       std::shared_ptr<Action_> after_tap_and_cycles_action = std::shared_ptr<Action_>()
                      );

      /// @brief Releases all keys that are currently pressed.
      ///
      void clearAllKeys();
         
      /// @brief Runs a scan cycle and processes actions afterwards.
      ///
      void cycle();
         
      /// @brief Runs a number of scan cycles and processes actions afterwards.
      ///
      /// @param n The number of cycles to run.
      /// @tparam actions A list of actions that are evaluated
      ///        after every cycle.
      ///
      template<typename..._Actions>
      void cycles(int n = 0, _Actions...actions) {
         this->cyclesInternal(n,
            std::vector<std::shared_ptr<Action_>>{
               std::forward<_Actions>(actions)...
            }
         );
      }
      
      template<typename..._Actions>
      void cycleExpectReports(_Actions...actions) {
         
         this->reportActionsQueue()
            .queue(std::forward<_Actions>(actions)...);
            
         this->cycle();
         
         if(!queued_report_actions_.empty()) {
            this->error() << "Keyboard report actions are left in queue";
         }
      }
            
      /// @brief Skips a given amount of time by running cycles.
      ///
      /// @param delta_t A time interval in [ms] that is supposed to be skipped.
      ///
      void advanceTimeBy(TimeType delta_t);
      
      /// @brief Runs keyboard scan cycles until a specified point in time.
      ///
      /// @param time The target time in [ms].
      ///
      void advanceTimeTo(TimeType time);
      
      /// @brief Immediately evaluates a number of actions
      ///
      /// @tparam actions A number actions to be evaluated immediately.
      ///
      template<typename..._Actions>
      void evaluateActions(_Actions...actions) {
         this->evaluateActionsInternal(
            std::vector<std::shared_ptr<Action_>>{
               std::forward<_Actions>(actions)...
            }
         );
      }
            
      /// @brief Retreives a stream object for log output.
      ///
      /// @returns The log stream object.
      /// 
      LogStream log() const { return LogStream{this}; }
      
      /// @brief Retreives a log stream for header output.
      ///
      /// @returns The header log stream object.
      /// 
      HeaderStream header() const { return HeaderStream{this}; }
      
      /// @brief Retreives a log stream for debug output.
      ///
      /// @returns The debug stream object.
      /// 
      DebugStream debug() const { return DebugStream{this}; }
         
      /// @brief Retrieves a log stream for error output.
      ///
      /// @returns The error log stream object.
      ///
      ErrorStream error() const { 
         ++error_count_;
         return ErrorStream{this}; 
      }
      
      /// @brief Retreives the total number of errors that were registered.
      ///
      int getErrorCount() const { return error_count_; }
      
      /// @brief Resets the keyboard to initial state.
      ///
      void initKeyboard();
      
      /// @brief Retreives the state of the AbortOnFirstError condition.
      ///
      bool getAbortOnFirstError() const { return abort_on_first_error_; }
      
      /// @brief Retreives the current time [ms].
      ///
      TimeType getTime() const { return time_; }
      
      /// @brief Sets time.
      /// 
      /// @param time The new value for time in [ms].
      ///
      void setTime(TimeType time) { time_ = time; }
      
      /// @brief Retreives the current cycle id.
      ///
      int getCycleId() const { return cycle_id_; }
      
      /// @brief Sets debugging output state. If enabled, additional (verbose)
      ///        output is generated.
      ///
      /// @param state The new state of debugging output.
      ///
      void setDebug(bool state = true) { debug_ = state; }
      
      /// @brief Retreives the current debugging state.
      ///
      bool getDebug() const { return debug_; }
            
      /// @brief Sets the simulator to quiet mode (no output).
      ///
      /// @param state The new state of quiet mode.
      ///
      void setQuiet(bool state = true) { 
         if(state != quiet_) {
            if(quiet_) {
               quiet_ = state;
               this->log() << "Quiet mode disabled.";
            }
            else {
               this->log() << "Quiet mode enabled.";
               quiet_ = state;
            }
         }
      }
      
      /// @brief Retreives the current quiet state.
      ///
      bool getQuiet() const { return quiet_; }
      
      /// @brief Asserts that no actions (keyboard report and cycle)
      ///        are currently queued.
      /// @details This function is automatically called at the end of each 
      ///        test to make sure that all actions were used.
      ///
      void assertNothingQueued() const;
      
      /// @brief Generates a test.
      /// @details The lifetime of the returned test object decides about
      ///        the duration of a test.
      /// @returns The test object. Add additonal scope if necessary to 
      ///        restrict the lifetime of test objects.
      ///
      Test newTest(const char *name) {
         return Test{this, name};
      }
      
      /// @brief Asserts a boolean condition.
      /// @param cond The condition.
      /// @param action_code A string representation of the action.
      ///
      void assertCondition(bool cond, const char *condition_string) const;
      
      /// @brief Changes the duration in [ms] of every simulated cycle.
      /// @param dt The new duration [ms].
      ///
      void setCycleDuration(TimeType dt) {
         cycle_duration_ = dt;
      }
      
      /// @brief Retreives the currently defined cycle duration.
      /// @returns The cycle duration in [ms].
      ///
      TimeType getCycleDuration() const {
         return cycle_duration_;
      }
      
      /// @brief Resets the simulator output stream.
      /// @details This might serve to redirect output to a file
      ///        by using a std::ofstream.
      /// @param out The new ostream object.
      ///
      void setOStream(std::ostream &out) {
         out_ = &out;
      }
      
      /// @brief Retreives the currently associated ostream object.
      ///
      std::ostream &getOStream() const { return *out_; }
      
      /// @brief Runs the simulator for a given amount of time.
      /// @details The simulation runs in real time, i.e. if necessary
      ///        the simulator waits for a given amount of time in each
      ///        cycle to make sure that the simulation runs in exactly
      ///        the same speed as it would run on the device.
      ///        A provided function is executed in every cycle.
      ///
      /// @param duration The duration of the run. If duration is zero,
      ///        the simulation runs forever.
      /// @param cycle_function A function that is executed after every cycle.
      ///
      void runRealtime(TimeType duration, const std::function<void()> &cycle_function);
      
      /// @brief Runs the simulator in a continuous loop an reacts on stdin.
      /// @details Key state information is read from stdin in each loop cycle.
      ///
      /// @param cycle_callback A function that is executed after every cycle.
      /// @param realtime If this parameter is true, the simulator waits 
      ///                 if necessary at the end of cycles to synchronize.
      ///
      void runRemoteControlled(const std::function<void()> &cycle_callback,
                              bool realtime = false);
      
      int getNumReportsInCycle() const { return n_typed_reports_in_cycle_[AnyTypeReportSid]; }
      int getNumOverallReports() const { return n_typed_overall_reports_[AnyTypeReportSid]; }
      
      int getNumBootKeyboardReportsInCycle() const { return n_typed_reports_in_cycle_[BootKeyboardReportSid]; }
      int getNumOverallBootKeyboardReports() const { return n_typed_overall_reports_[BootKeyboardReportSid]; }
      
      int getNumKeyboardReportsInCycle() const { return n_typed_reports_in_cycle_[KeyboardReportSid]; }
      int getNumOverallKeyboardReports() const { return n_typed_overall_reports_[KeyboardReportSid]; }
      
      int getNumMouseReportsInCycle() const { return n_typed_reports_in_cycle_[MouseReportSid]; }
      int getNumOverallMouseReports() const { return n_typed_overall_reports_[MouseReportSid]; }
      
      int getNumAbsoluteMouseReportsInCycle() const { return n_typed_reports_in_cycle_[AbsoluteMouseReportSid]; }
      int getNumOverallAbsoluteMouseReports() const { return n_typed_overall_reports_[AbsoluteMouseReportSid]; }
      
      template<typename _ReportType>
      int getNumTypedReportsInCycle() const {
         return n_typed_reports_in_cycle_[ReportTraits<_ReportType>::sid];
      }
      
      template<typename _ReportType>
      int getNumTypedOverallReports() const {
         return n_typed_overall_reports_[ReportTraits<_ReportType>::sid];
      }
      
      /// @brief Access the global simulator singleton.
      ///
      static Simulator &getInstance();
      
   private:
      
      /// @brief Constructor.
      ///
      /// @param out The output stream that is used for all output.
      /// @param debug Generates additional debug information if enabled.
      /// @param cycle_duration The duration in ms of one scan cycle.
      /// @param abort_on_first_error If enabled, testing is aborted after
      ///        the first error occurred.
      ///
      Simulator(std::ostream &out, 
             bool debug, 
             int cycle_duration = 1, 
             bool abort_on_first_error = false);
      
      bool checkStatus();
            
      void headerText();
      
      void footerText();
                 
      void cycleInternal(bool only_log_reports = false);
      
      void checkCycleDurationSet();
      
      ActionContainer<ReportAction<BootKeyboardReport>> &getPermanentReportActions(ReportType<BootKeyboardReport>) {
         return permanent_boot_keyboard_report_actions_;
      }
      
      ActionContainer<ReportAction<KeyboardReport>> &getPermanentReportActions(ReportType<KeyboardReport>) {
         return permanent_keyboard_report_actions_;
      }
      
      ActionContainer<ReportAction<MouseReport>> &getPermanentReportActions(ReportType<MouseReport>) {
         return permanent_mouse_report_actions_;
      }
      
      ActionContainer<ReportAction<AbsoluteMouseReport>> &getPermanentReportActions(ReportType<AbsoluteMouseReport>) {
         return permanent_absolute_mouse_report_actions_;
      }
      
      // This method is templated to enable it being used for std::vector
      // and std::deque.
      //
      template<typename _Container>
      void evaluateActionsInternal(const _Container &actions) {
            
         if(actions.empty()) { return; }
         
         for(auto &action: actions) {
      
            // Just in case we haven't done that before
            //
            action->setSimulator(this);
            
            bool action_passed = action->eval();
            
            if(!action_passed || debug_) {
               action->report();
            }
            
            actions_passed_ &= action_passed;
         }
      }
      
      void processReportAction(ReportAction_ &action, 
                                  const Report_ &report) {
         
         action.setReport(&report);
         
         bool action_passed = action.eval();
         
         if(!action_passed || debug_) {
            action.report();
         }
         
         actions_passed_ &= action_passed;
         action.setReport(nullptr);
      }
      
      template<typename _ReportType>
      void processReport(const _ReportType &report) {
         
         ++n_typed_overall_reports_[AnyTypeReportSid];
         ++n_typed_reports_in_cycle_[AnyTypeReportSid];
         
         static constexpr int sid = ReportTraits<_ReportType>::sid;
         
         ++n_typed_overall_reports_[sid];
         ++n_typed_reports_in_cycle_[sid];
         
         this->log() << "Processing " << _ReportType::typeString() << " report "
               << n_typed_overall_reports_[AnyTypeReportSid]
               << " (" << n_typed_reports_in_cycle_[AnyTypeReportSid] << ". in cycle "
               << this->getCycleId() << ")";
                        
         auto n_actions_queued = queued_report_actions_.size();
         
         this->log() << n_actions_queued
            << " queued " << _ReportType::typeString() << " report actions";
         
         if(!queued_report_actions_.empty()) {
            auto action = queued_report_actions_.popFront();
            
            auto report_type = action->getReportTypeId();
            
            if(report_type == Report_::hid_report_type_) {
               // Generic report
               //
               this->processReportAction(*action, report);
            }
            else if(report_type == _ReportType::hid_report_type_) {
               auto &typed_action = static_cast<ReportAction<_ReportType>&>(*action);
               this->processReportAction(typed_action, report);
            }
            else {
               this->error() << "Expected a "
                  << action->getTypeString() << " action but encountered a "
                  << _ReportType::typeString() << " action";
            }
         }
         
         auto &permanent_actions = this->getPermanentReportActions(ReportType<_ReportType>{});
         for(auto &action: permanent_actions.directAccess()) {
            this->processReportAction(*action, report);
         }

         for(auto &action: permanent_generic_report_actions_.directAccess()) {
            if(   (action->getReportTypeId() == _ReportType::hid_report_type_)
               || (action->getReportTypeId() == GenericReportTypeId)) {
               this->processReportAction(*action, report);
            }
         }
               
         if((n_actions_queued == 0) && this->getErrorIfReportWithoutQueuedActions()) {
            this->error() << "Encountered a " << _ReportType::typeString() << " report without actions being queued";
         }
      }
      
      std::string generateCycleInfo() const;
      
      void skipTimeInternal(TimeType delta_t);
      
      void cyclesInternal(int n, 
                  const std::vector<std::shared_ptr<Action_>> &cycle_action_list);
      
      static void processHIDReport(uint8_t id, const void* data, 
                                    int len, int result);
};

/// @brief Asserts a condition.
/// @details Use this macro instead of the Simulator class' assertCondition(...)
///        method to enable string output of the condition code in
///        error messages.
///
#define KT_ASSERT_CONDITION(DRIVER, ...)                                       \
   DRIVER.assertCondition((__VA_ARGS__), #__VA_ARGS__)
      
} // namespace simulator
} // namespace kaleidoscope
