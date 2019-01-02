#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <exception>
#include <vector>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function                    << need
// std::bind                        << may not need
// std::placeholders                << need
// std::map                         << need
// std::make_pair                   << need

#include "rapidjson/document.h"         // DOM API
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

// Operational globals. Do not change.
bool g_done = false;


// User globals. Ok to change.
bool TEST_ALL = true;        ///< test all functionality before prompting user
bool VERBOSE = false;        ///< turn on verbose messages
bool DEBUG = false;          ///< turn on debug messages



///
/// \brief DEBUG PRINTER function
///
///  \param id an integer that defines thread number (0 for main)
///  \param msg a string containing message to be printed
///
/// Prints to standard error
/// \warning NOT thread-safe; must be called within a thread-safe scope
///
void debugPrinter(std::string id, std::string msg) {
    
    std::cerr << "***\tDEBUG from " << id << ": " 
              << msg << " \t***" << std::endl;
}



///
/// \brief EXCEPTION HANDLER function
///
///  \param excpt a const char* containing a message
/// 
/// Prints to standard out
/// \warning NOT thread-safe; must be called within a thread-safe scope
///
void exceptionPrinter(const char* excpt) {
    cout << "EXCEPTION: Oops, " << excpt 
         << ". Please try again." << endl; 
}



//
// TEST COMMANDS
//

auto fail_command = R"(
 {
  "command":"fail",
  "payload": {
     "Does not":"really matter what is in here."
  }
 }
)";


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
  "command": "sum_ints",
  "payload": {
     "addends": [1, 2, 3]
  }
 }
)";

auto sum_command_long = R"(
 {
  "command": "sum_ints",
  "payload": {
     "addends": [1, 2, 3, 4, 5, 100, 999999999, -1, 0, -999]
  }
 }
)";

auto sum_command_floats = R"(
 {
  "command": "sum_ints",
  "payload": {
     "addends": [1, 2, 3, 4.5, 5.0, 100.1, 999999999.1, -1.1, 0, -999.1]
  }
 }
)";

auto sum_command_fail_1 = R"(
 {
  "command": "sum_ints",
  "payload": {
     "addends": ["apples", "oranges", "turtles"]
  }
 }
)";

auto sum_command_fail_2 = R"(
 {
  "command": "sum_ints",
  "payload": {
     "well":"formed",
     "json":"test"
  }
 }
)";

auto mean_command = R"(
 {
  "command": "mean_ints",
  "payload": {
     "addends": [1, 2, 3, 4, 5]
  }
 }
)";

auto mean_command_fraction = R"(
 {
  "command": "mean_ints",
  "payload": {
     "addends": [1, 2, 2]
  }
 }
)";

auto mean_command_long = R"(
 {
  "command": "mean_ints",
  "payload": {
     "addends": [1, 2, 3, 4, 5, 100, 999999999, -1, 0, -999]
  }
 }
)";

auto query_payload_command = R"(
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



///
/// \brief controller class of functions to "dispatch" from Command Dispatcher
///
class Controller {
public:

    ///
    /// \brief command handler for help
    ///
    /// \param payload a JSON string possibly containing a "usage" message
    ///
    /// \throws exception if JSON contains no value "usage"
    ///
    /// \return true if command handled successfully
    ///
    /// \todo change cout call to shared_print?
    ///
    static bool help(rapidjson::Value &payload)
    {
        cout << "Controller::help: command: \n";
        
        // create iterator to look for "usage".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("usage");

        // safely check for a value "usage"
        if (itr != payload.MemberEnd()) {

            // value exists. print to user
            cout << itr->value.GetString() << endl;
        }
        // if does not exist, throw exception
        else { 
            throw "no member \"usage\" present in payload JSON"; 
        }

        return true;
    }
    

    ///
    /// \brief command handler for exit
    ///
    /// \param payload a JSON string possibly containing a reason
    ///
    /// \return true if command handled successfully
    /// 
    /// \todo change cout call to shared_print?
    ///
    static bool exit(rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        // create iterator to look for "reason".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("reason");

        // safely check for a value "reason"
        if (itr != payload.MemberEnd()) {

            // value exists. print to user
            cout << itr->value.GetString() << endl;

            // terminate loop in main, causing controlled exit.
            g_done = true;

        }
        // if does not exist, throw exception
        else { 
            throw "no member \"reason\" present in payload JSON"; 
        }

        return true;
    }


    //
    // Additional commands below 
    //


