# Step 5: Dispatcher Challenge

## Prelims

* Familiarize myself with https://github.com/Tencent/rapidjson 
* Parse through [initial dispatcher_challenge.cpp prompt](../prompt/dispatcher_challenge.cpp) to determine if will be using DOM API or SAX API, among other things.
  * file has `#include "rapidjson/document.h"` which \[seems to] imply that DOM API will be used.

## Resources

https://jsonlint.com/ JSON validator
https://cmake.org/ build tool

## Parsing

psuedo-code for main:

```
start Command Dispatch
  create a CommandDispatcher class object
    member functions:
    public:
      * help
      * exit
      * Additional command 1
      * Additional command 2
      * Additional command 3
      * Additional command 4
  create a Controller class object
    member functions:
    public:
      * constructor
      * destructor
      * addCommandHandler
      * dispatchCommand
    private: 
    
