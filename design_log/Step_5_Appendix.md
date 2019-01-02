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

After some trial and error, it appears that debug symbols are not added to the executable with this CMakeLists.txt. I need to run gdb, so I tried adding this line: 

```set(CMAKE_BUILD_TYPE Debug)```

but it did not seem to have any effect. Searching around a bit showed that I can pass flags in explicitly, so I added this line as well: 

```
add_compile_options(-Wall -Wextra -Wpedantic -g)
```



<br>

Research
---

## Dispatcher abstraction

Unfamiliar with Dispatcher abstraction --  more familiar with Publisher / Subscriber model, which I have used on multiple projects recently. For instance, both the CAN bus, and ROS (Robot Operating System) use them. Found [this article](https://hackernoon.com/observer-vs-pub-sub-pattern-50d3b27f838c) that compares the two models.



Taken to be an event dispatcher, which maps functions to be called when string or int objects are passed to a message-passing interface. 

Two components:
1. Message-passing interface
1. Parameterized method-passing for inhereted objects.


https://www.youtube.com/watch?v=yD47_hq75q0

https://www.youtube.com/watch?v=Tu3Y2_3QaUY

Indentifying the fact that I probably need a much more solid understand of this construct, so diverting to research it more.

### UML Design Patterns

Unified Modeling Language

https://en.wikibooks.org/wiki/Introduction_to_Software_Engineering/UML/Introduction

#### Observer pattern

![UML Observer design pattern](https://doanduyhai.files.wordpress.com/2012/08/observer.png)


#### Dispatcher pattern

![UML Dispatcher design pattern](https://doanduyhai.files.wordpress.com/2012/08/dispatcher1.png) ![UML Dispatcher design pattern](https://doanduyhai.files.wordpress.com/2012/08/dispatcher_alternative.png)





https://doanduyhai.wordpress.com/2012/08/04/design-pattern-the-asynchronous-dispatcher/



https://en.wikibooks.org/wiki/Introduction_to_Software_Engineering/Architecture/Design_Patterns


##### DOM

Document Object Model -- A programming interface for HTML and XML documents. 

https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model/Introduction


<br>

Parsing Code
---

What is meant by "dispatching"? I assume data is moving from somewhere to somewhere else. I am having trouble understanding data movement in the code provided though.

### What is this actually doing?

* The auto test commands at the top appear to map a single JSON object to a C++ variable
* The typecasted typedef line may be revealing:

```C++
typedef std::function<bool(rapidjson::Value &)> CommandHandler;
```

alias to a function that takes a rapidjson::value& and returns bool


#### Typedef
  Syntax: *typedef data_type new_name;*\
  *data_type*: An existing type or user defined type created using structure/union.\
  *new_name*: alias to the existing type or user defined type.

  so:\
  `std::function<bool(rapidjson::Value &)>`    <-- existing type\
  `CommandHandler`    <-- alias to existing type
  

  
#### Typecast


#### Trying to parse this too:

    // add command handlers in Controller class to CommandDispatcher using addCommandHandler
    
---
## Still puzzling over this:

```C++
// Bonus Question: why did I type cast this?
// typedef: to make life easier
// typecast: to pass in a rapidjson::Value& and return a bool
typedef std::function<bool(rapidjson::Value &)> CommandHandler;
```

and

```C++
class CommandDispatcher {
  ...
private:
    std::map<std::string, CommandHandler> command_handlers_; /// map of command handlers
```

This is a map of functions called `command_handlers_`. The functions take in `rapidjson::Value &` and return `bool`. The map is indexed by instances of `std::string`. 

It seems like I am supposed to populate it with references to the public functions in the Controller class. I would like to do this programmatically so if I add members to the Controller class, the map will self-populate.

---
  
  
  
  
  
  


### class Controller

#### bool help()

##### `rapidjson::Value &payload`

  per http://rapidjson.org/classrapidjson_1_1_generic_value.html
  
<br>

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

<br>

Brainstorming
---

### Adding command handlers to map

Trying to think of a way to programmatically populate a map with member functions of a class.

Have a class with a bunch of members:

```C++
class Controller {
public:
    bool some_function(rapidjson::Value &payload) {
        //stuff
    }
    bool another_function(rapidjson::Value &payload) {
        //other stuff
    }
...
}
```

There's a typedef sitting in between the classes:

```C++
typedef std::function<bool(rapidjson::Value &)> CommandHandler;
```


Then have another class with a map, and an add function:

```C++
class CommandDispatcher {
public:
    bool addCommandHandler(std::string command, CommandHandler handler)
        {
            // do something
        }
  ...
private:
    std::map<std::string, CommandHandler> command_handlers_;
```

I think I am supposed to either traverse the Controller class and populate the map in CommandDispatcher based on the member functions of the Controller class. Or, I am supposed to do the opposite, and populate member functions in the Controller class based on what's in the map. That second thing doesn't seem possible in C++, so I assume it's the first thing I'm supposed to do.

Deciding to manually add the functions in main()

### Controller::help

        // implement
        //
        // print payload value "usage"
        //
        // Q: What condition might trigger a false return?
        //
        // Q: Should gatekeeping be performed prior to this 
        // function call? i.e. if this function is called, 
        // should it assume that the payload has a value "usage"?
        //    If so, likey the map will need to be queried for 
        //    allowable values to pass in.
        //    If not, then this function should probably 
        //    return false if no value "usage" exists in payload.
        
### CommandDispatcher::~CommandDispatcher

        // question why is it virtual? Is it needed in this case?
        //
        // AFAIK, virtual refers to members that an inhereting class will have
        // but the current class does not. I'm not sure why a class would not
        // have a destructor. Maybe this class only uses the implicit destructor,
        // but inhereting classes need more complex ones?
        //
        // Still not sure why this class would need to be inhereted yet though.

### CommandDispatcher::addCommandHandler

        // implement
        //
        // I think I need to use this to add commands to the map (private memeber
        // of this class). I'm not sure if I am supposed to call this programatically
        // or statically. ie. Do I create a function that iterates through the public
        // members of the Controller class and dynamically (at compile time, or maybe
        // even at runtime) adds them to the map? Or am I supposed to just manually 
        // write functions that do that inside of main()?
        //
        // It looks like C++ is not able to implement a programmatic enumberation of
        // a class's public member functions, so I need to do it manually in main().
 
  
<br>

Reference
---

### std::function                    << know this

### std::bind                        << need to look up

* Binds parameters to functions
* Guaranteed to make a copy of argument passed in.
* NOT lightweight! Use judiciously.
* Swallows extra arguments passed to it.

example usage:
```C++
#include <functional>
#include <iostream>

template<typename T>
void print(T i, const std::string &s)
{
  std:;cout << i << ' ' << s << '\n';
}

int main()
{
  int i = 5;
  cont auto f = std::bind(&print<int>, std::placeholders::_1, std::placeholders::_2);
  
  f(i, "hello", 1, 2, 3, 4);  // more parameters get thrown away
  i = 6;
  f(i, world");
  
  std::function<void (const std::string &, int)> f2(f);
  f2(i, "test"); // more parameters will produce a compile error
  
}
```

output:

```
shell:user$ ./bind.out
5 hello
6 world
6 test
```



source: https://www.youtube.com/watch?v=JtUZmkvroKg

### std::placeholders                << need to look up

### std::map                         << know this

key / value pair

usage example:

```C++
/*
* map
* 
* - No duplicated key
*/
map<char,int> mymap;
mymap.insert ( pair<char,int>('a',100) );
mymap.insert ( make_pair('z',200) );

map<char,int>::iterator it = mymap.begin();
mymap.insert(it, pair<char,int>('b',300));  // "it" is a hint

it = mymap.find('z');  // O(log(n))

// showing contents:
for ( it=mymap.begin() ; it != mymap.end(); it++ )
  cout << (*it).first << " => " << (*it).second << endl;
```

source: https://www.youtube.com/watch?v=6iyzPed7FrM

Note: class template does NOT infer parameter types. Function template does.

### std::make_pair                   << need to look up
  

<br> 

Exception Handling
---

initially used `assert()` statements, then converted to `return false` statements, but want to capture more information about failing JSON. Using exceptions instead.

try, catch, throw




<br>

Useful Tools
---

#### nm

#### c++filt

usage:

```
nm a.out | c++filt -t | less
```

  
