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

It looks straightforward enough. Going to copy [dispatcher_challenge.cpp prompt](../prompt/dispatcher_challenge.cpp) to src and rename it main.cpp to comply.

