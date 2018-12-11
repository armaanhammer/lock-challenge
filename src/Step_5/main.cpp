#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <functional>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function
// std::bind
// std::placeholders
// std::map
// std::make_pair

#include "rapidjson/document.h"         // DOM API
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

bool g_done = false;


//DEBUG
//StringBuffer buffer;



bool DEBUG = true;          // turn on debug messages

/** \brief DEBUG PRINTER function
 *
 *  \param id an integer that defines thread number (0 for main)
 *  \param msg a string containing message to be printed
 *
 * Prints to standard out
 * \warning NOT thread-safe; must be called within a thread-safe scope
 */
void DBG_PRNTR(std::string id, std::string msg) {
    
    std::cout << "***\tDEBUG from" << id << ": " 
              << msg << "\t***" << std::endl;
}





//
// TEST COMMANDS
//
auto help_command = R"(
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }",
  }
 }
)";

auto exit_command = R"(
 {
  "command":"exit",
  "payload": {
     "reason":"Exiting program on user request."
  }
 }
)";


/** \brief controller class of functions to "dispatch" from Command Dispatcher
 *
 */

class Controller {
public:

    /** \brief command handler for help
     *
     */
    bool help(rapidjson::Value &payload)
    {
        cout << "Controller::help: command: ";

        // implement

        return true;
    }
    
    /** \brief command handler for exit
     *
     */
    bool exit(rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        // implement

        return true;
    }

    /** \brief command handler for add 
     *
     */
    bool add(rapidjson::Value &payload)
    {
        cout << "Controller::add command: \n";

        // implement

        return true;
    }





    // implement 3-4 more commands
};

// Bonus Question: why did I type cast this?
typedef std::function<bool(rapidjson::Value &)> CommandHandler;



class CommandDispatcher {
public:
    // ctor - need impl
    CommandDispatcher()
    {
    }

    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
    }

    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;

        // implement

        return true;
    }

    /** \brief initial receiver of command
     *
     * does: what?
     */
    bool dispatchCommand(std::string command_json)
    {
        cout << "COMMAND: " << command_json << endl;
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it to dispatchCommand"); 

        // implement

        const char *command_ptr = command_json.c_str();  // maybe typecast instead?
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past command_ptr");
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, command_ptr);
        if(DEBUG) cout << *command_ptr << endl;
        if(DEBUG) cout << &command_ptr << endl;
        //if(DEBUG) cout << command_ptr* << endl;
        //if(DEBUG) cout << command_ptr& << endl;
 
        

        this->doc.Parse(command_ptr); // parse the received string
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past doc.Parse"); 

        // check if a value exists
        //rapidjson::Value::ConstMemberIterator itr = this->doc.FindMember("hello");
        //this->doc.FindMember(this->test);
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past doc.Parse"); 

        /*
        if (itr != this->doc.MemberEnd()) {
            if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it into for loop"); 

            printf("%s\n", itr->value.GetString());
        }// */


        // 3. Stringify the DOM
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        this->doc.Accept(writer);
        // Output {"project":"rapidjson","stars":11}
        std::cout << buffer.GetString() << std::endl;


        return true;
    }

private:
    std::string CUR_SCOPE = "class CommandDispatcher"; // DEBUG


    std::map<std::string, CommandHandler> command_handlers_; /// map of command handlers

    rapidjson::Document doc;
    const char * test = "test string";


    // Question: why delete these?

    // delete unused constructors
    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;

};

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    // Implement
    // add command handlers in Controller class to CommandDispatcher using addCommandHandler

    // command line interface for testing
    string command;
    while( ! g_done ) {
        cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
        cout << "\tenter command : ";
        getline(cin, command);
        command_dispatcher.dispatchCommand(command);
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
