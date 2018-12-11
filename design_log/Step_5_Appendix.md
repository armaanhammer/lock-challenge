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

What is meant by "dispatching"? I assume data is moving from somewhere to somewhere else. I am having trouble understanding data movememtn in the code provided though.

### class Controller

#### bool help()

##### `rapidjson::Value &payload`

  per http://rapidjson.org/classrapidjson_1_1_generic_value.html
  
  
  
  
