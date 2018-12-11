# Step 4: Dispatcher Challenge

## Prelims

* Familiarize myself with https://github.com/Tencent/rapidjson 
* Parse through [initial dispatcher_challenge.cpp prompt](../prompt/dispatcher_challenge.cpp) to determine if will be using DOM API or SAX API, among other things.

## Resources

https://jsonlint.com/ JSON validator

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
    
