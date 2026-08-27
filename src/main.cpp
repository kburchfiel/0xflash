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

  if (print_extra_info == true) {
    UtilityFunctions::print("Main::_ready() just got called.");
  }

  // Seeding our random-number generator:

  double current_unix_time = Time::get_singleton()->get_unix_time_from_system();
  if (print_extra_info == true) {
    UtilityFunctions::print("Current Unix time:", current_unix_time);
    UtilityFunctions::print("Current Unix time as int:",
                            uint64_t(current_unix_time));
  }

  // Using the current unix time as a seed should allow for
  // decently-random prompts.
  rng->set_seed(uint64_t(current_unix_time));
  // Converting Unix time to a uint64_t because this is what
  // set_seed is able to take.

  // Temp code for debugging:
  // rng->set_seed(0);

  get_node<LineEdit>("Response_Window")
      ->connect("text_changed", Callable(this, "_on_line_edit_text_changed"));

  // Providing gameplay instructions:

  get_node<RichTextLabel>("Message_Window")
      ->set_text("To begin a new round of tests, click inside the gray \
response window and then press your space bar. Press q to end a round.");
}

void Main::start_test() {

if (print_extra_info == true) {
    UtilityFunctions::print(
    "Contents of results_array:", results_array);
    }


  // Clearing the screen so that a leading space won't be
  // included within the player's response:
  get_node<LineEdit>("Response_Window")->clear();
  test_active = true;
  // UtilityFunctions::print("Starting function.");

  int random_int = rng->randi_range(0, 255);

  answer = godot::String::num_int64(random_int); // Corresponds to 0x0-0xFF
  if (print_extra_info == true) {
    UtilityFunctions::print("Answer for this round:", answer);
  }
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
  if (print_extra_info == true) {
  UtilityFunctions::print("started end_test().");
  }
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

  // Saving each item as a string will make it easier to add them
  // to a PackedStringArray later on via a for loop.

  // Note that num_int64() is used to convert integers to strings,
  // whereas num() is used to convert doubles to strings.
  // (See string.hpp for more details.)

  results_dict["Test_Number"] = godot::String::num_int64(within_round_test_number);
  results_dict["Test_Time"] =
      godot::String::num((end_ticks - start_ticks) / 1000000.0);
  results_dict["System_Time_at_End"] =
      godot::String(Time::get_singleton()->get_datetime_string_from_system());
  results_dict["Answer"] = answer;
  results_dict["Prompt"] = prompt;

  if (print_extra_info == true) {

    UtilityFunctions::print("Test_Number:", results_dict["Test_Number"]);
    UtilityFunctions::print("Test_Time:", results_dict["Test_Time"]);
    UtilityFunctions::print("Prompt:", results_dict["Prompt"]);
    UtilityFunctions::print("Answer:", results_dict["Answer"]);
  }

  results_array.append(results_dict);

  // Using -1 here gets us the last value within the array. See
  // https://docs.godotengine.org/en/stable/classes/class_array.html#description
  if (print_extra_info == true) {
    UtilityFunctions::print("Printing array data for most recent test:");

    // Calculating the last index of the array (i.e. the one
    // corresponding to the most recent test):
    // Note: I tried using -1 for this task (as shown within
    // https://docs.godotengine.org/en/stable/classes/class_array.html#description),
    // but this caused the game to crash for some reason.
    int array_index = results_array.size() - 1;

    UtilityFunctions::print(results_array[array_index].get("Test_Number"));

    UtilityFunctions::print(results_array[array_index].get("Test_Time"));
    UtilityFunctions::print(
        results_array[array_index].get("System_Time_at_End"));
    UtilityFunctions::print(results_array[array_index].get("Answer"));
    UtilityFunctions::print(results_array[array_index].get("Prompt"));
  }
  // For some reason, the following setup just prints out the
  // ASCII value corresponding to the first letter of the key
  // (e.g. 84 for 'Time').
  // UtilityFunctions::print(results_array[0]["Time"]);

  within_round_test_number++;

  // TEMP--seeing whether this causes a crash when the next test
  // finishes
  //results_array.clear();

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
          if (print_extra_info == true) {
       
    UtilityFunctions::print("LineEdit text changed.");

    UtilityFunctions::print("Current player response:", player_response,
    "."); UtilityFunctions::print("Answer:", answer, ".");
    UtilityFunctions::print("Does player response equal answer?",
     player_response == answer);}

    if (player_response == answer) {
      if (print_extra_info == true) {
        UtilityFunctions::print("Player got the correct answer.");
      }
      // We'll log the number of ticks here, rather than within
      // end_test(), in order to prevent any lag from
      // incorrectly lengthening the player's reported test time.
      end_ticks = Time::get_singleton()->get_ticks_usec();
      // For get_ticks_msec() and get_ticks_usec(), see
      // godot-cpp/gen/src/classes/time.cpp .
      // UtilityFunctions::print("You win! Calling end_test().");
      if (print_extra_info == true) {
        UtilityFunctions::print("Calling end_test().");
      }
      end_test();
    }
  }
}

