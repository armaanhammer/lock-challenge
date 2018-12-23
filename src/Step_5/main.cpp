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
    
    std::cerr << "***\tDEBUG from " << id << ": " 
              << msg << " \t***" << std::endl;
}





//
// TEST COMMANDS
//
/*auto help_command = R"(
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }",
  }
 }
)"; // */ // I don't have a damn clue why this one doesn't work.

auto help_command = R"(
 {
  "command":"help",
  "payload": {
     "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }"
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


auto help_command_fail = R"(
 {
  "command":"help",
  "payload": {
     "reason":"Write something."
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
     * \param payload a JSON string possibly containing a reason
     *
     */
    static bool help(rapidjson::Value &payload)
    {
        cout << "Controller::help: command: ";

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
        
        // 3. Stringify the DOM
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        payload.Accept(writer);
        // Output {"project":"rapidjson","stars":11}
        std::cout << buffer.GetString() << std::endl;
        // */
        
        /*
        // parse the received string
        payload.Parse(command_ptr);
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past payload.Parse");

        // check if JSON passed in. If not, throw error.
        if(payload.HasParseError()) { throw "payload has malformed JSON"; }
        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past payload.HasParseError");


        // create iterator to look for "usage".
        rapidjson::Value::ConstMemberIterator itr_r = this->doc.FindMember("command");


        // safely check for a value "usage"
        if (itr_r != this->doc.MemberEnd()) {
            if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it into itr_r if"); 
            printf("%s\n", itr_c->value.GetString()); /// \bug change to shared_print?
        }
        // if does not exist, throw exception
        else { throw "no member \"usage\" present in payload JSON"; }
        // */

        




        return true;
    }
    
    /** \brief command handler for exit
     *
     */
    static bool exit(rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        // print payload value "reason"
        //
        // Q: what condition might trigger a false return?

        // terminate loop in main, causing controlled exit.
        g_done = true;

        return true;
    }

    /** \brief command handler for add 
     *
     * Still am not sure what kind of functionality I need inside of this class.
     * Perhaps everything just need to print or control program flow? If so this
     * function will be useless and should be removed.
     */
    bool add(rapidjson::Value &payload)
    {
        cout << "Controller::add command: \n";

        // implement

        return true;
    }

    static bool write_json(rapidjson::Value &payload) 
    {
        cout << "Controller::write_json command: \n";

        // implement

        return true;
    }









    // implement 3-4 more commands


private:
    std::string CUR_SCOPE = "class Controller";     // DEBUG

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

        bool ret_val = true;

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
        
        /// add command and handler pair to map
        auto test = command_handlers_.insert( std::make_pair( command, handler));

        // DEBUG
        //cout << str(test) << endl;

        
        //if( command_handlers_.insert( std::make_pair( command, handler))) {
        //if( test == std::make_pair( command, handler)) {
        if( test.second == false) {
            cout << "Command " << command << " already existed. Addition failed" << endl;
            //cout << " with a value of " << ret.first->second << '\n';
            ret_val = false;
        }
        else {
            cout << "Command " << command << " added to map." << endl;
            
        } // */

        return ret_val;
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
            printf("%s\n", itr_c->value.GetString()); /// \bug change to shared_print?
        }
        // if does not exist, throw exception
        else { throw "no member \"command\" present in JSON"; }
        // */


        // safely check for a value "payload"
        if (itr_p != this->doc.MemberEnd()) {
            if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it into itr_p if"); 
            /// \bug of course this won't work, it's not a string.
            //printf("%s\n", itr_p->value.GetString());
            
            // create a new doc containing only "payload" Value

        }
        // if does not exist, throw exception
        else { throw "no member \"payload\" present in JSON"; }
        // */


        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past iterators");

        
        // 3. Stringify the DOM
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        this->doc.Accept(writer);
        // Output {"project":"rapidjson","stars":11}
        std::cout << buffer.GetString() << std::endl;
        // */
        
        // get payload
        //rapidjson::Value payload = get_payload( this->doc);

        //rapidjson::Value payload = this->doc["payload"];
        //payload_test = this->doc["payload"];

        if(DEBUG) DBG_PRNTR(this->CUR_SCOPE, "made it past get payload");



 




        // command routing
        /// \bug This may not guarantee that only one command will be run. That depends on whether
        ///      JSON supports multiple elements with the same name or not.
        bool command_found = false;  // until proven otherwise
        for( this->map_itr = command_handlers_.begin() ;
             this->map_itr != command_handlers_.end(); this->map_itr++ ) {

            // try to find match in map for command string
            if( itr_c->value.GetString() == (*map_itr).first ) {
                cout << itr_c->value.GetString() << " matched a value in map!" << endl
                     << " Attempting to start command handler" << endl;

                command_found = true;
                
                // dispatch command
                //(*map_itr).second(this->doc); /// \bug change this to doc containing only
                                              ///      "payload" Value
                (*map_itr).second(this->doc["payload"]); 
            }
        }

        // if does not exist, throw exception
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
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    // Implement
    // add command handlers in Controller class to CommandDispatcher using addCommandHandler
    //
    // Using polymorphism? Probably not
    
    // Add available commands manually
    command_dispatcher.addCommandHandler( "help", controller.help); //needs static
    command_dispatcher.addCommandHandler( "exit", controller.exit); //maybe use std::bind instead

    //DEBUG - should fail because already exists in map
    //command_dispatcher.addCommandHandler( "help", controller.help); //needs static

    //DEBUG
    try { command_dispatcher.dispatchCommand(help_command); }
    catch (const char* e) { cout << "Oops, " << e << ". Please try again." << endl; }
    cout << "\n\n\n"; // */
    /*try { command_dispatcher.dispatchCommand(help_command_debug); }
    catch (const char* e) { cout << "Oops, " << e << ". Please try again." << endl; }
    cout << "\n\n\n"; // */
    try { command_dispatcher.dispatchCommand(exit_command); }
    catch (const char* e) { cout << "Oops, " << e << ". Please try again." << endl; }
    // */

    // command line interface for testing
    string command;
    while( ! g_done ) {
        cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
        cout << "\tenter command : ";
        getline(cin, command);
        try {
            command_dispatcher.dispatchCommand(command);
        }
        catch (const char* e)
        {
            cout << "Oops, " << e << ". Please try again." << endl;
        }


    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
