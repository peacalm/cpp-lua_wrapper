
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

#include <iostream>
//
#include <gtest/gtest.h>

#if defined(ENABLE_MYOSTREAM_WATCH)
#include <myostream.h>
#define watch(...)                        \
  std::cout << MYOSTREAM_WATCH_TO_STRING( \
      std::string, " = ", "\n", "\n\n", __VA_ARGS__)
#define watch_with_std_cout(...) \
  MYOSTREAM_WATCH(std::cout, " = ", "\n", "\n\n", __VA_ARGS__)
#else
#define watch(...)
#define watch_with_std_cout(...)
#endif

#include "peacalm/lua_wrapper.h"

using namespace peacalm;

TEST(lua_wrapper, print_type_conversions_by_myostream) {
  lua_wrapper l;

#define full_watch(info)          \
  watch(info,                     \
        lua_isinteger(l.L(), -1), \
        lua_isnumber(l.L(), -1),  \
        lua_isstring(l.L(), -1),  \
        lua_isboolean(l.L(), -1), \
        l.to_llong(-1),           \
        l.to_ullong(-1),          \
        l.to_double(-1),          \
        l.to_string(-1),          \
        l.to_bool(-1),            \
        lua_tointeger(l.L(), -1), \
        lua_tonumber(l.L(), -1),  \
        lua_tostring(l.L(), -1),  \
        lua_toboolean(l.L(), -1))

  lua_pushinteger(l.L(), 0);
  full_watch("pushinteger 0");

  lua_pushinteger(l.L(), 2);
  full_watch("pushinteger 2");

  lua_pushinteger(l.L(), LLONG_MAX);
  full_watch("pushinteger LLONG_MAX");

  lua_pushinteger(l.L(), 7213265539493896576);
  full_watch("pushinteger 7213265539493896576");

  lua_pushnumber(l.L(), 2.5);
  full_watch("pushnumber 2.5");

  lua_pushnumber(l.L(), 2.0);
  full_watch("pushnumber 2.0");

  lua_pushnumber(l.L(), 0.0);
  full_watch("pushnumber 0.0");

  lua_pushstring(l.L(), "2.5");
  full_watch("pushstring '2.5'");

  lua_pushstring(l.L(), "0");
  full_watch("pushstring '0'");

  lua_pushstring(l.L(), "7213265539493896576");
  full_watch("pushstring '7213265539493896576'");
  watch((long long)(double)(7213265539493896576));

  lua_pushstring(l.L(), "+7213265539493896576");
  full_watch("pushstring '+7213265539493896576'");

  lua_pushstring(l.L(), "-7213265539493896576");
  full_watch("pushstring '-7213265539493896576'");

  lua_pushstring(l.L(), "12345678901234567890");  // > 2^63-1, <2^64-1
  full_watch("pushstring '12345678901234567890'");

  lua_pushstring(l.L(), "123456789012345678901234567890");  // > 2^64-1
  full_watch("pushstring '123456789012345678901234567890'");

  lua_pushstring(l.L(), "abc");
  full_watch("pushstring 'abc'");

  lua_pushboolean(l.L(), true);
  watch("pushboolean true",
        lua_isinteger(l.L(), -1),
        lua_isnumber(l.L(), -1),
        lua_isstring(l.L(), -1),
        lua_isboolean(l.L(), -1),
        l.to_llong(-1),
        l.to_double(-1),
        l.to_string(-1),
        l.to_bool(-1),
        lua_tointeger(l.L(), -1),
        lua_tonumber(l.L(), -1),
        // lua_tostring(l.L(), -1), // panic
        lua_toboolean(l.L(), -1));
}

