# Step 5: Dispatcher Challenge

To duplicate
---

1. In [/src/Step_5/ directory](../src/Step_5) run ``. Build environment will be set up in [/src/Step_5/build/ directory](../src/Step_5/build/)
1. Enure that the bool value for `TEST_ALL` inside of main.cpp is set to `true`.
1. In [/src/Step_5/build/ directory](../src/Step_5/build/) run `make`. Executable will build as *dispatcher*. 
1. To run executable, run `./dispatcher`.
1. If the final command (exit_command) on line is not commented out, execution will terminate. If it is commented out, execution will fall through to a user prompt. You may enter JSON for parsing.


Files
---

* [main.cpp](../src/Step_5/main.cpp)
* [main_no_debug.cpp](../src/Step_5/main_no_debug.cpp)  [(explanation)]()
* []()
* [Doxyfile]()


Documentation
---

Doxygen documentation is available for this step of the project. 

| [Step 5 main page](https://armaanhammer.github.io/lock-challenge/Step_5/html/index.html) | | |
| --- | --- | --- |
| main.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_5/html/main_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_5/html/main_8cpp_source.html) |
| main_no_debug.cpp | [Documentation](https://armaanhammer.github.io/lock-challenge/Step_5/html/main__no__debug_8cpp.html) | [Code](https://armaanhammer.github.io/lock-challenge/Step_5/html/main__no__debug_8cpp_source.html) |


Results
---

It produced this output to the terminal when combining standard out and standard error:

```bash
```

And this output when only printing standard out:

```bash
```

And this output when only printing standard error:

```bash
```

These outputs match expectations.


Test conditions
---

* Pass: 
  * 
  * Avoid anomalous behavior, including but not limted to, .
  * Demonstrate that .
* Fail: Any other result.


Method
---

### Coding


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
