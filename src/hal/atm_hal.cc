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

#include "atm_hal.h"

namespace atm_hal {

//-----------------------------------------------------------------------------
// ATM Card API
namespace card {

Status is_inserted(bool *state) {
  *state = false;
  return kOk;
}

Status eject(void) {
  return kOk;
}

Status get_data_blob(std::vector<uint8_t> *data) {
  data->clear();
  data->resize(1024, 0);  
  return kOk;
}

Status set_data_blob(const std::vector<uint8_t> &data) {
  (void)data;
  return kOk;
}

} // namespace card

//-----------------------------------------------------------------------------
// ATM Cash Dispenser API
namespace dispenser {

Status get_available_cash(std::vector<int> *cash_quantity) {
  cash_quantity->clear();
  cash_quantity->resize(CashValue::kEnd,0);
  return kOk;
}

Status dispense_cash(const std::vector<int> &cash_quantity) {
  (void)cash_quantity;
  return kOk;
}

} // namespace dispenser

//-----------------------------------------------------------------------------
// ATM Deposit API
namespace deposit {

Status is_full(bool *state) {
  (void)state;
  return kOk;
}

Status deposit_items(void) {
  return kOk;
}

} // namespace deposit

} // namespace atm_hal