void Main::save_results() {

  if (results_array.size() >=
      0) // It wouldn't make sense to save
         // an empty array--and attempting to do so could actually overwrite
         // a file that we had saved less than one second ago.
  {

    // Saving a parsed copy of the current text to a local file:
    // (Based on ZenPyro's response within the Godot Discord at
    // https://discord.com/channels/212250894228652034/342047011778068481/1266192098953269369
    // )
    //
    Ref<FileAccess> results_csv_file;

    // The simplest option (which I originally tested out) would be
    // to create a new file for each set of saved data. (Timestamps
    // would be used to differentiate these files.) However,
    // this would cause the data folder to get cluttered and also
    // make analyses a bit trickier, since you'd first need to 
    // add all of the separate data files together. 
    // Therefore, I'm now saving results to the same .csv file; 
    // if this file already exists, the results will get appended
    // (provided the headers within the file match what the code
    // expects); if it doesn't yet exist, it will get initialized
    // with a standard set of headers.
    
    // Adding a timestamp to the text file will prevent a newer
    // copy from overwriting an older one.

    // A colon-free timestamp should prove more compatible for
    // filenames on at least one operating system.
    // godot::String current_time_without_spaces =
    //     godot::String(Time::get_singleton()->get_datetime_string_from_system())
    //         .replace(":", "");

    // String filename = current_time_without_spaces + "_0xflash_results.csv";
    String filename = "user://0xflash_results.csv";

    // Building a header row:
    PackedStringArray header_row{"Test_Number", "Test_Time",
                                 "System_Time_at_End", "Prompt", "Answer"};

    // Checking whether this file already exists: (If not,
    // we'll create it *and* store our header row as its first
    // line.)

    if (FileAccess::file_exists(filename) == false) {
      Ref<FileAccess> new_csv_file =
          FileAccess::open(filename, FileAccess::WRITE);
      new_csv_file->store_csv_line(header_row);
      new_csv_file->close();
    }

    // Now that we know this file exists, we can go ahead and 
    // open it for further edits:
    results_csv_file = FileAccess::open(filename, FileAccess::READ_WRITE);
    // read_write will allow us to add new data to a file without
    // overwriting any of its previous data. (This file will get
    // created if it doesn't yet exist. See:
    // https://docs.godotengine.org/en/stable/classes/class_fileaccess.html#class-fileaccess-method-seek-end

    // Checking the first line of this file to make sure that
    // it has the exact same headers (in the exact same order)
    // as header_row. (If it doesn't, we shouldn't save the results,
    // since we'd then be storing invalid data for one or more
    // header fields.)

    PackedStringArray csv_first_row = results_csv_file->get_csv_line();

    UtilityFunctions::print("csv_first_row", csv_first_row);
    UtilityFunctions::print("csv_first_row equals header_row:",
                            csv_first_row == header_row);

    if (csv_first_row == header_row)
    {

    // Moving to the end of the file so as not to overwrite any
    // existing results:
    results_csv_file->seek_end();

    // To do: execute the following line if csv_first_row is empty
    // (i.e. there's no header file already present).
    // results_csv_file -> store_csv_line(header_row);

    // Iterating through each set of results:
    for (int i = 0; i < results_array.size(); i++)

    {
      // Adding the values for this row, field by field, to an
      // array that will then get stored as another .csv line:

      PackedStringArray data_row{};

      for (int j = 0; j < header_row.size(); j++) {
        data_row.append(results_array[i].get(header_row[j]));
      }

      // Now that all fields for the data row have been added,
      // this field, too can get saved as a new row within our
      // .csv file.

      results_csv_file->store_csv_line(data_row);
    }

    results_csv_file->close();
    UtilityFunctions::print("Finished saving output as: " + filename);
    // On Linux Mint, these results should be available at:
    // /home/>your_username>/.local/share/godot/app_userdata/0xflash .

    // We'll clear out results_array here in order to prevent duplicate
    // results from getting saved.

    results_array.clear();
    if (print_extra_info == true) {
      UtilityFunctions::print("Cleared contents of result_array().");
    }
  }
  else
  {
  UtilityFunctions::print("ERROR: headers within .csv file don't match \
those within the game's code. Correct the headers as needed, or \
rename the current results file so that a new one can get created in  \
its place.");
  }
}
}

void Main::_process(double delta) {

  auto input = Input::get_singleton();

  // We won't want the start_game action to begin a new test unless
  // no test is currently active.
  if ((input->is_action_just_pressed("start_game")) && (test_active == false)) {
    if (print_extra_info == true) {
      UtilityFunctions::print("Calling start_test;");
    }
    //  start_test() will change test_active to true right away,
    //  thus preventing this line from having any effect until we
    //  close out of the game.
    start_test();
  }

  if ((input->is_action_just_pressed("end_game")) && (test_active == true)) {
    if (print_extra_info == true) {
      UtilityFunctions::print("Exiting out of the current round.");
    }
    //  start_test() will change test_active to true right away,
    //  thus preventing this line from having any effect until we
    //  close out of the game.
    get_node<LineEdit>("Response_Window")->clear(); // Clears the 'q'
    // out of the response window
    test_active = false; // Now that this value is set to false,
    // the player won't be able to complete the current game and log
    // his/her results.
    // Resetting the test_number value (since we've now exited
    // out of the previous round):
    within_round_test_number = 1;
    get_node<RichTextLabel>("Message_Window")
        ->set_text("Exited current round. You can save your unsaved \
results from all rounds within this session by pressing s.\nPress \
your space bar to begin a new round of tests. You can then press q \
to end that round.");
  }

  if ((input->is_action_just_pressed("save_results")) && (
  test_active == false)) 
  {
    get_node<LineEdit>("Response_Window")->clear(); // Clears the 's'
    // out of the response window
    save_results();

if (print_extra_info == true) {
    UtilityFunctions::print(
    "Contents of results_array:", results_array);
    }

  }

    

}
