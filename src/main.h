#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>

using namespace godot;

class Main: public Node {GDCLASS(Main, Node)

protected:

static void _bind_methods();

public:
Main();
~Main();

void _ready();

void _process(double delta) override;

};