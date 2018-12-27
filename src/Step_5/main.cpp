#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <exception>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function                    << know this
// std::bind                        << need to look up
// std::placeholders                << need to look up
// std::map                         << know this
// std::make_pair                   << need to look up

#include "rapidjson/document.h"         // DOM API
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

// Operational globals. Do not change.
bool g_done = false;


// User globals. Ok to change.
bool TEST_ALL = true;        // test all functionality before prompting user
bool DEBUG = false;          // turn on debug messages



/** \brief DEBUG PRINTER function
 *
 *  \param id an integer that defines thread number (0 for main)
 *  \param msg a string containing message to be printed
 *
 * Prints to standard error
 * \warning NOT thread-safe; must be called within a thread-safe scope
 */
void DBG_PRNTR(std::string id, std::string msg) {
    
    std::cerr << "***\tDEBUG from " << id << ": " 
              << msg << " \t***" << std::endl;
}



/** \brief EXCEPTION HANDLER function
 *
 *  \param excpt a const char* containing a message
 * 
 * Prints to standard out
 * \warning NOT thread-safe; must be called within a thread-safe scope
 */
void ExceptionPrinter(const char* excpt) {
    cout << "\nOops, " << excpt << ". Please try again.\n\n" << endl; 
}



//
// TEST COMMANDS
//

auto help_command = R"(
 {
  "command":"help",
  "payload": {
     "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }"
  }
 }
)";

auto help_command_fail = R"(
 {
  "command":"help",
  "payload": {
     "reason":"Not usage."
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

auto exit_command_fail = R"(
 {
  "command":"exit",
  "payload": {
     "booga":"gooba"
  }
 }
)";

auto sum_command = R"(
 {
  "command": "sum",
  "payload": {
     "addends": [1, 2, 3]
  }
 }
)";

auto sum_command_large = R"(
 {
  "command": "sum",
  "payload": {
     "addends": [1, 2, 3, 4, 5, 100, 999999999, -1, 0, -999]
  }
 }
)";

auto sum_command_fail_1 = R"(
 {
  "command": "sum",
  "payload": {
     "addends": ["apples", "oranges", "turtles"]
  }
 }
)";

auto sum_command_fail_2 = R"(
 {
  "command": "sum",
  "payload": {
     "well":"formed",
     "json":"test"
  }
 }
)";

auto query_payload_command = R"(
 {
  "command": "query_payload",
  "payload": {
     "well":"formed",
     "json":"test",
     "a": [1,2,3,4],
     "number1":1,
     "number2":34.7,
     "b": [
        "something":"more",
        "here":"too"
        ]
  }
 }
)";

auto query_payload_command_2 = R"(
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
)";


/** \brief controller class of functions to "dispatch" from Command Dispatcher
 *
 */
class Controller {
public:

    /** \brief command handler for help
     *
     * \param payload a JSON string possibly containing a "usage" message
     *
     * \return true if command handled successfully, otherwise generate exception
     */
    static bool help(rapidjson::Value &payload)
    {
        cout << "Controller::help: command: ";
        
        // DEBUG - stringify the value
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        payload.Accept(writer);
        std::cout << buffer.GetString() << std::endl;
        // */
        
        // create iterator to look for "usage".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("usage");

        // safely check for a value "usage"
        if (itr != payload.MemberEnd()) {

            // value exists. print to user
            cout << itr->value.GetString() << endl; /// \bug change to shared_print?
        }
        // if does not exist, throw exception
        else { throw "no member \"usage\" present in payload JSON"; }
        // */

        return true;
    }
    

    /** \brief command handler for exit
     *
     * \param payload a JSON string possibly containing a reason
     *
     * \return true if command handled successfully, otherwise generate exception
     */
    static bool exit(rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        // create iterator to look for "reason".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("reason");

        // safely check for a value "reason"
        if (itr != payload.MemberEnd()) {

            // value exists. print to user
            cout << itr->value.GetString() << endl; /// \bug change to shared_print?
            
            // terminate loop in main, causing controlled exit.
            g_done = true;

        }
        // if does not exist, throw exception
        else { throw "no member \"reason\" present in payload JSON"; }
        // */

        return true;
    }


