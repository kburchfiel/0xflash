#include "main.h"

void Main::_bind_methods() {}

Main::Main() {}

Main::~Main() {}

void Main::_ready() {

UtilityFunctions::print("Main::_ready() just got called.");
get_node<RichTextLabel>("Message_Window")->set_text("Enter the \
decimal equivalent of the hexadecimal number C7.");
}

void Main::_process(double delta) 
{
// Retrieving the current contents of the response window:
// (See godot-cpp/gen/include/godot_cpp/classes/text_edit.hpp)
String player_response = get_node<TextEdit>("Response_Window")->get_text();
if (player_response == "199")
{UtilityFunctions::print("You win!");
// Now that a successful response has been entered, the screen
// should be cleared:
get_node<TextEdit>("Response_Window")->clear();

// Storing these results within a dictionary:
// Another approach would be 

// Note: Initializing this dictionary and adding items to it took
// a LOT of trial and error! (This was before I realized that
// the my_test.cpp file within the godot-cpp library, available at
// https://github.com/godotengine/godot-cpp/blob/master/test/src/my_test.cpp ,
// shows how to implement a dictionary!)

// Alternatively, you could use individual TypedDictionary objects,
// one for each dictionary entry, to store your results.
// The keys would be test numbers and the values would be your
// result values (e.g. test number, time, number in Base 16 
// and Base 10, etc.)

// Or, you could even try using a std::vector of std::map entries with
// std::string keys and std::variant<int, string, etc.> values
// to store your results.

// Dictionary results_dict {};

// The following two approaches work, but they're unnecessarily
// complex:
// results_dict[String("Test_Number")] = Variant(test_number);
// results_dict[String("Time")] = Variant(500);
// results_dict[String("B16")] = Variant("C7");
// results_dict[String("B10")] = Variant("199");

// results_dict[String("Test_Number")] = test_number;
// results_dict[String("Time")] = 500;
// results_dict[String("B16")] = "C7";
// results_dict[String("B10")] = "199";

Dictionary results_dict {};

results_dict["Test_Number"] = test_number;
results_dict["Time"] = 500;
results_dict["B16"] = "C7";
results_dict["B10"] = "199";

Dictionary results_dict{{"Test_Number", test_number}, 
{"Time", 500}, {"B16", "C7"}, {"B10", "199"}};

// UtilityFunctions::print(results_dict["Test_Number"]);
// UtilityFunctions::print(results_dict["Time"]);
// UtilityFunctions::print(results_dict["B16"]);
// UtilityFunctions::print(results_dict["B10"]);

int test_index = test_number -1;

// {{String("Time"), 500}, {String("0x"), "C7"},
//     {String("Decimal"), 199}, {String("Timestamp"), "2026-08-20T225126"}};

results_array.append(results_dict);

UtilityFunctions::print("Printing array data:");
//UtilityFunctions::print(results_array[test_index]);
UtilityFunctions::print(results_array[test_index].get("Test_Number"));
UtilityFunctions::print(results_array[test_index].get("Time"));
UtilityFunctions::print(results_array[test_index].get("B16"));
UtilityFunctions::print(results_array[test_index].get("B10"));

// For some reason, the following setup just prints out the
// ASCII value corresponding to the first letter of the key 
// (e.g. 84 for 'Time').
// UtilityFunctions::print(results_array[0]["Time"]);

test_number++;

}
}