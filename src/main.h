#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>




using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

private:

bool test_active = false;

bool print_extra_info = false; // Will allow a number of
// UtilityFunction::print() calls to execute, thus making it easier
// to debug issues.

godot::String prompt;

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

void _on_line_edit_text_changed(godot::String player_response);
// Based on: https://docs.godotengine.org/en/stable/classes/class_lineedit.html#class-lineedit-signal-text-changed
// (Note that, because the text_changed() signal emits a value
// (e.g. the latest version of the text within the LineEdit field),
// we'll need to add an argument to this function in order to
// accept that new text.

void end_test();

void save_results();

};