    // implement 3-4 more commands


    /** \brief command handler for add 
     *
     * Still am not sure what kind of functionality I need inside of this class.
     * Perhaps everything just need to print or control program flow? If so this
     * function will be useless and should be removed.
     */
    static bool sum(rapidjson::Value &payload)
    {
        cout << "Controller::sum command: \n";

        // create iterator to look for "addends".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("addends");

        // safely check for a value "reason"
        if (itr != payload.MemberEnd()) {

            // value exists. check for array of numbers
            //
            // note: probably need to differntiate between ints and floats
            // rapidJSON does not distinguish between the two, but C++ does

            // reference for consecutive access
            const Value& a = payload["addends"];
            
            for (Value::ConstValueIterator itr_a = a.Begin(); itr_a != a.End(); ++itr_a) {
                
                //printf("Type of member %s is %s\n",
                //        itr->name.GetString(), kTypeNames[itr->value.GetType()]);
            }



    
               
               //printf("%d ", itr_a->GetInt()); 


            if ( true ) {

                // numbers exist. add them together (even if array of only 1 element)

                // print sum to user
                cout << itr->value.GetString() << endl; /// \bug change to shared_print?


            }
            // if numbers do not exist, throw exception
            else { throw "no numbers exist in \"addends\" array, or \"addends\" not array"; }
            

        }
        // if does not exist, throw exception
        else { throw "no member \"addends\" present in payload JSON"; }
        // */



        return true;
    }


    static bool query_payload(rapidjson::Value &payload) 
    {
        cout << "Controller::payload_type command: \n";

        // array storing typenames
        static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", 
                                            "String", "Number" };

        // 
        for (Value::ConstMemberIterator itr = payload.MemberBegin();
                itr != payload.MemberEnd(); ++itr) {
            printf("Type of member %s is %s\n",
                    itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        }

        return true;
    }


private:
    //static std::string CUR_SCOPE = "class Controller";     // DEBUG

};



// Bonus Question: why did I type cast this?
// typedef: to make life easier
// typecast: to pass in a rapidjson::Value& and return a bool
typedef std::function<bool(rapidjson::Value &)> CommandHandler;



class CommandDispatcher {
public:
    // ctor - need impl
    CommandDispatcher()
    {
        // What goes in here likely depends on how the map is going to be 
        // populated.
        
    }


    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
        //
        // AFAIK, virtual refers to members that an inhereting class will have
        // but the current class does not. I'm not sure why a class would not
        // have a destructor. Maybe this class only uses the implicit destructor,
        // but inhereting classes need more complex ones?
        //
        // Still not sure why this class would need to be inhereted yet though.
    }


    /** \brief adds a command and handler pair to the map
     *
     *  \param command a string 
     *  \param handler a CommandHandler object containing a DOM value
     *
     *  \return true is addition successful, false if not
     */
    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;

        bool add_succeeded = true;

        /// add command and handler pair to map
        auto test = command_handlers_.insert( std::make_pair( command, handler));

        if( test.second == false) {
            cout << "Command " << command << " already existed. Addition failed" << endl;
            add_succeeded = false;
        }
        else {
            cout << "Command " << command << " added to map." << endl;
            
        }// */

        return add_succeeded;
    }


    /** \brief initial receiver of command
     *
     *  \param command_json a raw string of JSON commands
     *
     *  \return true if command dispatched to handler
     */
    bool dispatchCommand(std::string command_json)
    {
        cout << "\nCOMMAND: " << command_json << endl;
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it to dispatchCommand"); 

        const char *command_ptr = command_json.c_str();  /// \bug maybe typecast instead?
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past command_ptr");
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, command_ptr);
 
        
        // parse the received string
        this->doc.Parse(command_ptr);
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past doc.Parse");

        // check if JSON passed in. If not, throw error.
        if(this->doc.HasParseError()) { throw "malformed JSON"; }
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past doc.HasParseError");


        // create iterators for "command" and "payload", if they exist.
        rapidjson::Value::ConstMemberIterator itr_c = this->doc.FindMember("command");
        rapidjson::Value::ConstMemberIterator itr_p = this->doc.FindMember("payload");


        // safely check for a value "command"
        if (itr_c != this->doc.MemberEnd()) {
            if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it into itr_c if"); 
            cout << "found command: " << itr_c->value.GetString() << endl; /// \bug change to shared_print?
        }
        // if does not exist, throw exception
        else { throw "no member \"command\" present in JSON"; }
        // */


        // safely check for a value "payload"
        if (itr_p != this->doc.MemberEnd()) {
            if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it into itr_p if");

            // DEBUG - stringify the value
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            itr_p->value.Accept(writer);
            // */

            cout << "found payload: " 
                 << buffer.GetString() << endl; /// \bug change to shared_print?
        }
        // if does not exist, throw exception
        else { throw "no member \"payload\" present in JSON"; }
        // */


        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past iterators");

        // DEBUG - stringify the DOM
        /*StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        this->doc.Accept(writer);
        std::cout << buffer.GetString() << std::endl;
        // */

        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past get payload");


        // command routing
        bool command_found = false;  // until proven otherwise
        for( this->map_itr = command_handlers_.begin() ;
             this->map_itr != command_handlers_.end(); this->map_itr++ ) {

            // try to find match in map for command string
            if( itr_c->value.GetString() == (*map_itr).first ) {
                cout << itr_c->value.GetString() << " matched a value in map!" << endl
                     << "Attempting to start command handler" << endl;

                command_found = true;
                
                // dispatch payload to command handler
                (*map_itr).second(this->doc["payload"]); 

                break; // don't keep looping longer than necessary
            }
        }

        // if command does not exist, throw exception
        if ( ! command_found) {
            if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it into ! command_found if");
            throw "no match for command found in map"; 
        }

        return command_found; // should always be true if execution falls through to here
    }

