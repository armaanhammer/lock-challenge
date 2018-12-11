# Step 5 Appendix

## CMAKE

### Parsing

I am relatively new to CMAKE, so need to parse the [included CMakeLists.txt](../prompt/CMakeLists.txt):

```
cmake_minimum_required(VERSION 3.9)
project(dispatcher)

set(CMAKE_CXX_STANDARD 11)

add_executable(dispatcher main.cpp)
```

It looks straightforward enough. Going to copy [dispatcher_challenge.cpp prompt](../prompt/dispatcher_challenge.cpp) to src and rename it [main.cpp](../src/Step_5/main.cpp) to comply.

## Parsing Code

What is meant by "dispatching"? I assume data is moving from somewhere to somewhere else. I am having trouble understanding data movement in the code provided though.

### What is this actually doing?

* The auto test commands at the top appear to map a single JSON object to a C++ variable
* The typecasted typedef line may be revealing:
  `typedef std::function<bool(rapidjson::Value &)> CommandHandler;`
  
#### Typedef
  Syntax: *typedef data_type new_name;*
  
  *data_type*: An existing type or user defined type created using structure/union.

  *new_name*: alias to the existing type or user defined type.

  so: 
  
  `std::function<bool(rapidjson::Value &)>`    <-- existing type
  
  `CommandHandler`    <-- alias to existing type
  
#### Typecast


#### Trying to parse this too:

    // add command handlers in Controller class to CommandDispatcher using addCommandHandler
  
  
  
  
  
  
  


### class Controller

#### bool help()

##### `rapidjson::Value &payload`

  per http://rapidjson.org/classrapidjson_1_1_generic_value.html
  
---

## example test commands

```C++
auto help_command = R"(
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }",
  }
 }
)";
```

https://en.cppreference.com/w/cpp/language/string_literal says:

> prefix(optional) **R** "*delimiter*( raw_characters )*delimiter*"
>
> **raw_characters**	-	Any character sequence, except that it must not contain the closing sequence )*delimiter*"

So, the raw JSON is:

```json
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }",
  }
 }
```




  
  
  
  
  
  
