#include "main.h"

void Main::_bind_methods() {

  // Note that no parentheses should be added after the function's
  // name in either of its appearances.
  ClassDB::bind_method(D_METHOD("_on_line_edit_text_changed"),
                       &Main::_on_line_edit_text_changed);
}

Main::Main() {}

Main::~Main() {}

void Main::_ready() {

  // UtilityFunctions::print("Main::_ready() just got called.");

  // Seeding our random-number generator:

  double current_unix_time = Time::get_singleton()->get_unix_time_from_system();
  // UtilityFunctions::print("Current Unix time:", current_unix_time);
  // UtilityFunctions::print("Current Unix time as int:",
  //                       uint64_t(current_unix_time));

  // rng->set_seed(uint64_t(current_unix_time));
  // Converting Unix time to a uint64_t because this is what
  // set_seed is able to take.

  // Temp code for debugging:
  rng->set_seed(0);

  get_node<LineEdit>("Response_Window")
      ->connect("text_changed", Callable(this, "_on_line_edit_text_changed"));

  // Providing gameplay instructions:

  get_node<RichTextLabel>("Message_Window")
      ->set_text("To begin a new round of tests, click inside the gray \
response window and then press your space bar. Press q to end a round.");
}

void Main::start_test() {
  // Clearing the screen so that a leading space won't be
  // included within the player's response:
  get_node<LineEdit>("Response_Window")->clear();
  test_active = true;
  // UtilityFunctions::print("Starting function.");

  int random_int = rng->randi_range(0, 255);

  answer = godot::String::num_int64(random_int); // Corresponds to 0x0-0xFF
  // UtilityFunctions::print("Answer for this round:", answer);

  // Converting the random integer to a hexadecimal value:
  // (I stumbled upon this function after trying (and failing) to use
  // String::format to convert the integer to a hexadecimal value.
  // Note: you can add 'true' as a third argument in order to
  // capitalize hexadecimal values.
  prompt = godot::String::num_int64(random_int, 16);
  // UtilityFunctions::print(prompt);
  get_node<RichTextLabel>("Message_Window")
      ->set_text("Enter the \
decimal equivalent of the hexadecimal number " +
                 prompt + ".");

  // Now that the player can see the prompt, this will be the ideal
  // time to begin our test timer.

  start_ticks = Time::get_singleton()->get_ticks_usec();
}

void Main::end_test()

{
  test_active = false; // This value will get set back to true
  // by start_test() almost immediately.
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
  results_dict["Test_Time"] = (end_ticks - start_ticks) / 1000000.0;
  results_dict["System_Time_at_End"] =
      Time::get_singleton()->get_datetime_string_from_system();
  results_dict["Answer"] = answer;
  results_dict["Prompt"] = prompt;

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
  UtilityFunctions::print(results_array[test_index].get("Test_Number"));
  UtilityFunctions::print(results_array[test_index].get("Test_Time"));
  UtilityFunctions::print(results_array[test_index].get("System_Time_at_End"));
  UtilityFunctions::print(results_array[test_index].get("Answer"));
  UtilityFunctions::print(results_array[test_index].get("Prompt"));

  // For some reason, the following setup just prints out the
  // ASCII value corresponding to the first letter of the key
  // (e.g. 84 for 'Time').
  // UtilityFunctions::print(results_array[0]["Time"]);

  test_number++;
  // Immediately beginning a new test:
  start_test();
}

