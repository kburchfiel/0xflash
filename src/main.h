#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

private:

// We'll use an array of Dictionaries of variants to store
// each test's results. This should make it relatively straightforward
// to export our results to a standalone file.

Array results_array {};
int test_number = 1; // Will get incremented by 1 after each test.


protected:

static void _bind_methods();

public:
Main();
~Main();

void _ready();

void _process(double delta) override;

};