private:
    std::string CUR_SCOPE = "class CommandDispatcher";  // DEBUG - current scope
    const char * test = "test string";                  // DEBUG - test string

    std::map<std::string, CommandHandler> command_handlers_; /// map of command handlers

    std::map<std::string, CommandHandler>::iterator map_itr = 
        this->command_handlers_.begin(); /// iterator for the map

    rapidjson::Document doc;  // DOM API document

    // Question: why delete these?
    //
    // generate compile error if copy attempted
    // (supposed to be un-copyable)

    // delete unused constructors
    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;

};



int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;  // controller class of functions to "dispatch" from Command Dispatcher

    // Add available command handlers in Controller class to CommandDispatcher manually
    command_dispatcher.addCommandHandler( "help", controller.help); // needs static functions
    command_dispatcher.addCommandHandler( "exit", controller.exit); // maybe use std::bind instead
    command_dispatcher.addCommandHandler( "sum", controller.sum);   // and change to non-static
    command_dispatcher.addCommandHandler( "query_payload", controller.query_payload); 

    // DEBUG - should generate warning on fail because already exists in map
    //command_dispatcher.addCommandHandler( "help", controller.help); //needs static

    
    // DEBUG - send commands
    // uncomment to rapidly test all functionality
    try { command_dispatcher.dispatchCommand(help_command_fail); } // should fail
    catch (const char* excpt) { ExceptionPrinter(excpt); } // handle exception
    try { command_dispatcher.dispatchCommand(help_command); }
    catch (const char* excpt) { ExceptionPrinter(excpt); } 

    /*try { command_dispatcher.dispatchCommand(sum_command); }
    catch (const char* excpt) { ExceptionPrinter(excpt); } // */
    
    try { command_dispatcher.dispatchCommand(query_payload_command_2); }
    catch (const char* excpt) { ExceptionPrinter(excpt); }
    
    /*try { command_dispatcher.dispatchCommand(exit_command_fail); }  // should fail
    catch (const char* excpt) { ExceptionPrinter(excpt); } 
    try { command_dispatcher.dispatchCommand(exit_command); }
    catch (const char* excpt) { ExceptionPrinter(excpt); }   
    // */


    // command line interface for testing
    string command;
    while( ! g_done ) {
        cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
        cout << "\tenter command : ";
        getline(cin, command);
        try { command_dispatcher.dispatchCommand(command); }
        catch (const char* excpt) { ExceptionPrinter(excpt); } 
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
