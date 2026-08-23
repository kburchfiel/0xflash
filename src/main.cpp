#include "main.h"

void Main::_bind_methods() {}

Main::Main() {}

Main::~Main() {}

void Main::_ready() {

  UtilityFunctions::print("Main::_ready() just got called.");

  // Seeding our random-number generator:

  double current_unix_time = Time::get_singleton()->get_unix_time_from_system();
  UtilityFunctions::print("Current Unix time:", current_unix_time);
  UtilityFunctions::print("Current Unix time as int:",
                          uint64_t(current_unix_time));

  rng->set_seed(uint64_t(current_unix_time));
  // Converting Unix time to a uint64_t because this is what
  // set_seed is able to take.
}

void Main::run_hex_to_int_test() {
  UtilityFunctions::print("Starting function.");

  int int_answer = rng->randi_range(0, 255); // Corresponds to 0x0-0xFF
  UtilityFunctions::print(int_answer);
  can_begin_new_test = true;

  // Converting this number to a hexadecimal value:
  // (I stumbled upon this function after trying (and failing) to use
  // String::format to convert the integer to a hexadecimal value.
  // Note: you can add 'true' as a third argument in order to
  // capitalize hexadecimal values.
  godot::String int_0x = godot::String::num_int64(int_answer, 16);
  UtilityFunctions::print(int_0x);
  get_node<RichTextLabel>("Message_Window")
      ->set_text("Enter the \
decimal equivalent of the hexadecimal number " +
                 int_0x + ".");

  while (true) {
    // Retrieving the current contents of the response window:
    // (See godot-cpp/gen/include/godot_cpp/classes/text_edit.hpp)
    String player_response = get_node<LineEdit>("Response_Window")->get_text();

    if (player_response.to_int() == int_answer) {
      UtilityFunctions::print("You win!");
      // Now that a successful response has been entered, the screen
      // should be cleared:
      get_node<LineEdit>("Response_Window")->clear();

      // The my_test.cpp file within the godot-cpp library, available at
      // https://github.com/godotengine/godot-cpp/blob/master/test/src/my_test.cpp
      // , shows how to implement a dictionary!)

      // Alternatively, you could use individual TypedDictionary objects,
      // one for each dictionary entry, to store your results.
      // The keys would be test numbers and the values would be your
      // result values (e.g. test number, time, number in Base 16
      // and Base 10, etc.)

      Dictionary results_dict{};

      results_dict["Test_Number"] = test_number;
      results_dict["Time"] =
          Time::get_singleton()->get_datetime_string_from_system();
      results_dict["B10"] = int_answer;
      results_dict["B16"] = int_0x;

      // UtilityFunctions::print(results_dict["Test_Number"]);
      // UtilityFunctions::print(results_dict["Time"]);
      // UtilityFunctions::print(results_dict["B16"]);
      // UtilityFunctions::print(results_dict["B10"]);

      int test_index = test_number - 1;

      // {{String("Time"), 500}, {String("0x"), "C7"},
      //     {String("Decimal"), 199}, {String("Timestamp"),
      //     "2026-08-20T225126"}};

      results_array.append(results_dict);

      UtilityFunctions::print("Printing array data:");
      // UtilityFunctions::print(results_array[test_index]);
      UtilityFunctions::print(results_array[test_index].get("Test_Number"));
      UtilityFunctions::print(results_array[test_index].get("Time"));
      UtilityFunctions::print(results_array[test_index].get("B16"));
      UtilityFunctions::print(results_array[test_index].get("B10"));

      // For some reason, the following setup just prints out the
      // ASCII value corresponding to the first letter of the key
      // (e.g. 84 for 'Time').
      // UtilityFunctions::print(results_array[0]["Time"]);

      test_number++;
      can_begin_new_test = true;
      return;
    }
  }
}

void Main::_process(double delta) {

  auto input = Input::get_singleton();

  if ((input->is_action_just_pressed("start_game")) &&
      (can_begin_new_test == true)) {
    UtilityFunctions::print("Calling run_hex_to_int_test;");
    can_begin_new_test = false; // Will prevent process() from
    // calling this function again until the test has been
    // completed
    run_hex_to_int_test();
  }
}
