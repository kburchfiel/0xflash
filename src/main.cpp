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
}
}