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

#include "ATMStateObjects.h"


void ATMStateMachine::setup_transition_map() {

  transition_map_[&gAtmState_PowerUp][kSystemError] = &gAtmState_SystemError;
  transition_map_[&gAtmState_PowerUp][kSystemReady] = &gAtmState_Idle;

  transition_map_[&gAtmState_Idle][kSystemError] = &gAtmState_SystemError;
  transition_map_[&gAtmState_Idle][kCardInserted] = &gAtmState_Authenticate;

  transition_map_[&gAtmState_Authenticate][kCancel] = &gAtmState_Idle;
  transition_map_[&gAtmState_Authenticate][kPinEntryTimeout] = &gAtmState_Idle;
  transition_map_[&gAtmState_Authenticate][kSystemError] = &gAtmState_SystemError;
  transition_map_[&gAtmState_Authenticate][kAuthOk] = &gAtmState_Transaction;

  transition_map_[&gAtmState_Transaction][kCancel] = &gAtmState_Idle;
  transition_map_[&gAtmState_Transaction][kTransactionTimeout] = &gAtmState_Idle;
  transition_map_[&gAtmState_Transaction][kSystemError] = &gAtmState_SystemError;
  transition_map_[&gAtmState_Transaction][kTransactionComplete] = &gAtmState_Idle;

}

