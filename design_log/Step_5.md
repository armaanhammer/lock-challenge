# Step 5: Dispatcher Challenge

To duplicate
---

1. Rapidjson headers need to be placed in the [/src/Step_5/rapidjson/ directory](../src/Step_5/rapidjson) for the program to build. For convenience, the headers from the [66eb606 commit](https://github.com/Tencent/rapidjson/tree/66eb6067b10fd02e419f88816a8833a64eb33551/include/rapidjson) have been included already. If desired, it may be possible to update the headers to the current commit by replacing the entire rapidjson directory in this project the current [rapidjson/include/rapidjson directory](https://github.com/Tencent/rapidjson/tree/master/include/rapidjson).
1. In the [/src/Step_5/build/ directory](../src/Step_5/build) run `cmake ../`. The build environment will be set up in that directory using the [CMakeLists.txt file](../src/Step_5/CMakeLists.txt) in the parent directory.
1. Enure that the bool value for `TEST_ALL` [on line 37 of main.cpp](../src/Step_5/main.cpp#L37) is set to the desired value:
   * A value of `true` will cause all test commands to be sent to the dispatcher upon program startup.
   * A value of `false` will skip all tests and immediately fall through to a user prompt.
1. In the [/src/Step_5/build/ directory](../src/Step_5/build/) run `make`. Executable will build as `dispatcher`. 
1. To run executable, run `./dispatcher`.
1. If the final command (exit_command) [on line 804 of main.cpp](../src/Step_5/main.cpp#L804) is not commented out, the exit_command will succeed, terminating execution. If it is commented out, execution will fall through to a user prompt. 
1. At the user prompt, JSON may be entered for parsing.


Files
---

* [main.cpp](../src/Step_5/main.cpp) (with DEBUG code)
* [main_no_debug.cpp](../src/Step_5/main_no_debug.cpp) (identical, except without DEBUG code)
* [CMakeLists.txt file](../src/Step_5/CMakeLists.txt) (slightly modified from [prompt file](../prompt/CMakeLists.txt))


Documentation
---

Doxygen documentation is available for this step of the project. 

| [Step 5 main page](https://armaanhammer.github.io/lock-challenge/Step_5/html/index.html) | | |
| --- | --- | --- |
| main.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_5/html/main_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_5/html/main_8cpp_source.html) |
| main_no_debug.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_5/html/main__no__debug_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_5/html/main__no__debug_8cpp_source.html) |


Results
---

### Example 1

Running the program with:
* `TEST_ALL` enabled
* `VERBOSE` disabled
* `exit_command` not commented out 
produced this output to the terminal:

```bash
armaan@ubuntuVM:build$ ./dispatcher 
COMMAND DISPATCHER: STARTED
CommandDispatcher: addCommandHandler: help
CommandDispatcher: addCommandHandler: exit
CommandDispatcher: addCommandHandler: sum_ints
CommandDispatcher: addCommandHandler: query_payload
CommandDispatcher: addCommandHandler: mean_ints
CommandDispatcher: addCommandHandler: help
Command help already existed. Addition failed


COMMAND: 
 {
  "command":"fail",
  "payload": {
     "Does not":"really matter what is in here."
  }
 }

EXCEPTION: Oops, no match for command found in map. Please try again.


COMMAND: 
 {
  "command":"help",
  "payload": {
     "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }"
  }
 }

Controller::help: command: 
Enter json command in 'command':'<command>','payload': { // json payload of arguments }


COMMAND: 
 {
  "command":"help",
  "payload": {
     "reason":"Not usage."
  }
 }

Controller::help: command: 
EXCEPTION: Oops, no member "usage" present in payload JSON. Please try again.


COMMAND: 
 {
  "command": "sum_ints",
  "payload": {
     "addends": [1, 2, 3]
  }
 }

Controller::sum_ints command: 
6


COMMAND: 
 {
  "command": "sum_ints",
  "payload": {
     "addends": [1, 2, 3, 4, 5, 100, 999999999, -1, 0, -999]
  }
 }

Controller::sum_ints command: 
999999114


COMMAND: 
 {
  "command": "sum_ints",
  "payload": {
     "addends": [1, 2, 3, 4.5, 5.0, 100.1, 999999999.1, -1.1, 0, -999.1]
  }
 }

Controller::sum_ints command: 
6


COMMAND: 
 {
  "command": "sum_ints",
  "payload": {
     "addends": ["apples", "oranges", "turtles"]
  }
 }

Controller::sum_ints command: 
EXCEPTION: Oops, no integers in array. Please try again.


COMMAND: 
 {
  "command": "sum_ints",
  "payload": {
     "well":"formed",
     "json":"test"
  }
 }

Controller::sum_ints command: 
EXCEPTION: Oops, no member "addends" present in payload JSON. Please try again.


COMMAND: 
 {
  "command": "mean_ints",
  "payload": {
     "addends": [1, 2, 3, 4, 5]
  }
 }

Controller::mean_ints command: 
3


COMMAND: 
 {
  "command": "mean_ints",
  "payload": {
     "addends": [1, 2, 2]
  }
 }

Controller::mean_ints command: 
1


COMMAND: 
 {
  "command": "mean_ints",
  "payload": {
     "addends": [1, 2, 3, 4, 5, 100, 999999999, -1, 0, -999]
  }
 }

Controller::mean_ints command: 
99999911


COMMAND: 
 {
    "command": "query_payload",
    "payload": {
      "hello": "world",
      "t": true ,
      "f": false,
      "n": null,
      "i": 123,
      "pi": 3.1416,
      "a": [1, 2, 3, 4]
   }
 }

Controller::payload_type command: 
Type of member hello is String
Type of member t is True
Type of member f is False
Type of member n is Null
Type of member i is Number
Type of member pi is Number
Type of member a is Array


COMMAND: 
 {
  "command":"exit",
  "payload": {
     "booga":"gooba"
  }
 }

Controller::exit: command: 
EXCEPTION: Oops, no member "reason" present in payload JSON. Please try again.


COMMAND: 
 {
  "command":"exit",
  "payload": {
     "reason":"Exiting program on user request."
  }
 }

Controller::exit: command: 
Exiting program on user request.
COMMAND DISPATCHER: ENDED
armaan@ubuntuVM:build$
```

This output matches expectations.

### Example 2

Running the program with:
* `TEST_ALL` disabled
* `VERBOSE` enable
produced this output to the terminal, including user commands entered at prompt:

```bash
armaan@ubuntuVM:build$ ./dispatcher 
COMMAND DISPATCHER: STARTED
CommandDispatcher: addCommandHandler: help
Command help added to map.
CommandDispatcher: addCommandHandler: exit
Command exit added to map.
CommandDispatcher: addCommandHandler: sum_ints
Command sum_ints added to map.
CommandDispatcher: addCommandHandler: query_payload
Command query_payload added to map.
CommandDispatcher: addCommandHandler: mean_ints
Command mean_ints added to map.
CommandDispatcher: addCommandHandler: help
Command help already existed. Addition failed



COMMANDS: {"command":"exit", "payload":{"reason":"User requested exit."}}
	enter command : {"command":"help","payload":{"usage":"something arbitrary"}}


COMMAND: {"command":"help","payload":{"usage":"something arbitrary"}}
found command: help
found payload: {"usage":"something arbitrary"}
help matched a value in map!
Attempting to start command handler
Controller::help: command: 
something arbitrary



COMMANDS: {"command":"exit", "payload":{"reason":"User requested exit."}}
	enter command : {"command":"exit","payload":{"reason":"I've been looking at the computer for far too long, and my eyes are going square."}}


COMMAND: {"command":"exit","payload":{"reason":"I've been looking at the computer for far too long, and my eyes are going square."}}
found command: exit
found payload: {"reason":"I've been looking at the computer for far too long, and my eyes are going square."}
exit matched a value in map!
Attempting to start command handler
Controller::exit: command: 
I've been looking at the computer for far too long, and my eyes are going square.
COMMAND DISPATCHER: ENDED
armaan@ubuntuVM:build$ 
```

This output also matches expectations.


Test conditions
---

* Pass: 
  * Avoid anomalous behavior, including but not limited to:
    * unhandled exceptions
    * seg faults
    * other memory errors
  * Produce output identical to examples from prompt.
  * Successfully add commands to map.
    * If conflict, throw and handle exception.
  * Parse strings for JSON.
    * If no JSON present, throw and handle exception.
    * If JSON present, parse JSON for "command" and "payload" key/value pairs.
      * If one or both not present, throw and handle exception.
      * If both present, query map for matching command.
        * If no matching command, throw and handle exception.
        * If matching command found, pass value stored by "payload" key to command handler function.
  * Inside of each command handler function, parse value passed in for JSON.
    * If no match for required key/value pair, throw and handle exception.
    * If match for required key/value pair, safely determine if value contains valid data.
      * If value does not contain valid data, throw and handle exception.
      * If value either contains only valid data, or both valid and invalid data, handle command using valid data.
  * Continue indefinitely until "exit" command successfully handled.
* Fail: Any other result.


Method
---

### Coding


## Design Constraints

The project involved gluing together parts of an already-established codebase, and creating new functionality on top of it. To facilitate smooth operation, needed to establish what constraints the existing code placed on my implementations. Here are the constraints identified:

* main()
  * adds command handlers in Controller class to CommandDispatcher using addCommandHandler
  * invokes CommandDispatcher::dispatchCommand using a raw string.
* CommandDispatcher()
  * stores command handlers in Controller class using a private map
  * invokes command handler functions using either rapidjson::value or rapidjson::document objects
* Controller()
  * implements command handler functions
  * return values are bool
  
## Design Decisions

To design around these constraints, I needed to parse out a reasonable division of labor for the functions I implemented, and determine how information would be passed back and forth. I chose to allow functions at all depths to print to the user, except on error conditions. On errors, I chose to throw exceptions which get passed back to and handled by main(). Were I to continue to improve on this code, I would consider creating a shared print function for all the other functions to call so the output stream is consolidated to one place.

Here is a psuedo-code division of labor for my implementations:

* main()
  * prints success to user
  * catches failure message exceptions from all other parts of program and passes them to exceptionPrinter
* exceptionPrinter()
  * prints to user
  * returns to main
* CommandDispatcher()
  * parses raw string from main into a rapidjson::document
  * performs initial checks to verify that "command" and "payload" values exist
  * attempts to find command handler function matching "command" 
  * extracts "payload" value from document and passes by reference to command handler
  * prints success to user
  * passes failures as exceptions back to main
  * passes exceptions from Controller back to main
* Controller()
  * performs commanded functions
  * functions check whether their individual parameters are satisfied.
  * prints success to user
  * passes failures as exceptions back to CommandDispatcher


Requirements
---

> 5) Platform Dispatcher
> See attached *.cpp file

The prompt cpp file is available here: [/prompt/dispatcher_challenge.cpp](../prompt/dispatcher_challenge.cpp)


Prelims
---

### Research

Since I have not had prior exposure to several concepts needed for this task, I performed significant research and brainstorming to understand what was going on and what I needed to do. Some information about that process is available in these sections of the Appendix:

* [Parsing Code (Appendix)](Step_5_Appendix.md#parsing-code)
* [Brainstorming (Appendix)](Step_5_Appendix.md#brainstorming)
* [Research (Appendix)](Step_5_Appendix.md#research)

### Environment

The general environment remains the same as the previous steps.

#### CMAKE

I set up my CMAKE environment based on the prompt CMakeLists.txt available here: [/prompt/CMakeLists.txt](../prompt/CMakeLists.txt). My final CMakeLists.txt file has some changes. Commentary is available in the [Reference in Appendix](Step_5_Appendix.md#cmake).

### Doxygen

Considerations for creating documentation remain the same as for Step 3: [Reference in Step 3 Appendix](Step_3_Appendix.md#doxygen-creation)