    ///
    /// \brief command handler for query_payload
    ///
    /// \param payload a JSON string possibly containing any allowed JSON value(s)
    ///
    /// \return true if command handled successfully, otherwise generate exception
    ///
    /// \sa derived from: http://rapidjson.org/md_doc_tutorial.html#QueryObject
    ///
    static bool query_payload(rapidjson::Value &payload) 
    {
        cout << "Controller::payload_type command: \n";

        // array storing typenames
        static const char* kTypeNames[] = { 
            "Null", 
            "False", 
            "True", 
            "Object", 
            "Array", 
            "String", 
            "Number" 
        };
 
        for (Value::ConstMemberIterator itr = payload.MemberBegin();
                itr != payload.MemberEnd(); ++itr) {

            cout << "Type of member " << itr->name.GetString()
                 << " is " << kTypeNames[itr->value.GetType()] << endl;
        }

        return true;
    }



    ///
    /// \brief command handler for sum_ints
    ///
    /// \param payload a JSON string possibly containing an array value
    ///
    /// \return true if command handled successfully, otherwise generate exception
    ///
    /// \throws exception when payload contains no value "addends"
    /// \throws exception when addends is not an array
    /// \throws exception when no integers in array
    ///
    /// This function looks for an array named "addends". If it exists, it traverses
    /// it and prints the sum of all integers present in the array. All non-integer 
    /// members of the array are ignored.
    ///
    static bool sum_ints(rapidjson::Value &payload)
    {
        cout << "Controller::sum_ints command: \n";

        // create iterator to look for "addends".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("addends");

        // safely check for a value "addends"
        if ( itr != payload.MemberEnd( )) {

            // value exists. check for array of numbers
            
            int num_ints = 0; // efficient vector allocation

            // reference for consecutive access
            const Value& a = payload["addends"];

            // make sure addends is an array. throw exception if not
            if ( ! a.IsArray() ) 
                throw "payload value \"addends\" is not an array";

            // initial loop for vector allocation optimization
            for ( auto& v : a.GetArray() ) {
                if (v.IsInt() ) {
                    ++num_ints;
                }
            }

            // if no integers in array, throw exception
            if ( num_ints == 0 )
                throw "no integers in array"; 

            // create vector and allocate
            vector<int> int_vect;
            int_vect.reserve(num_ints);

            int sum_i = 0;

            // loop through array again, filling vector
            for (auto& v : a.GetArray()) {

                // only add int elements
                if (v.IsInt()) {  
                    int_vect.push_back(v.GetInt());
                }
            }

            if(DEBUG) 
                cerr << "Made it past for loop in the int try" << endl;

            sum_i = summation(int_vect);

            cout << sum_i << endl;

        }
        // if does not exist, throw exception
        else { 
            throw "no member \"addends\" present in payload JSON"; 
        }

        return true;
    }



    ///
    /// \brief command handler for mean_ints
    ///
    /// \todo mean_int is an almost identical function to sum_ints. May be more efficient
    ///       to break large amount of functionality out into a private member function.
    ///       Will need to figure out division of labor if so.
    ///
    /// \param payload a JSON string possibly containing an array value
    ///
    /// \return true if command handled successfully, otherwise generate exception
    ///
    /// \throws exception when payload contains no value "addends"
    /// \throws exception when addends is not an array
    /// \throws exception when no integers in array
    ///
    /// This function looks for an array named "addends". If it exists, it traverses
    /// it, and prints the mean of all integers present in the array. All non-integer
    /// members of the array are ignored.
    ///
    static bool mean_ints(rapidjson::Value &payload)
    {
        cout << "Controller::mean_ints command: \n";

        // create iterator to look for "addends".
        rapidjson::Value::ConstMemberIterator itr = payload.FindMember("addends");

        // safely check for a value "addends"
        if ( itr != payload.MemberEnd() ) {

            // value exists. check for array of numbers
            
            int num_ints = 0; // efficient vector allocation

            // reference for consecutive access
            const Value& a = payload["addends"];

            // make sure addends is an array. throw exception if not
            if ( ! a.IsArray() ) 
                throw "payload value \"addends\" is not an array";

            // initial loop for vector allocation optimization
            for ( auto& v : a.GetArray() ) {
                if ( v.IsInt() ) {
                    ++num_ints;
                }
            }

            // if no integers in array, throw exception
            if ( num_ints == 0 )
                throw "no integers in array"; 

            // create vector and allocate
            vector<int> int_vect;
            int_vect.reserve(num_ints);

            int sum_i = 0;

            // loop through array again, filling vector
            for ( auto& v : a.GetArray() ) {

                // only add int elements
                if ( v.IsInt() )  
                    int_vect.push_back(v.GetInt()); 
            }

            if(DEBUG) 
                cerr << "Made it past for loop in the int try" << endl;

            sum_i = summation(int_vect);

            /// \bug verify that this cannot convert int to float
            cout << (sum_i/num_ints) << endl; 
        }
        // if does not exist, throw exception
        else { 
            throw "no member \"addends\" present in payload JSON"; 
        }

        return true;
    }


