# Step 5: Dispatcher Challenge

## Prelims

* Familiarize myself with https://github.com/Tencent/rapidjson 
* Parse through [dispatcher_challenge.cpp prompt](../prompt/dispatcher_challenge.cpp) to determine if will be using DOM API or SAX API, among other things.
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
``` 



## Design Constraints

* main()
  * adds command handlers in Controller class to CommandDispatcher using addCommandHandler
  * invokes CommandDispatcher::dispatchCommand using a raw string.
* CommandDispatcher()
  * stores command handlers in Controller class using a private map
  * invokes command handler functions using either rapidjson::value or rapidjson::document objects
* Controller()
  * implements command handler functions
  
## Design Decisions

* main()
  * prints success to user
  * catches failure messages exceptions and prints to user
* CommandDispatcher()
  * parses raw string from main into a rapidjson::document
  * performs initial checks to verify that "command" and "payload" values exist
  * extracts "payload" value from document and creates new document to pass to command handler
  * attempts to invoke command handler function matching "command" 
  * prints success to user
  * passes failures as exceptions back to main
  * passes exceptions from Controller back to main
* Controller()
  * performs commanded functions
  * prints success to user
  * passes failures as exceptions back to CommandDispatcher
  



    
<br>

Current Progress
---

Info on what I am currently working on is available in [Parsing Code (Appendix)](Step_5_Appendix.md#parsing-code), [Brainstorming (Appendix)](Step_5_Appendix.md#brainstorming), and [Research (Appendix)](Step_5_Appendix.md#research)

Also, most functions have lengthy comments about what I am working through in [the code itself](../src/Step_5/main.cpp).

    
