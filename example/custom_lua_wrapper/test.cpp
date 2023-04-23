// Copyright (c) 2023 Shuangquan Li. All Rights Reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of the License
// at
//
//   http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

#include <peacalm/lua_wrapper.h>

#include <cstdio>
#include <iostream>
#include <memory>

struct provider {
  provider() { puts("provider()"); }
  ~provider() { puts("~provider()"); }
  bool provide(lua_State *L, const char *vname) {
    if (strcmp(vname, "a") == 0)
      lua_pushinteger(L, 1);
    else if (strcmp(vname, "b") == 0)
      lua_pushinteger(L, 2);
    else if (strcmp(vname, "c") == 0)
      lua_pushinteger(L, 3);
    else
      return false;
    return true;
  }
};
using provider_type = std::unique_ptr<provider>;

int main() {
  peacalm::custom_lua_wrapper<provider_type> l;
  l.provider(std::make_unique<provider>());
  double ret = l.eval_double("return a*10 + b^c");
  std::cout << ret << std::endl;  // 18
}