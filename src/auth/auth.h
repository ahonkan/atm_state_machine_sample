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
#ifndef _AUTH_AUTH_H_
#define _AUTH_AUTH_H_

#include <string>
#include <vector>
#include <stdint.h>


class Auth {

public:
  
  enum Status {
    kOk = 0,
    kAuthCardFail,
    kAuthServerFail,
  };
  
  const std::string &get_token(void) { return token_; } 


  /**
   * Performs validation of the user's pin with card, user & account, and then
   * gets a session token for further access to user account
   */
  Status create_session();
  Status destroy_session();

  Auth(const std::string &pin, const std::vector<uint8_t> &blob);
  ~Auth();

protected:
  
  std::string token_;
};

#endif  /* _AUTH_AUTH_H_ */
