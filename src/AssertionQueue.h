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

// Undefine some macros that are defined by Arduino
#undef min
#undef max

#include <deque>
#include <vector>
#include <memory>

namespace kaleidoscope {
namespace testing {
   
class Driver;
class _Assertion;
   
class AssertionQueue
{
   public:
      
      AssertionQueue(Driver &driver) : driver_(driver) {}
      
      /** brief Queues an assertion.
       *
       * param assertion The assertion to be added to the queue.
       * 
       * return void
       */ 
      AssertionQueue &add(const std::shared_ptr<_Assertion> &assertion);
      
      /** brief Queues a list of assertion.
       *
       * param assertions The assertions to be added to the queue.
       * 
       * return void
       */ 
      AssertionQueue &addGrouped(const std::vector<std::shared_ptr<_Assertion>> &assertions);
      
      
      /** brief Queues a list of assertion.
       *
       * param assertions The assertions to be added to the queue.
       * 
       * return void
       */ 
      template<typename..._Assertions>
      AssertionQueue &addGrouped(_Assertions...assertions) {
         return this->addGrouped(
            std::vector<std::shared_ptr<_Assertion>>{std::forward<_Assertions>(assertions)...}
         );
      }
      
      /** brief Removes an assertion.
       *
       * param assertion The assertion to be removed from the queue.
       * 
       * return void
       */ 
      AssertionQueue &remove(const std::shared_ptr<_Assertion> &assertion);
      
      /** brief Removes a list of assertions.
       *
       * param assertions The assertions to be removed from the queue.
       * 
       * return void
       */ 
      AssertionQueue &remove(const std::vector<std::shared_ptr<_Assertion>> &assertions);
      
      /** brief Removes the assertion at the front of the queue and returns it.
       * 
       * return The former front element of the queue.
       */ 
      std::shared_ptr<_Assertion> popFront();
      
      /** brief Retreives the number of entries in the queue.
       * 
       * return The number of entries currently in the queue.
       */
      std::size_t size() const;
      
      /** brief Checks the queue for being empty.
       * 
       * return True if the queue is empty, false otherwise.
       */
      bool empty() const;
      
      /** brief Clear the entire queue.
       * 
       * return void.
       */
      void clear();
      
      const std::deque<std::shared_ptr<_Assertion>> &directAccess() {
         return queue_;
      }
      
   private:
      
      void configureAssertion(const std::shared_ptr<_Assertion> &assertion);
      
      std::shared_ptr<_Assertion> generateAssertionGroup(
            const std::vector<std::shared_ptr<_Assertion>> &assertions);
      
   private:
      
      Driver &driver_;
      std::deque<std::shared_ptr<_Assertion>> queue_;
};

} // namespace testing
} // namespace kaleidoscope