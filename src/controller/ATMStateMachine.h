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
#ifndef _CONTROLLER_ATMSTATEMACHINE_H_
#define _CONTROLLER_ATMSTATEMACHINE_H_

#include <stdio.h>
#include <unordered_map>
#include <queue>
#include "ATMEvents.h"
#include "ATMStateObjectBase.h"

#ifndef LOG_PRINT
#define LOG_PRINT(...)  do { printf("%s:%s:%d ", __FILE__, __func__, __LINE__); printf(__VA_ARGS__); } while(0) 
#endif

class ATMStateMachine {
public:
  ATMStateMachine(ATMStateObjectBase &start_state) : current_state_(&start_state) {
    LOG_PRINT("Setting up transition map\n");
    setup_transition_map();
    LOG_PRINT("Raising Event\n");
    raise_event(kPowerUp);
  }

  void raise_event(ATMEvents event);
  void execute();

protected:
  void setup_transition_map();

  ATMStateObjectBase *current_state_;

  /*
   * Cannot use enum type in hash as there is no hashing function for enum.
   * https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
   * Using int as type for enum for the map.
   */ 
  typedef std::unordered_map<int, ATMStateObjectBase* > EventAction;
  typedef std::unordered_map<ATMStateObjectBase*, EventAction> TransitionMap;
  TransitionMap transition_map_;

  std::queue<ATMEvents> event_queue_; 

};
#endif // _CONTROLLER_ATMSTATEMACHINE_H_