TEST(lua_wrapper, type_conversion) {
  lua_wrapper l;

  // NONE
  std::cout << "Lua always convert NONE to 0" << std::endl;
  EXPECT_EQ(lua_toboolean(l.L(), -1), 0);
  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);
  EXPECT_EQ(lua_tonumber(l.L(), -1), 0);
  EXPECT_EQ(lua_tostring(l.L(), -1), nullptr);
  std::cout << "We convert NONE to default" << std::endl;
  EXPECT_EQ(l.to_bool(-1), false);
  EXPECT_EQ(l.to_bool(-1, true), true);
  EXPECT_EQ(l.to_int(-1), 0);
  EXPECT_EQ(l.to_llong(-1, -1), -1);
  EXPECT_EQ(l.to_double(-1, 1.5), 1.5);
  EXPECT_EQ(l.to_string(-1), "");

  // NIL
  l.settop(0);
  lua_pushnil(l.L());
  std::cout << "Lua always convert NIL to 0" << std::endl;
  EXPECT_EQ(lua_toboolean(l.L(), -1), 0);
  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);
  EXPECT_EQ(lua_tonumber(l.L(), -1), 0);
  EXPECT_EQ(lua_tostring(l.L(), -1), nullptr);
  std::cout << "We convert NIL to default" << std::endl;
  EXPECT_EQ(l.to_bool(-1), false);
  EXPECT_EQ(l.to_bool(-1, true), true);
  EXPECT_EQ(l.to_int(-1), 0);
  EXPECT_EQ(l.to_llong(-1, -1), -1);
  EXPECT_EQ(l.to_double(-1, 1.5), 1.5);
  EXPECT_EQ(l.to_string(-1), "");

  // Boolean
  l.settop(0);
  lua_pushboolean(l.L(), int(true));
  std::cout << "Lua doesn't convert bool to other types, return 0 on fail"
            << std::endl;
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);  // true to 0
  EXPECT_EQ(lua_tonumber(l.L(), -1), 0);   // true to 0
  EXPECT_EQ(lua_tostring(l.L(), -1), nullptr);
  std::cout << "We convert bool to number" << std::endl;
  EXPECT_EQ(l.to_bool(-1), true);
  EXPECT_EQ(l.to_int(-1), 1);          // convert true to 1
  EXPECT_EQ(l.to_llong(-1, -1), 1);    // convert true to 1
  EXPECT_EQ(l.to_double(-1, 2.5), 1);  // convert true to 1
  EXPECT_EQ(l.to_string(-1), "");      // can't convert to string

  // integer
  std::cout << "Lua convert any integer to bool true" << std::endl;
  l.settop(0);
  lua_pushinteger(l.L(), 0);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));  // 0 to true!
  l.settop(0);
  lua_pushinteger(l.L(), 1);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  l.settop(0);
  lua_pushinteger(l.L(), -1);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  l.settop(0);
  lua_pushinteger(l.L(), INT_MAX);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));

  l.settop(0);
  lua_pushinteger(l.L(), 3);
  std::cout << "Lua convert integer to bool, number and string" << std::endl;
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(lua_tointeger(l.L(), -1), 3);
  EXPECT_EQ(lua_tonumber(l.L(), -1), 3);
  EXPECT_EQ(strcmp(lua_tostring(l.L(), -1), "3"), 0);
  std::cout << "We convert integer to bool, number and string" << std::endl;
  EXPECT_EQ(l.to_bool(-1), true);  // integer to true
  EXPECT_EQ(l.to_int(-1), 3);
  EXPECT_EQ(l.to_llong(-1, -1), 3);
  EXPECT_EQ(l.to_double(-1, 2.5), 3);
  EXPECT_EQ(l.to_string(-1, ""), std::string("3"));  // convert to string

  l.settop(0);
  lua_pushinteger(l.L(), 0);
  std::cout << "Lua convert integer 0 to boolean true!\n";
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(strcmp(lua_tostring(l.L(), -1), "0"), 0);
  std::cout << "We convert integer 0 to bool false" << std::endl;
  EXPECT_EQ(l.to_bool(-1), false);  // 0 to false
  EXPECT_EQ(l.to_int(-1), 0);
  EXPECT_EQ(l.to_string(-1, ""), std::string("0"));  // convert to string

  // string
  std::cout << "Lua convert any string to bool true" << std::endl;
  l.settop(0);
  lua_pushstring(l.L(), "");
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));  // '' to true
  l.settop(0);
  lua_pushstring(l.L(), "0");
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));  // '0' to true
  l.settop(0);
  lua_pushstring(l.L(), "1");
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  l.settop(0);
  lua_pushstring(l.L(), "abc");
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));

  std::cout << "Lua convert number like string to number" << std::endl;
  l.settop(0);
  lua_pushstring(l.L(), "-123");
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(lua_tointeger(l.L(), -1), -123);
  EXPECT_EQ(lua_tonumber(l.L(), -1), -123);
  EXPECT_EQ(strcmp(lua_tostring(l.L(), -1), "-123"), 0);

  std::cout << "Lua convert other string to 0" << std::endl;
  l.settop(0);
  lua_pushstring(l.L(), "other");
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);
  EXPECT_EQ(lua_tonumber(l.L(), -1), 0);

  std::cout << "We convert number like string to it's literal value. "
               "Especially '0' to false!"
            << std::endl;
  l.settop(0);
  lua_pushstring(l.L(), "-123");
  EXPECT_EQ(l.to_bool(-1), true);
  EXPECT_EQ(l.to_int(-1), -123);
  EXPECT_EQ(l.to_llong(-1, -1), -123);
  EXPECT_EQ(l.to_double(-1, 2.5), -123);
  EXPECT_EQ(l.to_string(-1), std::string("-123"));
  l.settop(0);
  lua_pushstring(l.L(), "0");
  EXPECT_EQ(l.to_bool(-1), false);

  std::cout << "We can't convert non-number-like-string to other types"
            << std::endl;
  l.settop(0);
  lua_pushstring(l.L(), "non-number-like-string");
  EXPECT_EQ(l.to_bool(-1), false);       // error, return default
  EXPECT_EQ(l.to_int(-1), 0);            // error, return default
  EXPECT_EQ(l.to_llong(-1, -1), -1);     // error, return default
  EXPECT_EQ(l.to_double(-1, 2.5), 2.5);  // error, return default
  EXPECT_EQ(l.to_string(-1), std::string("non-number-like-string"));

  EXPECT_EQ(l.gettop(), 1);

  // number
  std::cout << "Lua convert number to bool, string. 0 to true! 0 to '0.0'!"
            << std::endl;
  l.settop(0);
  lua_pushnumber(l.L(), 0);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));  // 0.0 to true
  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);
  EXPECT_EQ(lua_tonumber(l.L(), -1), 0);
  EXPECT_EQ(strcmp(lua_tostring(l.L(), -1), "0.0"), 0);
  std::cout << "We convert number to bool, string. 0 to false! 0 to '0.0'"
            << std::endl;
  EXPECT_EQ(l.to_bool(-1), false);  // integer 0 to false
  EXPECT_EQ(l.to_int(-1), 0);
  EXPECT_EQ(l.to_llong(-1, -1), 0);
  EXPECT_EQ(l.to_double(-1, 2.5), 0);
  EXPECT_EQ(l.to_string(-1), std::string("0.0"));  // convert to string

  l.settop(0);
  lua_pushnumber(l.L(), 1.0);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(l.to_string(-1), std::string("1.0"));  // convert to string

  EXPECT_EQ(lua_tointeger(l.L(), -1), 1);  // Lua: 1.0 to 1
  EXPECT_EQ(l.to_int(-1), 1);              // we: 1.0 to 1

  l.settop(0);
  lua_pushnumber(l.L(), 1.5);
  EXPECT_EQ(lua_toboolean(l.L(), -1), int(true));
  EXPECT_EQ(l.to_string(-1), std::string("1.5"));  // convert to string

  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);  // Lua: 1.5 to 0
  EXPECT_EQ(l.to_int(-1), 1);              // we: 1.5 to 1

  EXPECT_EQ(l.gettop(), 1);
}

