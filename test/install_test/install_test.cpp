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

#include <iostream>

int main() {
  peacalm::lua_wrapper l;
  std::cout << l.eval_string("return 'Hello ' .. 'LuaWrapper!'") << std::endl;
  return 0;
}