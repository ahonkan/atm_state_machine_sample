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

#include "hal/atm_hal.h"
#include "user_info/user_info.h"
#include "auth/auth.h"

#include <unordered_map>

enum ATMEvents {
  kInvalid = 0,
  kCardInserted,
  kCancel,
  kPINEntryTimeout,
  kAuthOK,
  kTransactionTimeout,
  kSystemError,
};

template <typename T>
class StateMachineBase {
public:
  virtual bool enter(T event) = 0;
  virtual bool run_loop(T event) = 0;
  virtual bool exit(T event) = 0;
  virtual StateMachineBase() = 0;
  virtual ~StateMachineBase() = 0;
};

class ATMState_Idle : public StateMachineBase<ATMEvents> {
public:
  ATMState_Idle() {}
  ~ATMState_Idle() {}

  virtual bool enter(ATMEvents event);
  virtual bool run_loop(ATMEvents event);
  virtual bool exit(ATMEvents event);
};




typedef std::unordered_map<ATMEvents, const StateMachineBase<ATMEvents>& > EventAction;
std::unordered_map StateTransition<const StateMachineBase<ATMEvents>&, EventAction> ATMStateTransitions;






int main(void) {
  printf("Hello world\n");
  return 0;
}