TEST(lua_wrapper, long_number_like_string) {
  lua_wrapper l;
  const char *s = "123456789012345678901234567890";
  double      d = std::stod(s);
  lua_pushstring(l.L(), s);

  EXPECT_EQ(lua_tonumber(l.L(), -1), d);
  EXPECT_EQ(l.to_double(-1), d);
  // Lua can't convert integer if oversize, got 0
  EXPECT_EQ(lua_tointeger(l.L(), -1), 0);

  // We use C++ style static_cast, the result may be different by different
  // Compiler or Platform
  EXPECT_EQ(l.to_llong(-1), static_cast<long long>(d));

  EXPECT_EQ(l.gettop(), 1);

  l.reset();
  l.set_string("bignum", s);

  // The result may be different by different Compiler or Platform
  EXPECT_EQ(l.get_llong("bignum"),
            static_cast<long long>(d));  // value LLONG_MAX or LLONG_MIN
  EXPECT_EQ(l.get_ullong("bignum"),
            static_cast<unsigned long long>(d));  // value 0 or ULLONG_MAX

  EXPECT_EQ(l.get_double("bignum"), d);
  EXPECT_EQ(l.gettop(), 0);

  l.set_integer("bignum", LLONG_MAX);
  EXPECT_EQ(l.get_llong("bignum"), LLONG_MAX);
  EXPECT_EQ(l.get_ullong("bignum"), LLONG_MAX);
  EXPECT_EQ(l.get_double("bignum"), static_cast<double>(LLONG_MAX));

  l.set_integer("bignum", LLONG_MIN);
  EXPECT_EQ(l.get_llong("bignum"), LLONG_MIN);
  EXPECT_EQ(l.get_ullong("bignum"), 1ull << 63);

  // No ullong in Lua, this is equal to set -1 to 'bignum'
  l.set_integer("bignum", ULLONG_MAX);
  EXPECT_EQ(l.get_ullong("bignum"),
            ULLONG_MAX);  // we convert -1 to ull_max in C++
  EXPECT_EQ(l.get_llong("bignum"), -1);
  EXPECT_EQ(l.get_double("bignum"), -1);

  EXPECT_EQ(l.gettop(), 0);
}

