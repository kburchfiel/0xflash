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




using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

private:

// We'll use an array of Dictionaries of variants to store
// each test's results. This should make it relatively straightforward
// to export our results to a standalone file.

bool can_begin_new_test = true;

Array results_array {};
int test_number = 1; // Will get incremented by 1 after each test.

// Creating a new ref-counted RandomNumberGenerator object:
// (Based on https://docs.godotengine.org/en/stable/engine_details/architecture/object_class.html#refcounted-memory-management )
Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);

protected:

static void _bind_methods();

public:
Main();
~Main();

void _ready();

void _process(double delta) override;

void run_hex_to_int_test();

};