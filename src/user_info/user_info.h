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
#ifndef _USER_INFO_USER_INFO_H_
#define _USER_INFO_USER_INFO_H_

#include <string>
#include <vector>
#include <stdint.h>

#include "auth/auth.h"

class UserInfo {

public:
  
  enum Status {
    kOk = 0,
    kAuthTokenFail,
    kServerFail,
  };
  
  enum AcctType {
    kInvalid = 0,
    kChecking,
    kSavings
  };

  struct AcctInfo {
    AcctType    type;
    std::string acct_number;
    int         balance;
  };

  const UserInfo &instance(void) {
    static UserInfo instance;
    return instance;
  }

  Status open_session(const Auth &auth); 
  Status get_acct_info(std::vector<AcctInfo> *acct_info);
  Status deposit(int val);
  Status withdraw(int val);
  Status close_session(std::vector<uint8_t> *blob);  

protected:
  UserInfo() {} 

  const Auth *auth_;
};

#endif  /* _USER_INFO_USER_INFO_H_ */