TEST(lua_wrapper, large_number) {
  lua_wrapper l;
  l.reset();
  const char *s = "1921332203851725413";
  long long   i = std::stoll(s);
  assert(std::to_string(i) == s);
  double d = std::stod(s);
  l.set_string("s", s);
  lua_pushstring(l.L(), s);

  watch(s,
        lua_isinteger(l.L(), -1),
        lua_tointeger(l.L(), -1),
        l.get_llong("s"),
        std::stoll(s),
        std::stod(s),
        (long long)std::stod(s));

  EXPECT_EQ(l.get_llong("s"), i);
  EXPECT_EQ(l.get_llong("s"), d);
  EXPECT_EQ(l.get_double("s"), d);
  EXPECT_EQ(l.get_double("s"), i);
  EXPECT_NE(l.get_llong("s"), (long long)d);
  EXPECT_EQ((long long)l.get_double("s"), (long long)d);
  EXPECT_EQ(l.get_double("s"), double((long long)d));

  i = 6773679268829351174LL;
  l.set_integer("i", i);
  EXPECT_EQ(l.get_llong("i"), i);
  EXPECT_NE((long long)l.get_double("i"), i);
  EXPECT_EQ(l.get_double("i"), i);
}

TEST(lua_wrapper, set_and_get) {
  lua_wrapper l;

  l.set_boolean("b", true);
  l.set_integer("i", 5);
  l.set_number("f", 3.14);
  l.set_string("s", "Hello Lua!");

  EXPECT_EQ(l.gettop(), 0);

  EXPECT_EQ(l.get_bool("b"), true);
  EXPECT_EQ(l.get_int("i"), 5);
  EXPECT_EQ(l.get_double("f"), 3.14);
  EXPECT_EQ(l.get_string("s"), "Hello Lua!");
  EXPECT_EQ(strcmp(l.get_c_str("s"), "Hello Lua!"), 0);
  // get_c_str won't pop
  EXPECT_EQ(l.gettop(), 1);
  l.pop();  // we pop

  // max integer, min interger, -1
  l.set_integer("imax", LLONG_MAX);
  EXPECT_EQ(l.get_llong("imax"), LLONG_MAX);
  EXPECT_EQ(l.get_ullong("imax"), LLONG_MAX);
  EXPECT_EQ(l.get_int("imax"), -1);
  EXPECT_EQ(l.get_uint("imax"), UINT_MAX);
  l.set_integer("imin", LLONG_MIN);
  EXPECT_EQ(l.get_llong("imin"), LLONG_MIN);
  EXPECT_EQ(l.get_ullong("imin"), LLONG_MAX + 1);
  EXPECT_EQ(l.get_int("imin"), 0);
  EXPECT_EQ(l.get_uint("imin"), 0);
  l.set_integer("n1", -1);
  EXPECT_EQ(l.get_llong("n1"), -1);
  EXPECT_EQ(l.get_ullong("n1"), ULLONG_MAX);
  EXPECT_EQ(l.get_int("n1"), -1);
  EXPECT_EQ(l.get_uint("n1"), UINT_MAX);

  // clear
  l.set_nil("n1");
  EXPECT_EQ(l.get_int("n1"), 0);
  EXPECT_EQ(l.get_uint("n1"), 0);
  EXPECT_EQ(l.get_llong("n1"), 0);
  EXPECT_EQ(l.get_ullong("n1"), 0);

  // return default value
  EXPECT_EQ(l.get_int("n1", 1), 1);
  EXPECT_EQ(l.get_uint("n1", 2), 2);
  EXPECT_EQ(l.get_llong("n1", 3), 3);
  EXPECT_EQ(l.get_ullong("n1", 4), 4);

  EXPECT_EQ(l.get_string("nx", "def"), "def");
  EXPECT_EQ(l.get_string("nx"), "");

  EXPECT_EQ(l.gettop(), 0);

  // type conversion
  EXPECT_EQ(l.get_int("b"), 1);
  EXPECT_EQ(l.get_int("f"), 3);
  EXPECT_EQ(l.get_bool("i"), true);
  EXPECT_EQ(l.get_bool("f"), true);
  EXPECT_EQ(l.get_double("b"), 1);
  EXPECT_EQ(l.get_double("i"), 5);
  l.set_integer("i0", 0);
  EXPECT_EQ(l.get_bool("i0"), false);
  EXPECT_EQ(l.get_bool("none"), false);
  EXPECT_EQ(l.get_bool("none", true), true);
  l.set_boolean("bfalse", false);
  EXPECT_EQ(l.get_int("bfalse"), 0);
  EXPECT_EQ(l.get_int("bfalse", 1), 0);

  EXPECT_EQ(l.gettop(), 0);

  // number like string  <-> number
  l.set_string("si", "3.14");
  EXPECT_EQ(l.get_int("si"), 3);
  EXPECT_EQ(l.get_double("si"), 3.14);
  EXPECT_EQ(l.get_string("i"), "5");
  EXPECT_EQ(l.get_string("i0"), "0");
  EXPECT_EQ(l.get_string("f"), "3.14");

  EXPECT_EQ(l.gettop(), 0);

  // false conversion
  bool failed = false;
  EXPECT_EQ(l.get_bool("s", false, true, &failed), false);
  EXPECT_TRUE(failed);
  failed = false;
  EXPECT_EQ(l.get_bool("s", true, true, &failed), true);
  EXPECT_TRUE(failed);
  failed = false;
  EXPECT_EQ(l.get_int("s", 0, true, &failed), 0);
  EXPECT_TRUE(failed);
  failed = false;
  EXPECT_EQ(l.get_int("s", -1, true, &failed), -1);
  EXPECT_TRUE(failed);

  EXPECT_EQ(l.gettop(), 0);

  l.set_string("btrue", "true");
  EXPECT_EQ(l.get_bool("btrue"), false);
  EXPECT_EQ(l.get_int("btrue"), 0);

  EXPECT_EQ(l.get_string("b"), "");
  l.set_boolean("b", false);
  EXPECT_EQ(l.get_string("b"), "");

  EXPECT_EQ(l.gettop(), 0);
}

