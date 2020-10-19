/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org>
 *
 */
#include <stdio.h>

#include "ATMStateMachine.h"

#define CRITICAL_SECTION()
#ifndef LOG_PRINT
#define LOG_PRINT(...)  do { printf("%s:%s:%d ", __FILE__, __func__, __LINE__); printf(__VA_ARGS__); } while(0) 
#endif

void ATMStateMachine::raise_event(ATMEvents event) {
  CRITICAL_SECTION();
  event_queue_.push(event);
  LOG_PRINT("raising event: %d\n", event);
}

void ATMStateMachine::execute() {

  /* 
   * Loop through all queued events and execute the state object run function
   * to handle the event.
   *
   * If there is a state transition associated with that event, perform the 
   * current state exit and next state entry actions.
   */
  while (!event_queue_.empty()) {
    ATMEvents event = event_queue_.front();
    event_queue_.pop();

    LOG_PRINT("executing event: %d\n", event);

    current_state_->run_loop(event);

    TransitionMap::iterator current_state_it = transition_map_.find(current_state_);
    
    if (current_state_it == transition_map_.end()) {
      // Handle error for undefined state
      LOG_PRINT("Error: Transitioning to undefined state\n");
    } else {
      EventAction::iterator event_action_it = current_state_it->second.find(event);

      if (event_action_it == current_state_it->second.end()) {
        LOG_PRINT("event not triggering state transition: %d\n", event);
        continue;
      } 
      
      LOG_PRINT("Potential triggering state transition: %d "
          " from: %p to %p\n", event, current_state_, event_action_it->second);

      /*
       * State transition behavior here is to only switch states if 
       * we are successful in current state exit and new state entry operations.
       * Otherwise we remain in the same state.  
       * Clearing the event queue is important for state transition as we don't
       * want to act on stale events.
       */ 
      bool exit_status = current_state_->exit(event);
      LOG_PRINT("Exit Status: %d\n", exit_status);  

      if (exit_status) {
        bool enter_status = event_action_it->second->enter(event);
        LOG_PRINT("Enter Status: %d\n", enter_status);  

        if (enter_status) {
          while (!event_queue_.empty()) event_queue_.pop();
          current_state_ = event_action_it->second;
          current_state_->run_loop(event);
        } else {
          LOG_PRINT("Not transitioning due to fail entry status\n");
        }
      } else {
        LOG_PRINT("Not transitioning due to fail exit status\n");
      }
    }
  }
}