    ///
    /// \todo write additional member functions to handle other number types such
    ///       as float, double, or Uint64.
    ///


private:
    //static std::string CUR_SCOPE = "class Controller";     // DEBUG
    
    ///
    /// \brief summation function for sum_ints() and mean_ints()
    ///
    /// \param collection a vector of numbers to add together
    ///
    /// \return sum of numbers of same type as vector passed in.
    ///
    /// This function takes in a vector of numbers and addes them together. It
    /// supports all number types in C++ including int, float, etc. It was written
    /// to accomodate future functions like sum_int and mean_int that take in
    /// other types of numbers.
    ///
    /// \todo maybe change cerr call to shared error print?
    ///
    template<typename T>
    static auto summation(vector<T> collection) -> T {

        T sum = 0; // initialize 
        
        // loop through array
        for (auto& num : collection) {

            // add current number to sum
            sum += num;
        }

        if(DEBUG) 
            cerr << sum << endl;

        return sum;
    }
    
};



// Bonus Question: why did I type cast this?
//
// typedef: To make life easier
// typecast: To pass in a rapidjson::Value& and return a bool
//           I am more or less bypassing any false returns by throwing
//           exceptions, but true returns indicate that the command
//           was successfully handled.
typedef std::function<bool(rapidjson::Value &)> CommandHandler;



class CommandDispatcher {
public:
    // ctor - need impl
    CommandDispatcher()
    {
        // My implementation uses the default constructor.
        // I suspect that a more complex constructor might be needed if the map was
        // populated using a different method.
    }


    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
        //
        // Virtual refers to members that an inhereting class will implement that the
        // superclass does not implement, or to a member that the inhereting class will 
        // overload.
        //
        // I am not sure why this class would not have an explicit destructor but one of 
        // its inhereting classes would. I am not using inhereting classes for this. If
        // I was, perhaps this class would use the implicit destructor, but the inhereting 
        // classes would need more complex ones.
        //
        // In my case, I do not believe it is needed.
    }


    ///
    /// \brief adds a command and handler pair to the map
    ///
    /// \param command a string 
    /// \param handler a CommandHandler object containing a DOM value
    ///
    /// \return true if addition successful, false if not
    ///
    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;

        bool add_succeeded = true;

        /// add command and handler pair to map
        auto test = command_handlers_.insert( std::make_pair( command, handler));

        /// check if command already added to map
        if( test.second == false) {
            cout << "Command " << command << " already existed. Addition failed" << endl;
            add_succeeded = false;
        }
        else {
            if(VERBOSE) {
                cout << "Command " << command << " added to map." << endl;
            }
        }

        return add_succeeded;
    }


    ///
    /// \brief initial receiver of command
    ///
    /// \param command_json a raw string of JSON commands
    ///
    /// \throws exception if JSON malformed
    /// \throws exception if JSON contains no member "command"
    /// \throws exception if JSON contains no member "payload"
    /// \throws exception if no match for command found in map
    ///
    /// \return true if command dispatched to handler
    ///
    /// \todo change cout calls to shared print?
    ///
    bool dispatchCommand(std::string command_json)
    {
        cout << "\n\nCOMMAND: " << command_json << endl;
        if(DEBUG) 
            debugPrinter(this->CUR_SCOPE, "made it to dispatchCommand"); 

        /// \todo maybe typcast instead?
        const char *command_ptr = command_json.c_str();  
        if(DEBUG) 
            debugPrinter(this->CUR_SCOPE, "made it past command_ptr");
        if(DEBUG) 
            debugPrinter(this->CUR_SCOPE, command_ptr);
 
        
        // parse the received string
        this->doc.Parse(command_ptr);
        if(DEBUG) 
            debugPrinter(this->CUR_SCOPE, "made it past doc.Parse");

        // check if JSON passed in. If not, throw error.
        if( this->doc.HasParseError() )  
            throw "malformed JSON"; 
        
        if(DEBUG) 
            debugPrinter(this->CUR_SCOPE, "made it past doc.HasParseError");


        // create iterators for "command" and "payload", if they exist.
        rapidjson::Value::ConstMemberIterator itr_c = this->doc.FindMember("command");
        rapidjson::Value::ConstMemberIterator itr_p = this->doc.FindMember("payload");


        // safely check for a value "command"
        if (itr_c != this->doc.MemberEnd()) {
            if(DEBUG)  
                debugPrinter(this->CUR_SCOPE, "made it into itr_c if"); 

            if(VERBOSE) {
                cout << "found command: " << itr_c->value.GetString() << endl;
            }
        }
        // if does not exist, throw exception
        else { 
            throw "no member \"command\" present in JSON"; 
        }


        // safely check for a value "payload"
        if (itr_p != this->doc.MemberEnd()) {
            if(DEBUG) 
                debugPrinter(this->CUR_SCOPE, "made it into itr_p if");

            if(VERBOSE) {
                // stringify the value
                StringBuffer buffer;
                Writer<StringBuffer> writer(buffer);
                itr_p->value.Accept(writer);

                cout << "found payload: " << buffer.GetString() << endl;
            }
        }
        // if does not exist, throw exception
        else { 
            throw "no member \"payload\" present in JSON"; 
        }


        if(DEBUG) 
            debugPrinter(this->CUR_SCOPE, "made it past iterators");

        // command routing
        bool command_found = false;  // until proven otherwise
        for( this->map_itr = command_handlers_.begin() ;
             this->map_itr != command_handlers_.end(); this->map_itr++ ) {

            // try to find match in map for command string
            if( itr_c->value.GetString() == (*map_itr).first ) {

                if(VERBOSE) {
                    cout << itr_c->value.GetString() << " matched a value in map!" << endl
                         << "Attempting to start command handler" << endl; 
                }

                command_found = true;
                
                // dispatch payload to command handler
                (*map_itr).second(this->doc["payload"]); 

                break; // don't keep looping longer than necessary
            }
        }

        // if command does not exist, throw exception
        if ( ! command_found) {
            if(DEBUG) 
                debugPrinter(this->CUR_SCOPE, "made it into ! command_found if");
            
            throw "no match for command found in map"; 
        }

        return command_found; // should always be true if execution falls through to here
    }