TEST(lua_wrapper, reset) {
  lua_wrapper l;

  l.set_boolean("b", true);
  l.set_integer("i", 5);
  l.set_number("f", 3.14);
  l.set_string("s", "Hello Lua!");

  l.reset();
  EXPECT_EQ(l.get_bool("b"), false);
  EXPECT_EQ(l.get_int("i"), 0);
  EXPECT_EQ(l.get_double("f"), 0);
  EXPECT_EQ(l.get_string("s"), "");
}

TEST(lua_wrapper, enable_log) {
  lua_wrapper l;

  l.set_string("x", "enable log once then disable!");
  EXPECT_EQ(l.get_bool("x", false, true), false);
  EXPECT_EQ(l.get_bool("x", false, false), false);
  EXPECT_EQ(l.get_int("x", 0, false), 0);

  l.set_boolean("b", true);
  EXPECT_EQ(l.get_string("b", "", false), "");
  l.set_boolean("b", false);
  EXPECT_EQ(l.get_string("b", "", false), "");

  EXPECT_EQ(l.gettop(), 0);
}

TEST(lua_wrapper, eval) {
  lua_wrapper l;

  // Error! Lua returns '', C++ returns default and prints an error log
  EXPECT_EQ(l.eval_bool("return ''"), false);
  // OK! Lua converts '' to true
  EXPECT_EQ(l.eval_bool("return not not ''"), true);

  EXPECT_EQ(l.eval_bool("return 0"), false);
  EXPECT_EQ(l.eval_bool("return 1"), true);
  EXPECT_EQ(l.eval_bool("return -1"), true);
  EXPECT_EQ(l.eval_bool("return 123"), true);

  EXPECT_EQ(l.eval_int("return 2^3"), 8);
  EXPECT_EQ(l.eval_int("return 2^3 - 9"), -1);

  EXPECT_EQ(l.eval_double("return 3/2"), 1.5);
  EXPECT_EQ(l.eval_double("return 3//2"), 1);

  EXPECT_EQ(l.eval_string("return 'Hello'"), "Hello");
  EXPECT_EQ(l.eval_string("if 0 then return 'A' else return 'B' end"), "A");
  EXPECT_EQ(l.eval_string("if false then return 'A' else return 'B' end"), "B");

  EXPECT_EQ(l.gettop(), 0);

  l.set_integer("a", 1);
  l.set_integer("b", 2);
  l.set_integer("c", 3);
  l.set_integer("d", 4);
  EXPECT_EQ(l.eval_int("return a + b + c + d"), 10);
  l.dostring("e = a + b + c + d");
  EXPECT_EQ(l.get_int("e"), 10);
  EXPECT_EQ(l.eval_int("return e"), 10);

  EXPECT_EQ(l.eval_double("return a + b * c / d"), 1 + 2 * 3 / 4.0);

  EXPECT_EQ(l.gettop(), 0);

  l.eval_string("s = 'a' .. '0' ");
  l.eval_int("return 1,2,3");
  l.get_string("s");

  watch(l.eval_string("s = 'a' .. '0' "),
        l.eval_int("return 1,2,3"),
        l.get_string("s"));

  EXPECT_EQ(l.gettop(), 0);
}

