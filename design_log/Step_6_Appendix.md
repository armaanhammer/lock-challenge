# Step 6 Appendix

## CMAKE

### Parsing

Parsing the [included CMakeLists_1.txt](../prompt/CMakeLists_1.txt):

```
cmake_minimum_required(VERSION 3.8)

project (test-memory-pool
        VERSION 1.0.0
        DESCRIPTION "Test Memory Pool")

include_directories(.)
link_directories(../ ./)

set(CMAKE_CXX_FLAGS "-Wall -std=c99")

# create executable
add_executable (test test-memory-pool.c memory_pool.c)
set_property(TARGET test PROPERTY C_STANDARD 99)
```

Ok, so this is going to be straight C, rather than C++. Going to copy [memory_pool.c prompt](../prompt/memory_pool.c) and [memory_pool.h prompt](../prompt/memory_pool.h) src.

Also going to add debug symbols. Trying adding this line: `set(CMAKE_BUILD_TYPE Debug)`, and will also add flag `-g` to this line: `set(CMAKE_CXX_FLAGS "-Wall -std=c99")` to pass in explicitly. 


<br>


Analysis
---



  
