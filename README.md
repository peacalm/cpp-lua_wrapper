# Luaw : Lua Wrapper for C++

[![Build](https://github.com/peacalm/cpp-luaw/actions/workflows/ci.yml/badge.svg)](https://github.com/peacalm/cpp-luaw/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

This library is a simple wrapper of Lua which makes interaction between Lua and 
C++ easier.

It can be used as a binder, a config file parser or a dynamic expression evaluator.

Features:
* Get Lua values in C++
* Set C++ values to Lua
* Get and call Lua functions in C++
* Bind C++ functions to Lua (including C function, lambda, std::function and user defined callable objects)
* Bind C++ classes to Lua (can bind constructors, member variables and member functions, etc)
* Bind C++ copyable objects, movable objects, smart pointer of objects or raw pointer of objects to Lua
* Evaluate Lua expressions to get results in C++
* If a variable provider is provided, it can automatically seek variables from 
provider while evaluating expressions in C++.

This lib depends only on Lua:
* Lua version >= 5.4
* C++ version >= C++14

## Detailed Features

<table style="width: 200%; table-layout: fixed;">
<colgroup>
    <col span="1" style="width: 45%;">
    <col span="1" style="width: 10%;">
</colgroup>

<tr>
  <th> Feature ++++++++++++++++++++++++++++++++++++++++ </th>
  <th> Supported? </th>
  <th> Example </th>
</tr>


<tr>
  <td> <ul><li> Get Lua values in C++ </li></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// Define a luaw instance, which is used by the following examples
peacalm::luaw l;
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Get simple type values (bool/integer/float number/string) </li></ul></ul> </td>
  <td> ✅ get_xxx </td>
  <td>

```C++
int retcode = l.dostring("i = 1; b = true; f = 2.5; s = 'luastring';");
if (retcode != LUA_OK) {
  // error handler...
}
int i = l.get_int("i");
long long ll = l.get_llong("i");
bool b = l.get_bool("b");
double f = l.get_double("f");
std::string s = l.get_string("s");

// Or use alternative writings:
int i = l.get<int>("i");
long long ll = l.get<long long>("i");
bool b = l.get<bool>("b");
double f = l.get<double>("f");
std::string s = l.get<std::string>("s");
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Get complex container type values 
  (std::pair, std::tuple, std::vector, std::map, std::unordered_map, std::set, std::unordered_set, std::deque, std::list, std::forward_list) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
int retcode = l.dostring("a = {1,2,3}; b = {x=1,y=2}; c = {x={1,2},y={3,4}}; d = {true, 1, 'str'}");
if (retcode != LUA_OK) {
  // error handler...
}
auto a = l.get<std::vector<int>>("a");
auto b = l.get<std::map<std::string, int>>("b");
auto c = l.get<std::unordered_map<std::string, std::vector<int>>>("c");
auto d = l.get<std::tuple<bool, int, std::string>>("d");

// About set: only collect keys of a table in Lua into a C++ container set.
l.dostring("ss={a=true,b=true,c=true}; si={}; si[1]=true si[2]=true;");
auto ss = l.get<std::set<std::string>>("ss"); // ss == std::set<std::string>{"a", "b", "c"}
auto si = l.get<std::set<int>>("si"); // si == std::set<int>{1, 2}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Tell whether the target exists or whether the operation fails 
  (we don't regard target's non-existence as fail) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
bool failed, exists;
auto i = l.get<int>("i", /* disable log */ false, &failed, &exists);
if (failed) {
  // failure handers
}
if (!exists) {
  // ...
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Support a default value on target does not exist or operation fails </li></ul></ul> </td>
  <td> ✅ only for simple types (get_xxx) </td>
  <td>

```C++
// API supports default value is defined as get_xxx:
auto i = l.get_int("i", -1); // default value of i is -1
auto s = l.get_string("s", "def"); // default value of s is 'def'

// Donot support defult value for containers:
// auto a = l.get<std::vector<int>>("a", /* disable log */ false, &failed, &exists);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Recursively get element in table by a given path </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
peacalm::luaw l;
int retcode = l.dostring("a=true; m={p1={x=1,y=2},p2={x=3.2,y=4.5}}");
if (retcode != LUA_OK) {
  // error handlers...
}

bool a = l.get_bool({"a"});  // same to l.get_bool("a"), a == true
int p1x = l.get_int({"m", "p1", "x"}, -1); // p1x == 1
double p2y = l.get<double>({"m", "p2", "y"}); // p2y == 4.5
// p2 = std::map<std::string, double>{{"x", 3.2}, {"y", 4.5}}
auto p2 = l.get<std::map<std::string, double>>({"m", "p2"}); 
```
  </td>
</tr>

<tr>
  <td> <ul><li> Set C++ values to Lua</li></ul> </td>
  <td> ✅ </td>
  <td>
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set simple type values (bool/integer/float number/string) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
l.set("b", true);
l.set("i", 123);
l.set("f", 123.45);
l.set("s", "cstr");
l.set("s2", std::string("std::string"));
// Alternative writings may use: 
// set_boolean/set_integer/set_number/set_string
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set complex container type values 
  (std::pair, std::tuple, std::vector, std::map, std::unordered_map, std::set, std::unordered_set, std::deque, std::list, std::forward_list) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// All containers map to Lua table
l.set("a", std::vector<int>{1,2,3});
l.set("b", std::map<std::string, int>{{"a",1},{"b",2}});
l.set("c", std::make_pair("s", true)); // c[1] == "s", c[2] == true
// Map set to table whose keys are from C++ set and values are boolean true.
l.set("s", std::set<int>{1,2,3});
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set nullptr to Lua (Equivalent to set nil to a variable in Lua) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// Equivalent to run "x = nil" in Lua
l.set("x", nullptr);
// Alternative writing:
l.set_nil("x");
```
  </td>
</tr>


<tr>
  <td> <ul><li> Get and call Lua functions in C++ </li></ul> </td>
  <td> ✅ </td>
  <td>

```Lua
-- Functions defined in Lua
fadd = function(a, b) return a + b end
fdiv = function(a, b) return a // b, a % b end
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Call a Lua function directly </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// Should at least provide result type
int s = l.callf<int>("fadd", 1, 2); // s = 1 + 2
assert(s == 3);
// Or provide result type and some argument types
auto d = l.callf<double, double, double>("fadd", 1.25, 2.5);
assert(d == 3.75);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Get a std::function object to represent the Lua function </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
auto f = l.get<std::function<int(int, int)>>("fadd");
int s = f(1,2);
```
  </td>
</tr>

<tr>
  <td>
    <ul><ul>
      <li> Get a luaw::function object to represent the Lua function, which can: </li>
      <ul>
        <li> Tell whether call the Lua function and get result successfully </li>
        <li> Tell whether the function failed while running in Lua </li>
        <li> Tell whether the Lua function exists </li>
        <li> Tell whether converting the Lua function's result to C++ fails </li>
        <li> Tell whether the Lua function returns result (whether result exists) </li>
        <li> Tell whether got enough results (Lua could return more, couldn't less) </li>
        <li> Tell how many results the Lua function returned </li>
      </ul>
    </ul></ul> 
  </td>

  <td> ✅ use luaw::function </td>
  <td>

```C++
auto f = l.get<peacalm::luaw::function<int(int, int)>>("fadd");
int s = f(1,2);
// After call, check:
if (f.failed()) {
  if (f.function_failed()) {
    // ...
  } else if (!f.function_exists()) {
    // ...
  } else if (f.result_failed()) {
    // ...
  } else if (!f.result_enough()) {
    std::cout << "Result number of f " << f.real_result_size()
              << " less than " << f.expected_result_size() << std::endl;
  } else {
    // May never happen
  }
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Get/Call a Lua function with multiple results </li></ul></ul> </td>
  <td> ✅ use std::tuple </td>
  <td>

```C++
// Call it directly
auto q = l.callf<std::tuple<int, int>>("fdiv", 7, 3); // q == make_tuple(2, 1)
assert(std::get<0>(q) == 2 && std::get<1>(q) == 1);

// Get a function object first
auto f = l.get<peacalm::luaw::function<std::tuple<int,int>(int, int)>>("fdiv");
auto q2 = f(7, 3);
assert(q2 == std::make_tuple(2, 1));
```
  </td>
</tr>


<tr>
  <td> <ul><li> Bind C++ functions to Lua (also lambda, std::function or callable objects) </li></ul> </td>
  <td> ✅ </td>
  <td>
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Bind C style functions to Lua</li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// The C function following examples may use
int fadd(int x, int y) { return a + b; }
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set C function directly </li></ul></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
l.set("fadd", fadd);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set C function pointer </li></ul></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
l.set("fadd", &fadd);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set C function reference </li></ul></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
auto& ref = fadd;
l.set("fadd", ref);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set C function with arbitrary number of arguments </li></ul></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
int add_many(int a, int b, int c, int d, /* define any number of arguments */) {
  // ...
}
int main() {
  peacalm::luaw l;
  l.set("add_many", add_many);
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set C style variadic function </li></ul></ul></ul> </td>
  <td> ❌ </td>
  <td>

```C++
// A function like printf
int vf(const char* s, ...) { /* some codes */ }
// l.set("vf", vf); // error! not supported
```
  </td>
</tr>


<tr>
  <td> <ul><ul><li> Bind C++ overloaded functions to Lua </li></ul></ul> </td>
  <td> ✅ should provide hint type</td>
  <td>

```C++
// Overloaded functions f
int f(int i) { return i * 2; }
double f(double d) { return d / 2; }
double f(double a, double b) { return a * b; }
int main() {
  peacalm::luaw l;
  // Provide the function proto type as hint
  l.set<int(int)>("f1", f);
  l.set<double(double)>("f2", f);
  l.set<double(double, double)>("f3", f);
  // Or could use function pointer proto type as hint
  l.set<int(*)(int)>("f1", f);
  l.set<double(*)(double)>("f2", f);
  l.set<double(*)(double, double)>("f3", f);
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Bind C++ template functions to Lua </li></ul></ul> </td>
  <td> ✅ should specialize or provide hint type</td>
  <td>

```C++
template <typename T>
T tadd(T a, T b) { return a + b; }
int main() {
  peacalm::luaw l;
  // Explicitly specialize the function
  l.set("tadd", tadd<int>);
  // Or provide the function proto type as hint
  l.set<double(double, double)>("tadd", tadd);
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Bind C++ lambdas to Lua </li></ul></ul> </td>
  <td> ✅ </td>
  <td>
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set captureless lambdas to Lua </li></ul></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// May not provide hint type
l.set("add", [](int a, int b) { return a + b; });
// Or alternative writings:
l.set<int(int, int)>("add", [](int a, int b) { return a + b; });
l.set<peacalm::luaw::function_tag>("add", [](int a, int b) { return a + b; });
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set not-captureless lambdas to Lua </li></ul></ul></ul> </td>
  <td> ✅ should provide hint type </td>
  <td>

```C++
int x = 1;
auto f = [&](int a) { return a + x; };

// Provide a function type hint: int(int)
l.set<int(int)>("add", f);

// Alternative writing: could use luaw::function_tag as hint type
l.set<peacalm::luaw::function_tag>("add", f);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><ul><li> Set generic lambdas to Lua </li></ul></ul></ul> </td>
  <td> ✅ should provide hint type </td>
  <td>

```C++
// Generic lambda
auto f = [](auto a, auto b) { return a + b; }

// Should provide a function proto type as hint
l.set<int(int, int)>("add", f);

// The lambda object can be reused with other function proto types
l.set<double(double, double)>("add2", f);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set std::function to Lua </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
std::function<int(int, int)> f = [](auto a, auto b) { return a + b; };

// Set f by copy:
l.set("f", f);

// Or set f by move:
l.set("f", std::move(f));
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Support C++ function with multiple returns </li></ul></ul> </td>
  <td> ✅ use std::tuple </td>
  <td>

```C++
std::tuple<int, int> fdiv(int a, int b) { return std::make_tuple(a / b, a % b); }

l.set("fdiv", fdiv);
l.dostring("q, r = fdiv(7, 3)"); // q == 2, r == 1
```
  </td>
</tr>


<tr>
  <td> <ul><ul><li> Support default arguments provided in C++ when binding C++ functions 
  (Bind C++ default arguments to Lua when binding C++ functions) </li></ul></ul> </td>
  <td> ❌ argumengts(not given real parameter in Lua) will be default-initialized 
  no matter what default values provided in C++ </td>
  <td>

```C++
// The default argument values 3 and 4 have no effect in Lua
std::tuple<int,int> point(int x = 3, int y = 4) {
  return std::make_tuple(x, y);
}
int main() {
  peacalm::luaw l;
  l.set("point", point);
  l.dostring("x, y = point(1)");
  assert(l.get_int("x") == 1);  // ok, 1 is explicitly provided
  assert(l.get_int("y") == 0);  // y will be default-initialized to 0, not 4
}
```
  </td>
</tr>

<tr>
  <td> <ul><li> Bind C++ classes to Lua </li></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// The C++ custom class to register
struct Obj {
  int       i  = 1;
  const int ci = 1;

  Obj() {}
  Obj(int v) : i(v) {}
  Obj(int v, int cv) : i(v), ci(cv) {}
  
  int abs() const { return std::abs(i); }

  int plus() { return ++i; }
  int plus(int d) { i += d; return i; }
};
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register constructor </li></ul></ul> </td>
  <td> ✅ should provide hint type </td>
  <td>

```C++
l.register_ctor<Obj()>("NewObj");          // default constructor
l.register_ctor<Obj(int)>("NewObj1");      // constructor with 1 argument
l.register_ctor<Obj(int, int)>("NewObj2"); // constructor with 2 argument2

// Then can use ctor as a global function in Lua
l.dostring("a = NewObj(); b = NewObj1(1); c = NewObj2(1,2)");
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register constructor for const object</li></ul></ul> </td>
  <td> ✅ should provide hint type </td>
  <td>

```C++
// The constructors will generate a const instance of Obj in Lua
using ConstObj = const Obj;  // or use std::add_const_t<Obj>
l.register_ctor<ConstObj()>("NewConstObj");          // default constructor
l.register_ctor<ConstObj(int)>("NewConstObj1");      // constructor with 1 argument
l.register_ctor<ConstObj(int, int)>("NewConstObj2"); // constructor with 2 argument2

// Then can use ctor as a global function in Lua
l.dostring("a = NewConstObj(); b = NewConstObj1(1); c = NewConstObj2(1,2)");
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register member variable </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
l.register_member("i", &Obj::i);
l.register_member("ci", &Obj::ci);  // register const member
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register member function </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
l.register_member("abs", &Obj::abs);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register overloaded member function </li></ul></ul> </td>
  <td> ✅ should provide hint type </td>
  <td>

```C++
l.register_member<int (Obj::*)()>("plus", &Obj::plus);
l.register_member<int (Obj::*)(int)>("plusby", &Obj::plus);
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register dynamic members (members whose name are dynamically defined in Lua, like keys of a table) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// If types of dynamic members are unknown, use luaw::luavalueref
struct Foo {
  std::unordered_map<std::string, peacalm::luaw::luavalueref> m;
};
peacalm::luaw::luavalueref foo_dm_getter(const Foo* o, const std::string& k) {
  auto entry = o->m.find(k);
  if (entry != o->m.end()) { return entry->second; }
  return peacalm::luaw::luavalueref(); // default value is nil
}
void foo_dm_setter(Foo* o, const std::string& k, const peacalm::luaw::luavalueref& v) {
  o->m[k] = v;
}
int main() {
  peacalm::luaw l;
  l.register_dynamic_member(foo_dm_getter, foo_dm_setter);
  int retcode = l.dostring("f.a = 1; f.b = true; f.c = 10.5; f.d = 'str'");
  // ...
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register fake member variables (members who are not really member of the class) </li></ul></ul> </td>
  <td> ✅ should provide a member type as hint </td>
  <td>

```C++
// Fake a const member "id" whose value is the object's address
l.register_member<void* const Obj::*>(
    "id", [](const volatile Obj* p) { return (void*)p; });

auto o = std::make_shared<Obj>();
l.set("o", o);
assert(l.eval<void*>("return o.id") == (void*)(o.get()));
```
---
```C++
int gi = 100;  // global i, member to be faked
int main() {
    peacalm::luaw l;
    // gi will be shared by all instances of Obj in Lua, like a static member
    l.register_member<int Obj::*>("gi", [&](const volatile Obj*) -> int& { return gi; });
    l.set("o", Obj{});
    assert(l.eval<int>("return o.gi") == gi);
    assert(l.eval<int>("o.gi = 101; return o.gi") == 101);
    assert(gi == 101);
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Register fake member functions </li></ul></ul> </td>
  <td> ✅ should provide a member type as hint </td>
  <td>

```C++
// Fake a member function "seti" for class Obj
l.register_member<void (Obj ::*)(int)>(
  "seti", [](Obj* p, int v) { p->i = v; });
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set class instance directly to Lua </li></ul></ul> </td>
  <td> ✅  </td>
  <td>

```C++
Obj o;
l.set("o", o); // by copy
// or
l.set("o", std::move(o)); // by move
// or
l.set("o", Obj{}); // by move
// The variable "o" can access all registered members of Obj

const Obj co;
l.set("co", co); // set const instance by copy
// or
l.set("co", std::move(co)); // set const instance by move
// or
l.set("co", std::add_const_t<Obj>{}); // set const instance by move
// The variable "co" is const, it can access all registered members variables 
// and registered const member functins of Obj.
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set class instance by smart pointer </li></ul></ul> </td>
  <td> ✅  </td>
  <td>

```C++
// shared_ptr
auto o = std::make_shared<Obj>();
l.set("o", o); // by copy
// or
l.set("o", std::move(o)); // by move

// unique_ptr
l.set("o", std::make_unique<Obj>()); // by move

// The variable "o" can access all registered members of Obj
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set class instance by raw pointer 
  (set a lightuserdata and set metatable simultaneously for not only it but for all lightuserdata) </li></ul></ul> </td>
  <td> ✅ (❗But NOT recommend) </td>
  <td>

```C++
Obj o;
l.set("o", &o); // set a lightuserdata which has metatable of class Obj
// The variable "o" can access all registered members of Obj
// Now all lightuserdata in Lua share the same metatable as "o"!
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set class instance by raw pointer of smart pointer 
  (set a lightuserdata and set metatable simultaneously for not only it but for all lightuserdata) </li></ul></ul> </td>
  <td> ✅ (❗But NOT recommend) </td>
  <td>

```C++
auto o = std::make_shared<Obj>();
l.set("o", &o); // set a lightuserdata with metatable of class Obj
// The variable "o" can access all registered members of Obj
// Now all lightuserdata in Lua share the same metatable as "o"!
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set address of class instance (no setting metatable) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
Obj o;
// Set address of o, which will be a lightuserdata without metatable if there 
// are no metatable installed for lightuserdata before!
l.set<void*>("o", &o);
// or alternative writing:
l.set("o", (void*)(&o));
// or alternative writing:
l.set<void*>("o", (void*)(&o));

// The variable "o" is a lightuserdata without metatable, so it can not 
// access members of Obj. But metatable can be installed later. 
// Then it can access members...
```
---
```C++
Obj o;
// Really set address of "o" as a number in Lua, not as lightuserdata
l.set("addr", reinterpret_cast<long long>(&o));
assert(l.get_llong("addr") == reinterpret_cast<long long>(&o));
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Set class instance by unique_ptr with custom deleter </li></ul></ul> </td>
  <td> ✅  </td>
  <td>

```C++
// Define deleter
struct ObjDeleter {
  void operator()(Obj* p) const { delete p; }
};

// ...

std::unique_ptr<Obj, ObjDeleter> o(new Obj, ObjDeleter{});
l.set("o", std::move(o));
// The variable "o" can access all registered members of Obj,
// just like that unique_ptr with default deleter.

l.close(); // ObjDeleter will be called when destructing "o"
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Const and volatile properties of objects, member variables and member functions are kept in Lua </li></ul></ul> </td>
  <td> ✅  </td>
  <td>

```C++
l.register_ctor<Obj()>("NewObj");
l.register_ctor<std::add_const_t<Obj>()>("NewConstObj"); // ctor for const object
l.register_member("i", &Obj::i);
l.register_member("ci", &Obj::ci); // const member
l.register_member<int (Obj::*)()>("plus", &Obj::plus); // nonconst member function

// Const property of member ci is kept in Lua
int retcode = l.dostring("o = NewObj(); o.ci = 3");
assert(retcode != LUA_OK);
l.log_error_out(); // error log: Const member cannot be modified: ci

l.set("o", std::add_const_t<Obj>{}); // "o" is const Obj
l.eval<void>("o:plus()"); // call a nonconst member function
// error log: Nonconst member function: plus

l.eval<void>("o = NewConstObj(); o:plus()");
// error log: Nonconst member function: plus

// Member of const object is also const
l.eval<void>("o = NewConstObj(); o.i = 3");
// error log: Const member cannot be modified: i
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Get object instance created in Lua </li></ul></ul> </td>
  <td> ✅  </td>
  <td>

```C++
l.register_member("i", &Obj::i);
l.register_ctor<Obj()>("NewObj");
l.dostring("a = NewObj(); a.i = 3;"); // creat a instance of Obj
Obj a = l.get<Obj>("a");
assert(a.i == 3);
assert(a.ci == 1);
```
  </td>
</tr>


<tr>
  <td> <ul><li> Evaluate a Lua script and get results </li></ul> </td>
  <td> ✅ </td>
  <td>

```C++
int ret = l.eval<int>("return 1");
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Return simple type result (bool/integer/float number/string) </li></ul></ul> </td>
  <td> ✅ eval_xxx </td>
  <td>

```C++
bool b = l.eval_bool("return 1 > 0");
int i = l.eval_int("return 1 + 2");
double d = l.eval_double("return 1.2 * 3.4");
std::string s = l.eval_string("return 'hello' .. ' world' ");

// Alternative writing:
bool = l.eval<bool>("return 1 > 0");
int i = l.eval<int>("return 1 + 2");
// ...
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Return complex type result (std::pair, std::tuple, std::vector, std::map, std::unordered_map, std::set, std::unordered_set, std::deque, std::list, std::forward_list) </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
auto v = l.eval<std::vector<int>>("return {1,2,3}");
auto m = l.eval<std::unordered_map<std::string, int>>("return {x=1,y=2}");
```
  </td>
</tr>


<tr>
  <td> <ul><ul><li> Return multiple values </li></ul></ul> </td>
  <td> ✅ use std::tuple </td>
  <td>

```C++
auto ret = l.eval<std::tuple<int, int, int>>("return 1,2,3");
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Return void </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
// Equivalent to l.dostring, but this will print error log automatically
l.eval<void>("a=1 b=2");
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Tell whether the evaluation fails </li></ul></ul> </td>
  <td> ✅ </td>
  <td>

```C++
bool failed;
int ret = l.eval<int>("return a + b",  /* disable log */ false, &failed);
if (failed) {
  // error handler
}
```
  </td>
</tr>

<tr>
  <td> <ul><ul><li> Support a defult value returned when evaluation fails </li></ul></ul> </td>
  <td> ✅ only supported by eval simple types (eval_xxx) </td>
  <td>

```C++
int ret = l.eval_int("return a + b", /* disable log */ false, /* default value */ -1);
```
  </td>
</tr>

</table>

## Value Conversions

**Notice**: Value conversions from Lua to C++ may be different with that in Lua!

Conversion process: Firstly convert value in Lua to C++'s value with
corresponding type, e.g. boolean to bool, integer to long long, number to
double, string to string, then cast it to target type by C++'s type
conversion strategy. Note that number in Lua is also string, and
number-literal-string is also number.

This lib mainly uses C++'s value conversion strategy, in addition, an implicitly
conversion strategy between number and number-literal-string, which is
supported by Lua.

In total, this lib makes value conversions behave more like C++.

Details:
1. Implicitly conversion between integer, number, boolean using
   C++'s static_cast
2. Implicitly conversion between number and number-literal-string by Lua
3. When convert number 0 to boolean, will get false (not true as Lua does)
4. NONE and NIL won't convert to any value, default value (user given or
   initial value of target type) will be returned
5. Non-number-literal-string, including empty string, can't convert to any
   other types, default value will be returned (can't convert to true as 
   Lua does)
6. Integer's precision won't be lost if its value is representable by 64bit
   signed integer type, i.e. between [-2^63, 2^63 -1], which is
   [-9223372036854775808, 9223372036854775807]

Examples:
* number 2.5 -> string "2.5" (By Lua)
* number 3 -> string "3.0" (By Lua)
* integer 3 -> string "3" (By Lua)
* string "2.5" -> double 2.5 (By Lua)
* number 2.5 -> int 2 (By C++)
* string "2.5" -> int 2 (Firstly "2.5"->2.5 by lua, then 2.5->2 by C++)
* boolean true -> int 1 (By C++)
* boolean false -> int 0 (By C++)
* integer 0 -> bool false (By C++)
* number 2.5 -> bool true (By C++)
* string "2.5" -> bool true ("2.5"->2.5 by Lua, then 2.5->true by C++)
* string "0" -> bool false ("0"->0 by Lua, then 0->false by C++)

## Introduction

### 1. Get Lua values in C++

#### 1.1 Get simple type values, default value supported!

API:

```C++
bool               get_bool   (@PATH_TYPE@ path, const bool&               def = false, bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
int                get_int    (@PATH_TYPE@ path, const int&                def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
unsigned int       get_uint   (@PATH_TYPE@ path, const unsigned int&       def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
long               get_long   (@PATH_TYPE@ path, const long&               def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
unsigned long      get_ulong  (@PATH_TYPE@ path, const unsigned long&      def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
long long          get_llong  (@PATH_TYPE@ path, const long long&          def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
unsigned long long get_ullong (@PATH_TYPE@ path, const unsigned long long& def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
float              get_float  (@PATH_TYPE@ path, const float&              def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
double             get_double (@PATH_TYPE@ path, const double&             def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
long double        get_ldouble(@PATH_TYPE@ path, const long double&        def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
std::string        get_string (@PATH_TYPE@ path, const std::string&        def = "",    bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
```

`@PATH_TYPE@` could be:

1. `const char*` or `const std::string&`.

Then the formal parameter `path` is a global variable's name in Lua.

2. `const std::initializer_list<const char*>&`, `const std::initializer_list<std::string>&`, `const std::vector<const char*>&` or`const std::vector<std::string>&`.

Then the formal parameter `path` is a path to subfield of a table.

* @param [in] path The target variable's name or path.
* @param [in] def The default value returned if failed or target does not exist.
* @param [in] disable_log Whether print a log when exception occurs.
* @param [out] failed Will be set whether the operation is failed if this
pointer is not nullptr.
* @param [out] exists Set whether the variable exists. Regard none and nil as 
not exists.
* @return Return the variable's value if the variable exists and conversion 
succeeded.


Example:

```C++
peacalm::luaw l;
l.dostring("a = 1 b = true c = 2.5 d = 'good'");
int a = l.get_int("a");            // 1
bool b = l.get_bool("b");          // true
bool b2 = l.get_bool("b2", false); // false
bool b3 = l.get_bool("b2", true);  // true
double c = l.get_double("c");      // 2.5
std::string d = l.get_string("d"); // "good"

bool dfailed, dexists;
long dlong = l.get_long("d", -1, false, &dfailed, &dexists);
// dlong == -1, dfailed == true, dexists == true

bool dfailed2, dexists2;
long dlong2 = l.get<long>("d", false, &dfailed2, &dexists2);
// dlong2 == 0, dfailed2 == true, dexists2 == true
```

```C++
peacalm::luaw l;
l.dostring("a = 1 p={x=10,y=20} m={p1={x=1,y=2},p2={x=3.2,y=4.5}}");
l.get_int({"a"});      // 1
l.get_int({"ax"}, -1); // -1 (return user given default value)
l.get_long({"p", "x"});      // 10
l.get_long({"p", "z"}, 30);  // 30 (return user given default value)
l.get_double({"m", "p2", "x"}); // 3.2
```

#### 1.2 Get complex type values, no default value support!

API:

```C++
/// Get values, functions, userdatas, pointers etc.
/// @param path Could be single string or a list of string.
template <typename T>
T get(@PATH_TYPE@ path, bool disable_log = false, 
      bool* failed = nullptr, bool* exists = nullptr);


/// Options used as first parameter for "lget".
struct lgetopt {
  bool  disable_log;
  bool *failed, *exists;
  lgetopt(bool d = false, bool* f = nullptr, bool* e = nullptr)
      : disable_log(d), failed(f), exists(e) {}
};

/// Long get. Args is the path to get value.
template <typename T, typename... Args>
T lget(const lgetopt& o, Args&&... args) 
```

Also, `@PATH_TYPE@` could be single string or string list.

`lget` not only support single string or string list as path, but support list 
of any type if only it can be used as a key of a table, 
such as string, integer, void*, and `luaw::metatable_tag`.

Using this API we can get:

1. Can get a C++ data type.

`T` could be any type composited by bool, integer types, float number types,
C string, std::string, std::pair, std::tuple, std::vector, std::set,
std::unordered_set, std::map, std::unordered_map, std::deque, std::list,
std::forward_list.

Also, `T` could be cv-qualified, but cannot be a reference.

Note that using C string "const char*" as key type of set or map is
forbidden, and anytime using "const char*" as a member of a container is not 
recommended, should better use std::string instead.

2. Can get a user defined class type.

3. Can get a callable function-like type which represents a Lua function.

`T` could be std::function or luaw::function.

4. Can get a pointer type.
`T` could be pointer of data type.

When getting a container type and the variable exists, the result will contain 
elements who are successfully converted, and discard who are not or who are nil.
Regard the operation as failed if any element's conversion failed.

* @tparam T The result type user expected. 
* @param [in] path The target variable's name or path.
* @param [in] disable_log Whether print a log when exception occurs.
* @param [out] failed Will be set whether the operation is failed if this
pointer is not nullptr. If T is a container type, it regards the operation
as failed if any element failed.
* @return Return the value with given name in type T if conversion succeeds,
otherwise, if T is a simple type (e.g. bool, int, double, std::string, etc), 
return initial value of T(i.e. by statement `T{}`), if T is a container
type, the result will contain all non-nil elements whose conversion
succeeded and discard elements who are nil or elements whose conversion
failed.


Example:

```C++
peacalm::luaw l;
l.dostring("a = 1 b = true v={1,2} v2={1,2.5} v3={1,nil,3} m={p1={1,2},p2={3,4}}");
auto a = l.get<int>("a");          // 1
auto as = l.get<std::string>("a"); // "1"
auto b = l.get<bool>("b");         // true
auto bs = l.get<std::string>("b"); // "" (this conversion fails)

auto v = l.get<std::vector<int>>("v");          // [1,2]
auto vs = l.get<std::vector<std::string>>("v"); // ["1","2"]
auto v2 = l.get<std::vector<int>>("v2");        // [1,2] (2.5->2)
auto v3 = l.get<std::vector<int>>("v3");        // [1,3] (ignore nil)

auto m = l.get<std::map<std::string, std::vector<int>>>("m"); // {"p1":[1,2],"p2":[3,4]}

auto p1 = l.get<std::pair<int, int>>({"m", "p1"}); // (1,2)

// example for lget:
l.dostring("g={gg={{a=1,b=2},{a=10,b=20,c='s'}}}");
int b = l.lget<int>({}, "g", "gg", 2, "b"); // 20
```

Another example, could use lget to get a userdata's metatable name:
```C++
struct Obj {};

int main() {
  peacalm::luaw l;
  l.register_member<void* const Obj::*>(
    "id", [](const volatile Obj* p) { return (void*)p; });

  auto s = std::make_shared<Obj>();
  l.set("s", s);

  void* sid = l.eval<void*>("return s.id");
  void* sid2 = l.get<void*>({"s", "id"});
  assert(sid == sid2);
  assert(sid == (void*)(s.get()));

  void* saddr = l.get<void*>("s"); // the shared ptr s itself's address
  assert(saddr != sid);

  // Use lget to get metatable name
  bool disable_log, failed, exists;
  auto metatbname = l.lget<std::string>({disable_log, &failed, &exists},
    "s", peacalm::luaw::metatable_tag{}, "__name");
  std::cout << "metatable name of s: " << metatbname << std::endl;
}
```

### 2. Set C++ values to Lua
API:

```C++
/// Set value as a global variable or a subfield of a table
template <typename T>
void set(@PATH_TYPE@ path, T&& value);

/// Set with a hint type
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value>
void set(@PATH_TYPE@ path, T&& value);


/// Long set. The last argument is value, the rest arguments are indexes and
/// sub-indexes, where could contain luaw::metatable_tag.
template <typename... Args>
void lset(Args&&... args);

/// Long set with a hint type.
template <typename Hint, typename... Args>
void lset(Args&&... args)


// Other very simple API to set global variables to Lua.
// @NAME_TYPE@ could be const char* or const std::string&.
void set_integer(@NAME_TYPE@ name, long long value);
void set_number(@NAME_TYPE@ name, double value);
void set_boolean(@NAME_TYPE@ name, bool value);
void set_nil(@NAME_TYPE@ name);
void set_string(@NAME_TYPE@ name, const char* value);
void set_string(@NAME_TYPE@ name, const std::string& value);
```

Also, `@PATH_TYPE@` could be a single string or a string list.
If it is a single string, this API sets a global variable to Lua;
If it is a string list, this API sets a subfield of a Lua table, and if keys 
in path doesn't exist or it is not a table 
(or not indexable and newindexable userdata), 
it will creat a new table for it.

So the set operation will always succeed.

`lset` not only support string list as path, but support any type if only 
it can be used as a key of a table, such as string, integer, void*, and 
`luaw::metatable_tag`.


The value type can be:
1. C++ data type: simple types or container types
2. C function or lambda or std::function
3. Custom class types
4. smart pointers
5. raw pointers
6. nullptr
7. luaw::newtable_tag

Especially to say:

`nullptr` means nil in Lua. e.g. `set(name, nullptr)` means setting nil to "name".

`set(name, luaw::newtable_tag{})` means setting a new empty table to "name".

Setting a raw pointer means setting a lightuserdata to Lua.

Example:

```C++
l.set("b", true);
l.set("i", 123);
l.set({"g", "f"}, 123.45);  // set g.f = 123.45, make g as a global table
l.set({"g", "s"}, "cstr");
l.set("s2", std::string("std::string"));

l.set("a", std::vector<int>{1,2,3});
l.set("b", std::map<std::string, int>{{"a",1},{"b",2}});
l.set({"g", "c"}, std::make_pair("c", true));

l.set("x", nullptr); // set nil to "x"
l.set("t", peacalm::luaw::newtable_tag); // set "t" as a new empty table

l.lset("g", "b", "v", 1); // set g.b.v = 1, and make g, b as table

// These are equivalent to: l.dostring("nums = { en = {'one', 'two', 'three'}}");
l.lset("nums", "en", 1, "one");
l.lset("nums", "en", 2, "two");
l.dostring("nums.en[3] = 'three'");
```


### 3. Get and call Lua functions in C++

In C++ we use std::tuple to represent multiple returns in Lua.

#### 3.1 Call a Lua function directly:

API:

```C++
/// Call Lua function specified by path using C++ parameters.
/// Should at least provide return type.
template <typename Return, typename... Args>
Return callf(@PATH_TYPE@ path, const Args&... args);
```

`@PATH_TYPE@` could be ether a single string or a list of string.

Example:

```C++
l.dostring("f1 = function(a, b) return a + b end");
l.dostring("f2 = function(a, b) return a + b, a - b end");
l.dostring("g = {f1=f1, f2=f2}");

assert(l.callf<int>("f1", 1, 1) == 2);
assert(l.callf<int>({"g", "f1"}, 1, 1) == 2);
assert(l.callf<std::tuple<int, int>>({"g", "f2"}, 1, 1) == std::tuple<int, int>(2, 0));
```

In this case we can call a Lua function with C++ arguments directly and conveniently, 
but can't know whether it works correctly. If you want to know, see the following.

#### 3.2 Get a callable object to represent the Lua function

We can get a callable object of type `std::function` or `luaw::function` 
to represent the Lua function, then we can call it to implement the calling to 
the Lua function. But using `luaw::function` is recommended.

Using `std::function` is simple, but it provide nothing information about whether the function works correctly:

```C++
peacalm::luaw l;
int retcode = l.dostring("f = function(a, b) local s = a + b return s end;");
if (retcode != LUA_OK) { /* error handlers*/ }

// Test 1
{
  auto f = l.get<std::function<int(int, int)>>("f");
  int s = f(1, 1);
  assert(s == 2);  // works correctly
}

// Test 2
{
  // Typo of function name, getting an inexistent function
  auto f = l.get<std::function<int(int, int)>>("fnx");
  int s = f(1, 1);
  // Log info: Lua: calling an inexistent function
  assert(s == 0);  // works wrong
}

// Test 3
// The function f returns nothing, maybe forget
retcode = l.dostring("f = function(a, b) local s = a + b end;");
if (retcode != LUA_OK) { /* error handlers*/ }
{
  auto f = l.get<std::function<int(int, int)>>("f");
  int s = f(1, 1);
  assert(s == 0);  // works wrong
  // It returns a wrong result quietly, this is dangerous!
}
```

So, using `std::function` is not recommended.

Instance of `luaw::function` can be called like `std::function` 
(actually the latter is a simple wrapper of the former), 
but it provide more information after it was called, 
and these information are very userful to let us make sure whether the function works correctly as we expect, 
and we can write right error handlers if there are exceptions, 
and it can help us to debug where exceptions happen. 


These informations are:

A total state about the whole process:

- whether call the Lua function and get result successfully

Detailed states for each step:

- whether the function failed while running in Lua
- whether the Lua function exists
- whether converting the Lua function's result to C++ failed
- whether the Lua function returns result (whether result exists)
- whether got enough results (Lua could return more, couldn't less)
- how many results the Lua function returned
- how many results we expect the Lua function should at least return

Part of `luaw::function`'s implementation looks like:
```C++
template <typename>
class luaw::function;

template <typename Return, typename... Args>
class luaw::function<Return(Args...)> {
public:

  /// Whether the whole process failed.
  /// Any step fails the whole process fails, including running the function in
  /// Lua and converting results to C++, and whether get enough results.
  bool failed() const;

  /// Whether the function failed while running in Lua (not including function
  /// doesn't exist)
  bool function_failed() const;

  /// Whether the function exists in Lua
  bool function_exists() const;

  /// Whether converting the Lua function's results to C++ failed
  bool result_failed() const;

  /// Whether the Lua function returns results (whether results exist)
  bool result_exists() const;

  /// Whether Lua function returns enough results.
  /// Could return more than expect, but couldn't less.
  bool result_enough() const;

  /// Result number the Lua function returned
  int real_result_size() const;

  /// Result number we expect the Lua function should return.
  /// Could return more, couldn't less.
  constexpr int expected_result_size() const;

  // ...
};
```

Example:

```C++
peacalm::luaw l;
int retcode = l.dostring("f = function(a, b) local s = a + b end;");
if (retcode != LUA_OK) { /* error handlers*/ }

auto f = l.get<peacalm::luaw::function<int(int, int)>>("f");
int s = f(1, 1);
if (f.failed()) {
  if (f.function_failed()) {
    // ...
  } else if (!f.function_exists()) {
    // ...
  } else if (f.result_failed()) {
    // ...
  } else if (!f.result_enough()) {
    std::cout << "Result number of f " << f.real_result_size()
              << " less than " << f.expected_result_size() << std::endl;
  } else {
    // May never happen
  }
}
```

### 4. Bind C++ functions to Lua (also lambda, std::function or callable objects)

Uses same API as method "set". 

It supports:
* C style functions (not variadic)
* C++ overloaded functions
* template functions
* lambda
* std::function
* Any callable classes: should provide concrete hint type or luaw::function_tag

It doesn't support:
* C style variadic functions (such as function "printf")
* Unspecialized template functions
* Unspecialized generic lambda
* Unspecialized callable objects
* Default arguments defined in C++: will be default-initialized if no real parameters given in Lua


#### 4.1 Bind C/C++ functions

* The functions could have arbitary number of arguments
* Could use either C function reference or C function pointer
* Use std::tuple to represent multiple return values.
* C++ template functions should be explicitly specialized, or provide function proto type as hint
* When binding C++ overloaded funtions: should provide function proto type or its pointer type as hint

Example:

```C++
int fadd(int x, int y) { return a + b; }
int fadd_many(int a, int b, int c, int d, int e, int f, int g 
              /* define as many as you want */) {
  return a + b + c + d + e + f + g;
}

int main {
  peacalm::luaw l;

  // These are equivalent:
  l.set("fadd", fadd);  // set function directly
  l.set("fadd", &fadd); // set function address
  auto pfadd = &fadd;
  l.set("fadd", pfadd); // set function pointer
  auto& ref = fadd;
  l.set("fadd", ref);   // set function reference

  assert(l.eval<int>("return fadd(1, 2)") == 3);

  l.set("fadd_many", fadd_many);
  // Arguments not given real parameter will be default-initialized to zero,
  // which do not affect the function's result.
  assert(l.eval<int>("return fadd_many(1, 2, 3, 4)") == 10);
}
```

Multiple returns: use std::tuple!

```C++
std::tuple<int, int> fdiv(int x, int y) { return std::make_tuple(x / y, x % y); }

int main() {
  peacalm::luaw l;
  l.set("fdiv", fdiv);
  int retcode = l.dostring("q, r = fdiv(7, 3)");
  assert(retcode == LUA_OK);
  assert(l.get_int("q") == 2);
  assert(l.get_int("r") == 1);
}
```

C++ template functions: should specialize or provide function proto type as hint.

```C++
template <typename T>
T tadd(T a, T b) { return a + b; }
int main() {
  peacalm::luaw l;

  // Explicitly specialize the function
  l.set("tadd", tadd<int>);

  // Or provide the function proto type as hint
  l.set<double(double, double)>("tadd", tadd);
}
```

C++ overloaded functions: should provide function proto type or its pointer type as hint.

```C++
// Overloaded functions f
int f(int i) { return i * 2; }
double f(double d) { return d / 2; }
double f(double a, double b) { return a * b; }

int main() {
  peacalm::luaw l;

  // Provide the function proto type as hint
  l.set<int(int)>("f1", f);
  l.set<double(double)>("f2", f);
  l.set<double(double, double)>("f3", f);

  // Or could use function pointer proto type as hint
  l.set<int(*)(int)>("f1", f);
  l.set<double(*)(double)>("f2", f);
  l.set<double(*)(double, double)>("f3", f);

  assert(l.eval_bool("return f1(2) == 4"));
  assert(l.eval_bool("return f2(5) == 2.5"));
  assert(l.eval_bool("return f3(1.25, 10) == 12.5"));
  return 0;
}
```
#### 4.2 Bind lambdas


* Captureless lambda: May not provide hint type, like C function.

```C++
l.set("add", [](int a, int b) { return a + b; });
```

* Lambda with captured variables: should provide concrete hint type or use luaw::function_tag as hint.

```C++
int x = 1;
auto f = [&](int a) { return a + x; };

// Provide a function type hint: int(int)
l.set<int(int)>("add", f);

// Alternative writing: could use luaw::function_tag as hint type
l.set<peacalm::luaw::function_tag>("add", f);
```

* Generic lambda: should provide concrete hint type.

```C++
// Generic lambda
auto f = [](auto a, auto b) { return a + b; }

// Should provide a function proto type as hint
l.set<int(int, int)>("add", f);

// The lambda object can be reused with other function proto types
l.set<double(double, double)>("add2", f);
```

#### 4.3 Bind std::function

std::function has already contained function proto type, so we could set it directly.

```C++
std::function<int(int, int)> f = [](auto a, auto b) { return a + b; };

// Set f by copy:
l.set("f", f);

// Or set f by move:
l.set("f", std::move(f));
```

#### 4.4 Bind callable classes

Actually, lambda or std::function, is just a callable class.
So we can write other callable classes and bind it to Lua to be used as a function.

```C++
struct Plus {
  int operator()(int a, int b) { return a + b; }
};
int main() {
  peacalm::luaw l;

  // function proto type as hint:
  l.set<int(int, int)>("plus", Plus{});

  // Or use function_tag:
  l.set<peacalm::luaw::function_tag>("plus", Plus{});
}
```


### 5. Bind C++ classes to Lua

Suppose here is a C++ class to bind:
```C++
// The C++ custom class to register
struct Obj {
  int       i  = 1;
  const int ci = 1;

  Obj() {}
  Obj(int v) : i(v) {}
  Obj(int v, int cv) : i(v), ci(cv) {}
  
  int abs() const { return std::abs(i); }

  int plus() { return ++i; }
  int plus(int d) { i += d; return i; }
};
```

#### 5.1 Register constructors

API:

```C++
/**
 * @brief Register a global function to Lua who can create object.
 * 
 * It creates a userdata in Lua, whose C++ type is specified by Return type
 * of Ctor. e.g. `register_ctor<Object(int)>("NewObject")`. Then can run 
 * `o = NewObject(1)` in Lua.
 * 
 * @tparam Ctor Should be a function type of "Return(Args...)". 
 * @param fname The global function name registered.
 */
template <typename Ctor>
void register_ctor(const char* fname);
template <typename Ctor>
void register_ctor(const std::string& fname);
```

Equivalent to setting a global function to Lua who can create an instance of the 
corresponding class using the class's constructor.

For example:
```C++
peacalm::luaw l;
l.register_ctor<Obj()>("NewObj");          // default constructor
l.register_ctor<Obj(int)>("NewObj1");      // constructor with 1 argument
l.register_ctor<Obj(int, int)>("NewObj2"); // constructor with 2 argument

// Then can use ctor as a global function in Lua
l.dostring("a = NewObj(); b = NewObj1(2); c = NewObj2(3, 4)");
```

We can register constructors for const instances, then the instance created in Lua
will be a const variable, and all members registered will be its conster member.

Example:
```C++
// the constructors will generate a const instance of Obj in Lua
using ConstObj = const Obj;
l.register_ctor<ConstObj()>("NewConstObj");          // default constructor
l.register_ctor<ConstObj(int)>("NewConstObj1");      // constructor with 1 argument
l.register_ctor<ConstObj(int, int)>("NewConstObj2"); // constructor with 2 argument

// Then can use ctor as a global function in Lua
l.dostring("a = NewConstObj(); b = NewConstObj1(1); c = NewConstObj2(1,2)");
```

#### 5.2 Register member variables

API:

```C++
// Register a real member, ether member variable or member function.
// For overloaded member function, you can explicitly pass in the template
// parameter `MemberPointer`. e.g.
// `register_member<Return(Class::*)(Args)>("mf", &Class::mf)`
template <typename MemberPointer>
std::enable_if_t<std::is_member_pointer<MemberPointer>::value>
register_member(const char* name, MemberPointer mp);
```

Const/volatile property is kept in Lua.
That is you can't modify a const member's value in Lua.

Example:
```C++
l.register_member("i", &Obj::i);
l.register_member("ci", &Obj::ci); // const member

// Assume ctor is registered like that shows in above examples
l.dostring("a = NewObj(); a.i = 2");      // OK
l.dostring("a = NewObj(); a.ci = 2");     // Error: Const member cannot be modified: ci
l.dostring("a = NewConstObj(); a.i = 2"); // Error: Const member cannot be modified: i
```

#### 5.3 Register member functions

Uses same API as registering member variables.

If a member function is overloaded, then should explicitly provide the member 
function's type as hint.

What's more, the const/volatile property of the member function is kept in Lua.
That is, you can't call it's non-const member function by a const object.

Example:

```C++
l.register_member("abs", &Obj::abs);

// Register overloaded member functions.
l.register_member<int (Obj::*)()>("plus", &Obj::plus);
l.register_member<int (Obj::*)(int)>("plusby", &Obj::plus);

l.dostring("a = NewObj(); a:plus()"); // OK
l.dostring("b = NewConstObj(); b:plus()"); // Error: Nonconst member function: plus
```

#### 5.4 Register dynamic member variables

API:

```C++
// Register dynamic members by provide dynamic member getter and setter.
// getter/setter could be C function or lambda object.
// getter proto type: Member(const Class*, Key)
// setter proto type: void(Class*, Key, Member)
// where Key could be `const std::string&` or 'const char*',
// Member could be number, string, bool, luaw::luavalueidx, luaw::luavalueref, etc.
template <typename Getter, typename Setter>
void register_dynamic_member(Getter&& getter, Setter&& setter);
template <typename Getter>
void register_dynamic_member_getter(Getter&& getter);
template <typename Setter>
void register_dynamic_member_setter(Setter&& setter);
```

Dynamic members are members whose name is dynamically defined in Lua script, 
just like keys of a table, we can't know its name and value in advance.
So we register two functions for a class, dynamic member getter and setter,
to support this.

For example, if we want to support dynamic members for class `Foo` 
with string type key, any type of value, then we can use `luaw::luavalueref` as 
value type.

```C++
// If types of dynamic members are unknown, use luaw::luavalueref
struct Foo {
  std::unordered_map<std::string, peacalm::luaw::luavalueref> m;
};
peacalm::luaw::luavalueref foo_dm_getter(const Foo* o, const std::string& k) {
  auto entry = o->m.find(k);
  if (entry != o->m.end()) { return entry->second; }
  return peacalm::luaw::luavalueref(); // default value is nil
}
void foo_dm_setter(Foo* o, const std::string& k, const peacalm::luaw::luavalueref& v) {
  o->m[k] = v;
}
int main() {
  peacalm::luaw l;
  l.register_dynamic_member(foo_dm_getter, foo_dm_setter);
  l.register_ctor<Foo()>("NewFoo");
  l.dostring("a = NewFoo(); a.name = 'foo'; a.x = 1; a.y = 2; print(a.name, a.x, a.y, a.z)");
  // ...
}
```

If we expect the dynamic members' type is float number, then:

```C++
// If types of dynamic members are unknown, use luaw::luavalueref
struct Foo {
  std::unordered_map<std::string, double> m;
};
double foo_dm_getter(const Foo* o, const std::string& k) {
  auto entry = o->m.find(k);
  if (entry != o->m.end()) { return entry->second; }
  return 0; // suppose member's default value is 0
}
void foo_dm_setter(Foo* o, const std::string& k, double v) {
  o->m[k] = v;
}
int main() {
  peacalm::luaw l;
  l.register_dynamic_member(foo_dm_getter, foo_dm_setter);
  l.register_ctor<Foo()>("NewFoo");
  l.dostring("a = NewFoo(); a.x = 1; a.y = 2;");
  // ...
}
```

#### 5.5 Register fake member variables

API:

```C++
/**
 * @brief Register a fake member variable or fake member function.
 *
 * For example, register a const member "id" with type void* for class Obj:
 *     `register_member<void* const Obj::*>("id", [](const volatile Obj* p) {
 *         return (void*)p; });`
 *
 * Register a member function "plus" for class Obj:
 *     `register_member<void (Obj::*)(int)>("plus", [](Obj* p, int d) {
 *         p->value += d; })`
 *
 * Register a const member function "getvalue" for class Obj:
 *     `register_member<int (Obj::*)() const>("getvalue", [](const Obj* p) {
 *         return p->value; })`
 *
 * @tparam Hint The member type wanted to fake.
 * @tparam F C function type or lambda or std::function or any callable type.
 * @param name The member name.
 * @param f The function whose first parameter is pointer to the class whose
 * member is registered to. If faking a member variable, the first parameter
 * must be pointer of cv-qualified class type, e.g. `const volatile Obj*`.
 * @return void.
 */
template <typename Hint, typename F>
std::enable_if_t<std::is_member_pointer<Hint>::value &&
                  !std::is_same<Hint, F>::value>
register_member(const char* name, F&& f);
```

Except real members of a class, we can also register fake members by some special
function, and with a hint type which indicates the member's type that we want to fake.

When faking a member variable, the function's first and only parameter should be
a pointer to both const and volatile qualified class, e.g. `const volatile Obj* o`.

For example, fake a const member "id" for class Obj:

```C++
// Fake a const member "id" whose value is the object's address
l.register_member<void* const Obj::*>(
    "id", [](const volatile Obj* o) { return (void*)o; });

Obj o;
l.set("o", &o); // lightuserdata
assert(l.eval<void*>("return o.id") == (void*)(&o));

// For smart pointer of Obj, id also returns the underlying's address
auto so = std::make_shared<Obj>();
l.set("so", so);
assert(l.eval<void*>("return so.id") == (void*)(so.get()));
l.set("so2", so);
assert(l.eval_bool("return so.id == so2.id"));
assert(l.eval_bool("return so ~= so2"));
```

Or we can fake const members generated by operation on other members:
```C++
// Fake a const member 'sum' which is the sum of member i and member ci
l.register_member<const int Obj::*>(
  "sum", [](const volatile Obj* o) { return o->i + o->ci; });

// Fake a const member 'q' which is the quotient of member i and member ci
l.register_member<const double Obj::*>(
  "q", [](const volatile Obj* o) { return double(o->i) / double(o->ci); });

Obj o(3, 2);
l.set("o", o);
assert(l.eval<int>("return o.sum") == 5);
assert(l.eval<double>("return o.q") == 1.5);
```

If want to fake a mutable member, then the special function must return lvalue 
reference to a real variable in C++. For example:

```C++
int  gi = 100;  // global variable i
int& getgi(const volatile Obj* o) { return gi; }

int main() {
    peacalm::luaw l;

    l.set("o", Obj{});
    l.register_member<int Obj::*>("gi", &getgi);

    EXPECT_EQ(l.eval<int>("return o.gi"), gi);
    EXPECT_EQ(l.eval<int>("o.gi = 101; return o.gi"), 101);
    EXPECT_EQ(gi, 101);
}
```

Or we can fake member variables using lambda:

```C++
  peacalm::luaw l;

  l.set("o", Obj{});
  int  li    = 100;  // local variable i
  auto getli = [&](const volatile Obj*) -> int& { return li; };
  l.register_member<int Obj::*>("li", getli);

  EXPECT_EQ(l.eval<int>("return o.li"), li);
  EXPECT_EQ(l.eval<int>("o.li = 101; return o.li"), 101);
  EXPECT_EQ(li, 101);
```

Or we can fake member variables by dereference of pointer members of the object:
```C++
class Foo {
public:
  ~Foo() { delete pi; }
  int* pi = new int(1);
};

int main() {
  peacalm::luaw l;

  // Fake a member 'i' for Foo in Lua by dereference the pointer member 'pi'
  // The member 'i' is mutable in Lua
  l.register_member<int Foo::*>(
      "i", [](const volatile Foo* o) -> int& { return *o->pi; });
  
  auto o = std::make_shared<Foo>();
  l.set("o", o);

  assert(l.eval<int>("return o.i") == 1);
  assert(l.eval<int>("return o.i") == *o->pi);

  *o->pi = 2;
  assert(l.eval<int>("return o.i") == 2);

  assert(l.eval<int>("o.i = 3; return o.i") == 3);
  assert(*o->pi == 3);

  // const instance of Foo can also access 'i', but cannot modify it
  auto c = std::make_shared<const Foo>();
  l.set("c", c);
  assert(l.eval<int>("return c.i") == 1);
  assert(l.dostring("c.i = 2") != LUA_OK);
  l.log_error_out(); // Const member cannot be modified: i
  return 0;
}
```

#### 5.6 Register fake member functions

Uses same API as registering fake member variables.

Use a function whose first parameter is a pointer to the class, and provide a
hint type to indicate what type of member want to fake. 
Example:

```C++
// functions to fake as members of Obj
void seti(Obj* p, int v) { p->i = v; }
int  geti(const Obj* p) { return p->i; }

int main() {
  peacalm::luaw l;
  l.register_member<void (Obj ::*)(int)>("seti", &seti);
  l.register_member<int (Obj::*)() const>("geti", geti);

  auto o = std::make_shared<Obj>();
  l.set("o", o);
  l.dostring("print(o:geti())"); // prints 1
  l.dostring("o:seti(5)");
  assert(o->i == 5);
}
```


#### 5.7 Set class instance to Lua

Use "set" method, we can set a class instance which is defined in C++ to Lua,
just like set other type values.

Here are multiple ways to set a class instance to Lua, 
all the object setted to Lua by these ways 
can access members that have registered.

* Set instance by copy
* Set instance by move
* Set raw pointer to instance
* Set smart pointer to instance by copy
* Set smart pointer to instance by move
* Set raw pointer to smart pointer to instance

For example:

```C++
Obj a, b;
l.set("o1", a);            // by copy
l.set("o2", std::move(a)); // by move
l.set("o3", &b);           // by pointer

auto s = std::make_shared<Obj>();
l.set("o4", s);            // by copy of shared_ptr
l.set("o5", std::move(s)); // by move of shared_ptr
auto u = std::make_unique<Obj>();
l.set("o6", std::move(u)); // by move of unique_ptr


auto s2 = std::make_shared<Obj>();
auto u2 = std::make_unique<Obj>();

l.set("o7", &s2);           // by pointer to shared_ptr
l.set("o8", &u2);           // by pointer to unique_ptr

```

Then all variables "o1" ~ "o8" can access members of Obj that have registered.

What's more, `std::unique_ptr` with custom deleter behaves the same as that with 
default deleter.


#### 5.8 Const property

The const property of class instance or class member behaves same in Lua as that in C++.

* Const member variable cannot be modified.
* Cannot modify any member by const class instance or pointer to const class instance.
* Cannot call nonconst member functions by const class instance or pointer to const class instance.
* If the class instance is a smart pointer, only low level const property is concerned. (high level const, which is const about the smart pointer it self, has no effect)

Example:
```C++
l.register_member("i", &Obj::i);
l.register_member<int (Obj::*)()>("plus", &Obj::plus);


const Obj o;
l.set("o1", o); // set a const instance by copy
l.dostring("o1.i = 2"); // error

const auto s = std::make_shared<Obj>(); // high level const
l.set("o2", s);
l.dostring("o2.i = 2"); // OK

auto cs = std::make_shared<const Obj>(); // low level const
l.set("o3", cs);
l.dostring("o3.i = 2"); // error
```

### 6. Evaluate a Lua expression and get the results

#### 6.1 Evaluate to get a simple type result. Default result supported.

In the following API, `@EXPR_TYPE@` could be `const char*` or `const std::string&`.

```C++
bool               eval_bool   (@EXPR_TYPE@ expr, const bool&               def = false, bool disable_log = false, bool* failed = nullptr);
int                eval_int    (@EXPR_TYPE@ expr, const int&                def = 0,     bool disable_log = false, bool* failed = nullptr);
unsigned int       eval_uint   (@EXPR_TYPE@ expr, const unsigned int&       def = 0,     bool disable_log = false, bool* failed = nullptr);
long               eval_long   (@EXPR_TYPE@ expr, const long&               def = 0,     bool disable_log = false, bool* failed = nullptr);
unsigned long      eval_ulong  (@EXPR_TYPE@ expr, const unsigned long&      def = 0,     bool disable_log = false, bool* failed = nullptr);
long long          eval_llong  (@EXPR_TYPE@ expr, const long long&          def = 0,     bool disable_log = false, bool* failed = nullptr);
unsigned long long eval_ullong (@EXPR_TYPE@ expr, const unsigned long long& def = 0,     bool disable_log = false, bool* failed = nullptr);
float              eval_float  (@EXPR_TYPE@ expr, const float&              def = 0,     bool disable_log = false, bool* failed = nullptr);
double             eval_double (@EXPR_TYPE@ expr, const double&             def = 0,     bool disable_log = false, bool* failed = nullptr);
long double        eval_ldouble(@EXPR_TYPE@ expr, const long double&        def = 0,     bool disable_log = false, bool* failed = nullptr);
std::string        eval_string (@EXPR_TYPE@ expr, const std::string&        def = "",    bool disable_log = false, bool* failed = nullptr);
```

The expresion must have one return value, if more than one returned,
only the first one is used. 

If the evaluation fails, the default value `def` is returned.

#### 6.2 Evaluate to get complex type results. No default result supported.
API: 

```C++
template <typename T>
T eval(@EXPR_TYPE@ expr, bool disable_log = false, bool* failed = nullptr);
```
Result type T is like to that of method "get", but more.
T can be void, if T is void or std::tuple<>, it represents the expression 
do not have to provide return value.
If T is std::tuple, the expression can provide multiple return values.

Example:

```C++
peacalm::luaw l;
l.set("a", 10);
l.set("b", 5);
l.set("c", 2);

double ret = l.eval_double("return a^2 + b/c"); // 102.5
std::string s = l.eval_string("if a > b + c then return 'good' else return 'bad' end"); // "good"
auto si = l.eval<std::set<int>>("return {a, b, c}"); // {2,5,10}

auto t = l.eval<std::tuple<int, int, int>>("return a, b, c");  // multiple returns

l.eval<void>("print(a, b, c)"); // no returns
```

#### 6.3 Evaluate with custom variable provider (custom_luaw)

```C++
template <typename VariableProviderPointer>
class custom_luaw;
```

The class `custom_luaw` is derived from `luaw`, it can contain
a user defined variable provider.
When a global variable used in some expression does not 
exist in Lua, then it will seek the variable from the provider.

The template parameter `VariableProviderPointer` could 
be either a raw pointer type or a smart pointer type, i.e. std::shared_ptr or 
std::unique_ptr.
The underlying provider type should implement a member function:
* `bool provide(peacalm::luaw& l, const char* vname);`

In this member function, it should push exactly one value whose name is vname 
onto the stack then return true. Otherwise return false if vname is 
illegal or vname doesn't have a correct value.

Example:

```C++
struct provider {
  bool provide(peacalm::luaw& l, const char *vname) {
    if (strcmp(vname, "a") == 0) {
      l.push(1);
    } else if (strcmp(vname, "b") == 0) {
      l.push(2);
    } else if (strcmp(vname, "c") == 0) {
      l.push(3);
    } else {
      return false;
    }
    // If variables won't change, could set them to global,
    // which may improve performance:
    // l.copy_to_global(vname);
    return true;
  }
};

using provider_type = std::unique_ptr<provider>;
int main() {
  peacalm::custom_luaw<provider_type> l;
  l.provider(std::make_unique<provider>()); // Install provider
  double ret = l.eval_double("return a*10 + b^c");
  std::cout << ret << std::endl;  // 18
}
```


### 7. Low level operatioins: seek/to/touchtb/setkv/push

#### 7.1 The seek functions

**Notice**: Caller is responsible for popping the stack after calling the seek 
functions.

The seek functions push the global value or field of a table onto stack:
```C++
// Push the global environment onto the stack.
// Equivalent to gseek("_G") if "_G" is not modified.
luaw& gseek_env();

// Global Seek: Get a global value by name and push it onto the stack, or 
// push a nil if the name does not exist.
luaw& gseek(const char* name);
luaw& gseek(const std::string& name);

// Push t[name] onto the stack where t is the value at the given index `idx`,
// or push a nil if the operation fails.
luaw& seek(const char* name, int idx = -1);
luaw& seek(const std::string& name, int idx = -1);

// Push t[n] onto the stack where t is the value at the given index `idx`, or
// push a nil if the operation fails.
// Note that index of list in Lua starts from 1.
luaw& seek(int n, int idx = -1);

// Push t[p] onto the stack where t is the value at the given index `idx`,
// or push a nil if the operation fails.
self_t& seek(void* p, int idx = -1);

// Push the metatable of the value at the given index onto the stack if it
// has a metatable, otherwise push a nil.
self_t& seek(metatable_tag, int idx = -1);

// Long Seek: Call gseek() for the first parameter, then call seek() for the 
// rest parameters.
template <typename T, typename... Ts>
luaw& lseek(const T& t, const Ts&... ts);
```

#### 7.2 The type conversion functions
Type conversion functions convert a value in Lua stack to C++ type.

##### 7.2.1 To simple type
* @param [in] idx Index of Lua stack where the value in.
* @param [in] def The default value returned if conversion fails.
* @param [in] disable_log Whether print a log when exception occurs.
* @param [out] failed Will be set whether the convertion is failed if this 
pointer is not nullptr.
* @param [out] exists Will be set whether the value at given index exists if 
this pointer is not nullptr. Regard none and nil as not exists.
```C++
// To simple type
bool               to_bool   (int idx = -1, const bool&               def = false, bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
int                to_int    (int idx = -1, const int&                def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
unsigned int       to_uint   (int idx = -1, const unsigned int&       def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
long               to_long   (int idx = -1, const long&               def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
unsigned long      to_ulong  (int idx = -1, const unsigned long&      def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
long long          to_llong  (int idx = -1, const long long&          def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
unsigned long long to_ullong (int idx = -1, const unsigned long long& def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
float              to_float  (int idx = -1, const float&              def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
double             to_double (int idx = -1, const double&             def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
long double        to_ldouble(int idx = -1, const long double&        def = 0,     bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
std::string        to_string (int idx = -1, const std::string&        def = "",    bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
```

##### 7.2.2 To complex type
Conversion to complex C++ type.
Note that there are no default value parameters in this function:
```C++
// To complex type, without default value parameter
template <typename T> T to(int idx = -1, bool disable_log = false, bool* failed = nullptr, bool* exists = nullptr);
```

Example:
```C++
peacalm::luaw l;
l.dostring("g={a=1, gg={a=11,ggg={a='s'}}, list={1,2,3}, m={{a=1},{a=2}}}");

int a = l.gseek("g").seek("a").to_int(); // g.a : 1
std::cout << l.gettop() << std::endl;    // 2

// g.a on top
l.to<int>(); // 1
l.to<std::string>(); // "1"

l.pop(); // Now g on top of stack
l.seek("gg").seek("a").to<int>(); // g.gg.a : 11

l.settop(0); // Clear stack

// Note that list index starts from 1 in Lua
l.gseek("g").seek("list").seek(3).to_int(); // g.list[3] : 3
// Start with gseek, ignore existing values in stack
l.gseek("g").seek("gg").seek("ggg").seek("a").to_string(); // g.gg.ggg.a : s
std::cout << l.gettop() << std::endl; // 7 (3 for first line, 4 for second)

l.pop(); // Now ggg on top of stack
l.to<std::unordered_map<std::string, std::string>>(); // g.gg.ggg : {"a":"s"}
l.settop(0);

// The followings are equivalent ways of writing:
l.gseek("g").seek("m").seek(2).seek("a").to_int();            // g.m[2].a : 2
l.gseek_env().seek("g").seek("m").seek(2).seek("a").to_int(); // g.m[2].a : 2
l.gseek("_G").seek("g").seek("m").seek(2).seek("a").to_int(); // g.m[2].a : 2
l.lseek("g", "m", 2, "a").to_int();                           // g.m[2].a : 2
l.lseek("_G", "g", "m", 2, "a").to_int();                     // g.m[2].a : 2

// Don't forget to clear the stack at last.
l.settop(0);
```


#### 7.3 touchtb: touch table

Ensure a subfield of a table is a table.
If the subfield doesn't exist or is not a table, make a new table then overwrite it.

API:

```C++
/// Push the table (or value indexable and newindexable) with given name onto
/// stack. If not exists, create one.
self_t& gtouchtb(const char* name);

/// Push the table (or value indexable and newindexable) t[name] onto stack,
/// where t is a table at given index. If t[name] is not a table, create a new
/// one.
self_t& touchtb(const char* name, int idx = -1);


/// Push the table (or value indexable and newindexable) t[n] onto stack,
/// where t is a table at given index. If t[n] is not a table, create a new
/// one.
self_t& touchtb(int n, int idx = -1);

/// Push the table (or value indexable and newindexable) t[p] onto stack,
/// where t is a table at given index. If t[p] is not a table, create a new
/// one.
self_t& touchtb(void* p, int idx = -1);

/// Push the metatable of the value at the given index onto the stack.
/// If the value does not have a metatable, create a new metatable for it then
/// push the metatable onto stack.
/// The way to create new metatable: If m.tname is empty, create an empty
/// metatable, else create a new metatable using `luaL_newmetatable(L_,
/// m.tname)`.
self_t& touchtb(metatable_tag m, int idx = -1);

/// Long touchtb: Call gtouchtb() for the first parameter, then call touchtb()
/// for the rest parameters.
template <typename T, typename... Ts>
self_t& ltouchtb(const T& t, const Ts&... ts);
```

#### 7.4 setkv

API:

```C++
/// Set t[key] = value, where t is a table at given index.
template <typename T>
void setkv(const char* key, T&& value, int idx = -1);
template <typename T>
void setkv(const std::string& key, T&& value, int idx = -1);
template <typename T>
void setkv(int key, T&& value, int idx = -1);
template <typename T>
void setkv(void* key, T&& value, int idx = -1);


/// Set field with an user given hint type.
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value> setkv(const char* key,
                                                      T&&         value,
                                                      int         idx = -1);
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value> setkv(const std::string& key,
                                                      T&& value,
                                                      int idx = -1);
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value> setkv(int key,
                                                      T&& value,
                                                      int idx = -1);
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value> setkv(void* key,
                                                      T&&   value,
                                                      int   idx = -1);
                                                    

/// Set the parameter value as metatable for the value at given index.
/// Setting nullptr as metatable means setting nil to metatable.
template <typename T>
void setkv(metatable_tag, T&& value, int idx = -1);
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value> setkv(metatable_tag,
                                                      T&& value,
                                                      int idx = -1);
```

#### 7.5 push

API:

```C++
/// Push a value onto stack.
template <typename T>
int push(T&& value);

/// Push with an user given hint type.
template <typename Hint, typename T>
std::enable_if_t<!std::is_same<Hint, T>::value, int> push(T&& value);
```


### 8. Execute Lua Scripts (File or String)

Just a simple wrapper of raw Lua API, nothing more added:

```C++
int loadstring(const char*        s);
int loadstring(const std::string& s);
int dostring(const char*        s);
int dostring(const std::string& s);
int loadfile(const char*        fname);
int loadfile(const std::string& fname);
int dofile(const char*        fname);
int dofile(const std::string& fname);
```

`dostring` is equivalent to `eval<void>` or `eval<std::tuple<>>`, 
the only difference is that the former will keep error info in stack, 
and the latter will print error info to stderr.

Example:

```C++
int main() {
  peacalm::luaw l;
  if (l.dofile("conf.lua") != LUA_OK) {
    l.log_error_in_stack();
    return 1;
  }
  // ...

  std::string expr = "a = 1 b = 2";
  if (l.dostring(expr) != LUA_OK) {
    l.log_error_in_stack();
    return 1;
  }
  // ...

  return 0;
}
```

### 9. Initialization options

On default, the `luaw` instance will load all standard Lua libs, but it is costly,
and not all libs are always needed.
So here is a initialization option class to guide how to initialize a luaw instance.

```C++
/// Initialization options for luaw.
class opt {
public:
  opt() {}

  /// Ignore all standard libs.
  opt& ignore_libs();
  /// Load all standard libs.
  opt& load_libs();
  /// Preload all standard libs.
  opt& preload_libs();

  /// Register extended functions.
  opt& register_exfunctions(bool r);

  /// Use already existed lua_State.
  opt& use_state(lua_State* L);

  /// Load user specified libs.
  opt& custom_load(const std::vector<luaL_Reg>& l);
  opt& custom_load(std::vector<luaL_Reg>&& l);

  /// Preload user specified libs.
  opt& custom_preload(const std::vector<luaL_Reg>& l);
  opt& custom_preload(std::vector<luaL_Reg>&& l);
}
```

What's more, this lib provides some useful extended functions.
The option `opt::register_exfunctions` indicates whether register these:

```C++
/// Short writing for if-elseif-else statement.
/// The number of arguments should be odd and at least 3.
/// Usage: IF(expr1, result_if_expr1_is_true,
///           expr2, result_if_expr2_is_true,
///           ...,
///           result_if_all_exprs_are_false)
/// Example: return IF(a > b, 'good', 'bad')
inline int IF(lua_State* L);

/// Convert multiple arguments or a list to a set, where key's mapped value is
/// boolean true.
inline int SET(lua_State* L);

/// Convert multiple arguments or a list to a dict, where key's mapped value is
/// the key's appearance count. Return nil if key not exists.
inline int COUNTER(lua_State* L);

/// Like COUNTER but return 0 if key not exists.
inline int COUNTER0(lua_State* L);
```


Example:

```C++
using namespace peacalm;

// Default initialization: load all standard libs, register extended functions
luaw l1;

// Preload libs, register extended functions
luaw l2(luaw::opt{}.preload_libs());

// Do not load libs, do not register extended functions
luaw l3(luaw::opt{}.ignore_libs().register_exfunctions(false));

// Load and preload some specific libs
luaw l4(luaw::opt()
            .ignore_libs()
            .custom_load({{LUA_GNAME, luaopen_base},
                          {LUA_LOADLIBNAME, luaopen_package},
                          {LUA_OSLIBNAME, luaopen_os}})
            .custom_preload({{LUA_MATHLIBNAME, luaopen_math},
                            {LUA_STRLIBNAME, luaopen_string}}));
```


## Build and Install
Install Lua first (here use lua-5.4.4 just as an example, of course can use later versions):
```bash
curl -R -O http://www.lua.org/ftp/lua-5.4.4.tar.gz
tar zxf lua-5.4.4.tar.gz
cd lua-5.4.4
make all test
sudo make install
```

Build then install:
```bash
git clone https://github.com/peacalm/cpp-luaw.git
cd cpp-luaw
mkdir build
cd build
cmake .. 
sudo make install
```

Test is developed using GoogleTest, if GoogleTest is installed, then can run 
test like:
```bash
cd cpp-luaw/build
cmake .. -DBUILD_TEST=TRUE
make
ctest
```
