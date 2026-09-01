#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/option_button.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/button.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>


using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

private:

bool test_active = false;

bool print_extra_info = false; // Will allow a number of
// UtilityFunction::print() calls to execute, thus making it easier
// to debug issues.

godot::String prompt;

godot::String notification_string;

godot::String answer; // Setting this as a string makes it more
// flexible, as certain test types (such as int-to-hex questions)
// will need to accept string-based answers.

// We'll use an array of Dictionaries of variants to store
// each test's results. This should make it relatively straightforward
// to export our results to a standalone .csv file.

Array results_array {};
int within_round_test_number = 1; // Will get incremented by 1 after each test.

// Creating a new ref-counted RandomNumberGenerator object:
// (Based on https://docs.godotengine.org/en/stable/engine_details/architecture/object_class.html#refcounted-memory-management )
Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);

// The following two values will allow us to determine the amount
// of time, in millionths of a second, that the player needed
// to answer a question. (Dividing the difference between these two
// values by 1 million will allow us to figure out the number
// of *seconds* needed to answer the question.)
uint64_t start_ticks = 0;
uint64_t end_ticks = 0;

// Game configuration settings:
// The default mode is B16 to B10, which has a corresponding ID
// of 0.
godot::String game_mode = "B16 to B10";
godot::String test_intro = "Enter the decimal equivalent of the \
hexadecimal number ";

// The following indices and game modes should match what has been
// configured within the editor.
const TypedDictionary<int, godot::String> id_game_mode_map {
{0, "B16 to B10"},
{1, "B10 to B16"},
{2, "Multiplication"}
};


const TypedDictionary<int, godot::String> id_test_intro_map {
{0, "Enter the decimal equivalent of the hexadecimal number "},
{1, "Enter the hexadecimal equivalent of the decimal number "},
{2, "Enter the product of "}
};

int num_1_min_val = 0;
int num_1_max_val = 255;
int num_2_min_val = 0;
int num_2_max_val = 10;
int interval = 1;


protected:

static void _bind_methods();

public:
Main();
~Main();

void _ready();

void _process(double delta) override;

void start_test(); // Keeping this name generic so that it can
// eventually (with some modifications) to be used to support
// multiple test types (e.g. hex to int, int to hex, multiplication
// calculations, etc.)

void _on_response_window_text_changed(const godot::String player_response);
// Based on: https://docs.godotengine.org/en/stable/classes/class_lineedit.html#class-lineedit-signal-text-changed
// (Note that, because the text_changed() signal emits a value
// (e.g. the latest version of the text within the LineEdit field),
// we'll need to add an argument to this function in order to
// accept that new text.

void _on_game_mode_item_selected(const int mode_id);

void _on_number_1_min_value_changed(const float value);

void _on_number_1_max_value_changed(const float value);

void _on_number_2_min_value_changed(const float value);

void _on_number_2_max_value_changed(const float value);

void _on_interval_value_changed(const float value);

void _on_save_button_pressed();

void _on_correct_indicator_timer_timeout();

void update_notification_window(
    const godot::String new_notification);

void end_test();

void save_results();

void generate_prompt_and_answer();

};