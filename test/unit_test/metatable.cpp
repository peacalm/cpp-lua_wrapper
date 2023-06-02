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

#include "main.h"

static inline int always1__index(lua_State* L) {
  lua_pushinteger(L, 1);
  return 1;
}

TEST(metatable, metatable) {
  lua_wrapper l;

  auto push_mt = [&]() {
    if (luaL_newmetatable(l.L(), "always1_mt") == 1) {
      std::cout << "make new metatable always1_mt : \n";
      lua_pushcfunction(l.L(), always1__index);
      lua_setfield(l.L(), -2, "__index");
    }
  };

  // seek/touchtb using metatable_tag

  l.dostring("a={} b={} c={}");
  l.gseek("a");

  l.seek(lua_wrapper::metatable_tag{});
  EXPECT_TRUE(l.isnil());
  l.pop();
  l.touchtb(lua_wrapper::metatable_tag{});
  EXPECT_TRUE(l.eval<bool>("return a.x == nil"));

  push_mt();
  l.gseek("b");
  l.touchtb(lua_wrapper::metatable_tag{"always1_mt"});
  EXPECT_TRUE(l.eval<bool>("return b.x == 1"));

  // setfiled using metatable_tag

  l.pop();
  l.setfield(lua_wrapper::metatable_tag{}, nullptr);  // set nil to metatable
  EXPECT_TRUE(l.eval<bool>("return b.x == nil"));

  l.touchtb(lua_wrapper::metatable_tag{});
  l.setfield("__index",
             std::unordered_map<std::string, int>{{"x", 1}, {"y", 2}});
  EXPECT_TRUE(l.eval<bool>("return b.x == 1"));
  EXPECT_TRUE(l.eval<bool>("return b.y == 2"));
  EXPECT_TRUE(l.eval<bool>("return b.z == nil"));

  l.cleartop();
  l.gseek("c");
  std::unordered_map<std::string, std::unordered_map<std::string, int> > meta{
      {"__index", {{"x", 1}, {"y", 2}}}};
  l.setfield(lua_wrapper::metatable_tag{}, meta);
  EXPECT_TRUE(l.eval<bool>("return c.x == 1"));
  EXPECT_TRUE(l.eval<bool>("return c.y == 2"));
  EXPECT_TRUE(l.eval<bool>("return c.z == nil"));

  // ltouchtb using metatable_tag

  l.dostring("g = {gg = {} }");
  l.ltouchtb("g", "gg", lua_wrapper::metatable_tag{"always1_mt"});
  EXPECT_TRUE(l.eval<bool>("return g.gg.xxx == 1"));
}