void Main::_on_line_edit_text_changed(godot::String player_response) {

// Special thanks to ElMetroid on the Godot Discord for suggesting
// the use of a signal to respond to players' entries
// rather than a while loop (which was causing the game to crash), 

  if (test_active == true) // We only want changes to the response window
  // to trigger this function when an active test is underway.
  {

    // Because the text_changed signal already sends the player's
    // latest response, the following line is unnecessary.
    // godot::String player_response =
    // get_node<LineEdit>("Response_Window")->get_text();
    // The previous (commented-out) line would also retrieve the current
    // contents of the response window:
    // (See godot-cpp/gen/include/godot_cpp/classes/text_edit.hpp)
    // UtilityFunctions::print("LineEdit text changed.");
    // UtilityFunctions::print("Value of new_text:", new_text);

    // UtilityFunctions::print("Current player response:", player_response,
    // "."); UtilityFunctions::print("Answer:", answer, ".");
    // UtilityFunctions::print("Does player response equal answer?",
    //                         player_response == answer);

    if (player_response == answer) {
      // We'll log the number of ticks here, rather than within
      // end_test(), in order to prevent any lag from
      // incorrectly lengthening the player's reported test time.
      end_ticks = Time::get_singleton()->get_ticks_usec();
      // For get_ticks_msec() and get_ticks_usec(), see 
      // godot-cpp/gen/src/classes/time.cpp .
      // UtilityFunctions::print("You win! Calling end_test().");
      end_test();
    }
  }
}

void Main::_process(double delta) {

  auto input = Input::get_singleton();

  // We won't want the start_game action to begin a new test unless
  // no test is currently active.
  if ((input->is_action_just_pressed("start_game")) && (test_active == false)) {
    // UtilityFunctions::print("Calling start_test;");
    //  start_test() will change test_active to true right away,
    //  thus preventing this line from having any effect until we
    //  close out of the game.
    start_test();
  }

  if ((input->is_action_just_pressed("end_game")) && (test_active == true)) {
    // UtilityFunctions::print("Exiting out of the current round.");
    //  start_test() will change test_active to true right away,
    //  thus preventing this line from having any effect until we
    //  close out of the game.
    get_node<LineEdit>("Response_Window")->clear(); // Clears the 'q'
    // out of the response window
    test_active = false; // Now that this value is set to false,
    // the player won't be able to complete the current game and log
    // his/her results.
    get_node<RichTextLabel>("Message_Window")
        ->set_text("Exited current round. You can save your unsaved \
results from all rounds within this session by pressing s.\nPress \
your space bar to begin a new round of tests. You can then press q \
to end that round.");
  }

  if (input->is_action_just_pressed("save_results")) {

    if (results_array.size() >= 0) // It wouldn't make sense to save
    // an empty array--and attempting to do so could actually overwrite
    // a file that we had saved less than one second ago.
    {

    // Saving a parsed copy of the current text to a local file:
    // (Based on ZenPyro's response within the Godot Discord at
    // https://discord.com/channels/212250894228652034/342047011778068481/1266192098953269369
    // )
    //
    Ref<FileAccess> text_file;
    // Adding a timestamp to the text file will prevent a newer
    // copy from overwriting an older one.
    
    // A colon-free timestamp should prove more compatible for 
    // filenames on at least one operating system.
    godot::String current_time_without_spaces = godot::String(
    Time::get_singleton()->get_datetime_string_from_system()).replace(
    ":", "");


    String filename = current_time_without_spaces + "_0xflash_results.txt";
    text_file = FileAccess::open("user://" + filename, FileAccess::WRITE);

    // Initializing a string that will be used to store our
    // results:

    godot::String results_string = "";

    // Initializing this string with headers:

    // A less error-prone alternative here would be to specify the keys
    // as an array, then iterate through the array to populate both
    // the header row and the values for each result.

    results_string +=
        ("Test_Number|Test_Time|System_Time_at_End|Prompt|Answer\n");

    for (int i = 0; i < results_array.size(); i++) {
      results_string += (godot::String::num_int64(results_array[i].get("Test_Number")) + "|" +
                         godot::String(results_array[i].get("Test_Time")) + "|" +
                         godot::String(results_array[i].get("System_Time_at_End")) +
                         "|" + godot::String(results_array[i].get("Prompt")) + "|" +
                         godot::String(results_array[i].get("Answer")) + '\n');
    }

    text_file->store_string(results_string);
    text_file->close();
    UtilityFunctions::print("Finished saving output as: " + filename);
    // On Linux Mint, these results should be available at:
    // /home/>your_username>/.local/share/godot/app_userdata/0xflash .

    // We'll clear out results_array here in order to prevent duplicate
    // results from getting saved.
    


    results_array.clear();
    UtilityFunctions::print("Cleared contents of result_array().");

  }
  }
}