struct vprovider {
  // vprovider() { puts("vprovider()"); }
  // ~vprovider() { puts("~vprovider()"); }
  void provide_one(const std::string &v, lua_wrapper *l) {
    l->set_integer(v, 1);
  }
  void provide_variables(const std::vector<std::string> &vars, lua_wrapper *l) {
    for (const auto &v : vars) provide_one(v, l);
  }
};

TEST(custom_lua_wrapper_is_provider, auto_eval) {
  custom_lua_wrapper_is_provider<vprovider> l(luaL_newstate());
  EXPECT_EQ(l.auto_eval_int("return a + b + c"), 3);
}

TEST(custom_lua_wrapper_has_provider, auto_eval) {
  {
    custom_lua_wrapper_has_provider<vprovider> l(luaL_newstate());
    EXPECT_EQ(l.auto_eval_int("return a"), 1);
  }

  {
    custom_lua_wrapper_has_provider<vprovider *> l(luaL_newstate());
    l.provider(new vprovider);
    EXPECT_EQ(l.auto_eval_int("return a + b"), 2);
    delete l.provider();
  }
  {
    custom_lua_wrapper_has_provider<std::shared_ptr<vprovider> > l(
        luaL_newstate());
    l.provider(std::make_shared<vprovider>());
    EXPECT_EQ(l.auto_eval_int("return a + b + c"), 3);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  std::cout << ">>> Running lua_wrapper unit test." << std::endl;

  int ret = RUN_ALL_TESTS();

  return ret;
}