private:
    std::string CUR_SCOPE = "class CommandDispatcher";  // DEBUG - current scope

    std::map<std::string, CommandHandler> command_handlers_; ///< map of command handlers

    std::map<std::string, CommandHandler>::iterator map_itr = 
        this->command_handlers_.begin(); ///< iterator for the map

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
    ///
    /// \note needs static functions
    ///
    /// \todo maybe use std::bind instead to enable using non-static functions.
    ///
    command_dispatcher.addCommandHandler( "help", controller.help);
    command_dispatcher.addCommandHandler( "exit", controller.exit);
    command_dispatcher.addCommandHandler( "sum_ints", controller.sum_ints);
    command_dispatcher.addCommandHandler( "query_payload", controller.query_payload); 
    command_dispatcher.addCommandHandler( "mean_ints", controller.mean_ints); 

    // DEBUG - should generate warning on fail because "help" already exists in map
    command_dispatcher.addCommandHandler( "help", controller.help);

    
    // array of test commands
    ///
    /// \note exit_command commented out to allow fallthrough to user prompt. If uncommented,
    ///       exit_command will successfully exit program prior to user prompt.
    ///       Ok to uncomment if desired.
    ///
    /// \note The "// */" syntax is to allow easy commenting out of multiple lines while
    ///       removing the comma at the end of the last array element. See the "/*,"
    ///       at the end of the "exit_command_fail/*," line as an example.
    ///
    string test_commands[] = {

        fail_command, // */
        
        help_command,
        help_command_fail, // */

        sum_command,
        sum_command_long,
        sum_command_floats, 
        sum_command_fail_1, 
        sum_command_fail_2, // */

        mean_command,
        mean_command_fraction,
        mean_command_long, // */

        query_payload_command, // */

        exit_command_fail/*, 
        exit_command // */

    };

    
    // if set, send all test commands to command dispatcher
    if (TEST_ALL) {

        for (auto& com : test_commands) {

            //catch and handle each exception
            try { 
                command_dispatcher.dispatchCommand(com); 
            }
            catch (const char* excpt) { 
                exceptionPrinter(excpt); 
            }  
        }
    }


    // command line interface for testing
    string command;
    while( ! g_done ) {
        cout << "\n\n\n";
        cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
        cout << "\tenter command : ";
        getline(cin, command);
        try { 
            command_dispatcher.dispatchCommand(command); 
        }
        catch (const char* excpt) { 
            exceptionPrinter(excpt); 
        } 
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
