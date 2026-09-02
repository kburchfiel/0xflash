#include "main.h"

void Main::_bind_methods() {

  // Note that no parentheses should be added after the function's
  // name in either of its appearances.
  ClassDB::bind_method(D_METHOD("_on_response_window_text_changed"),
                       &Main::_on_response_window_text_changed);

  ClassDB::bind_method(D_METHOD("_on_game_mode_item_selected"),
                       &Main::_on_game_mode_item_selected);

  ClassDB::bind_method(D_METHOD("_on_number_1_min_value_changed"),
                       &Main::_on_number_1_min_value_changed);

  ClassDB::bind_method(D_METHOD("_on_number_1_max_value_changed"),
                       &Main::_on_number_1_max_value_changed);

  ClassDB::bind_method(D_METHOD("_on_number_2_min_value_changed"),
                       &Main::_on_number_2_min_value_changed);

  ClassDB::bind_method(D_METHOD("_on_number_2_max_value_changed"),
                       &Main::_on_number_2_max_value_changed);

  ClassDB::bind_method(D_METHOD("_on_interval_value_changed"),
                       &Main::_on_interval_value_changed);

  ClassDB::bind_method(D_METHOD("_on_correct_indicator_timer_timeout"),
                       &Main::_on_correct_indicator_timer_timeout);

  ClassDB::bind_method(D_METHOD("_on_save_button_pressed"),
                       &Main::_on_save_button_pressed);
}

Main::Main() {}

Main::~Main() {}

void Main::_ready() {

  // Hiding Multiplication values, as they won't be needed for
  // the default mode (B16 to B10):
  get_node<RichTextLabel>("Num2Label")->set_visible(false);
  get_node<RichTextLabel>("Num2Separator")->set_visible(false);
  get_node<SpinBox>("Num2Min")->set_visible(false);
  get_node<SpinBox>("Num2Max")->set_visible(false);

  // Assigning the default value to our 'correct' indicator, which
  // will flash green whenever the player completes a test
  // successfully:
  get_node<ColorRect>("Correct_Indicator")->set_color(Color(0.2, 0.2, 0.2));

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

  // Connecting signals: (I've found that performing these steps here
  // is more reliable than doing so within the editor, as
  // changes made using the latter method sometimes get lost.)
  get_node<LineEdit>("Response_Window")
      ->connect("text_changed",
                Callable(this, "_on_response_window_text_changed"));

  get_node<OptionButton>("GameMode")
      ->connect("item_selected", Callable(this, "_on_game_mode_item_selected"));

  get_node<SpinBox>("Num1Min")->connect(
      "value_changed", Callable(this, "_on_number_1_min_value_changed"));

  get_node<SpinBox>("Num1Max")->connect(
      "value_changed", Callable(this, "_on_number_1_max_value_changed"));

  get_node<SpinBox>("Num2Min")->connect(
      "value_changed", Callable(this, "_on_number_2_min_value_changed"));

  get_node<SpinBox>("Num2Max")->connect(
      "value_changed", Callable(this, "_on_number_2_max_value_changed"));

  get_node<SpinBox>("Interval")
      ->connect("value_changed", Callable(this, "_on_interval_value_changed"));

  get_node<Button>("SaveButton")
      ->connect("pressed", Callable(this, "_on_save_button_pressed"));

  get_node<ColorRect>("Correct_Indicator")
      ->get_node<Timer>("Correct_Indicator_Timer")
      ->connect("timeout",
                Callable(this, "_on_correct_indicator_timer_timeout"));

  // Providing gameplay instructions:

  get_node<RichTextLabel>("Prompt_Window")
      ->set_text("To begin a new round, click the gray \
window below and then press Space. Press Q to end a round.");
}

void Main::generate_prompt_and_answer() {

  // Our method for retrieving a prompt and corresponding answer
  // will depend on the game mode.

  if (game_mode == "B16 to B10") { // The user will need to enter a base-10
                                   // number corresponding to
    // a hexadecimal number.

    int random_int = rng->randi_range(num_1_min_val, num_1_max_val) * interval;
    // The starting and ending values of randi_range do appear
    // to be inclusive. See:
    // https://www.reddit.com/r/godot/comments/z792su/can_intrand_range0_5_actually_return_5/

    // The user will need to enter a base-10 number corresponding to
    // the hexadecimal version of a base-10 number multiplied by the
    // interval value. The interval is set at 1 by default; however,
    // it can also be changed to allow users to focus on a particular
    // set of numbers.
    // For example, if interval is set as 16, the user will be tested
    // on a multiple of 16 (32, 128, 240, etc). This will prove to be
    // useful practice for calculating the decimal version of 2-digit
    // hexadecimal numbers (which will consist of a multiple of 16 plus
    // a value between 0 and 16).

    // Converting the random integer to a hexadecimal value:
    // (I stumbled upon this function after trying (and failing) to use
    // String::format to convert the integer to a hexadecimal value.
    // Note: you can add 'true' as a third argument in order to
    // capitalize hexadecimal values.
    prompt = godot::String::num_int64(random_int, 16);

    answer = godot::String::num_int64(random_int);

  }

  else if (game_mode == "B10 to B16") { // The user will need to enter a
                                        // hexadecimal number corresponding to
    // a base-16 number.

    int random_int = rng->randi_range(num_1_min_val, num_1_max_val) * interval;
    answer = godot::String::num_int64(random_int, 16);
    prompt = godot::String::num_int64(random_int);
  }

  else if (game_mode == "Multiplication") { // The user will need to enter the
                                            // product of two separate
    // numbers. (This mode is currently the only one to make use
    // of the num_2_min_val and num_2_max_val variables. It does not,
    // however, make use of the Interval variable.)

    int random_int_1 = rng->randi_range(num_1_min_val, num_1_max_val);
    int random_int_2 = rng->randi_range(num_2_min_val, num_2_max_val);

    answer = godot::String::num_int64(random_int_1 * random_int_2);
    prompt = godot::String::num_int64(random_int_1) + " x " +
             godot::String::num_int64(random_int_2);
  }
};

void Main::update_notification_window(const godot::String new_notification)
// This function adds the latest notification two line breaks above
// the existing notification string, then displays this string
// within our notification window.
{
  notification_string =
      new_notification + godot::String("\n\n") + notification_string;
  get_node<RichTextLabel>("Notification_Window")->set_text(notification_string);
}

void Main::start_round() {
  // start_round() runs actions that ony need to be performed
  // at the beginning and end of the round--thus decreasing the amount
  // of work for start_test(), which will probably run much more
  // frequently.

  // Disabling option buttons, thus preventing the user from
  // changing them (intentionally or otherwise) while tests are
  // active:

  // UtilityFunctions::print("id_game_mode_map_size", id_game_mode_map.size());

  for (int i = 0; i < id_game_mode_map.size(); i++) {
    get_node<OptionButton>("GameMode")->set_item_disabled(i, true);
  }
  get_node<SpinBox>("Num1Min")->set_editable(false);
  get_node<SpinBox>("Num1Max")->set_editable(false);
  get_node<SpinBox>("Num2Min")->set_editable(false);
  get_node<SpinBox>("Num2Max")->set_editable(false);
  get_node<SpinBox>("Interval")->set_editable(false);
  get_node<Button>("SaveButton")->set_disabled(true);

  if (print_extra_info == true) {
    UtilityFunctions::print("Contents of results_array:", results_array);
  }

  // Clearing the screen so that a leading space won't be
  // included within the player's response:
  get_node<LineEdit>("Response_Window")->clear();
  test_active = true;

  // Launching the first test of the round:
  start_test();
}

void Main::end_round() {
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

    // Making the option buttons editable again:
    for (int i = 0; i < id_game_mode_map.size(); i++) {
      get_node<OptionButton>("GameMode")->set_item_disabled(i, false);
    }
    get_node<SpinBox>("Num1Min")->set_editable(true);
    get_node<SpinBox>("Num1Max")->set_editable(true);
    get_node<SpinBox>("Num2Min")->set_editable(true);
    get_node<SpinBox>("Num2Max")->set_editable(true);
    get_node<SpinBox>("Interval")->set_editable(true);
    get_node<Button>("SaveButton")->set_disabled(false);

    get_node<RichTextLabel>("Prompt_Window")
        ->set_text("Exited current round. Press S to save your \
progress and Space to begin a new round of tests.");
        }


void Main::start_test() {
  // UtilityFunctions::print("Starting function.");

  generate_prompt_and_answer();

  if (print_extra_info == true) {
    UtilityFunctions::print("Answer for this round:", answer);
  }

  get_node<RichTextLabel>("Prompt_Window")->set_text(
  test_intro + prompt + ".");

  // Now that the player can see the prompt, this will be the ideal
  // time to begin our test stopwatch.

  start_ticks = Time::get_singleton()->get_ticks_usec();
}

void Main::end_test()

{
  if (print_extra_info == true) {
    UtilityFunctions::print("started end_test().");
  }
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

  results_dict["Test_Number"] =
      godot::String::num_int64(within_round_test_number);
  godot::String test_time =
      godot::String::num((end_ticks - start_ticks) / 1000000.0);
  results_dict["Test_Time"] = test_time;
  godot::String system_time_at_end =
      godot::String(Time::get_singleton()->get_datetime_string_from_system());
  results_dict["System_Time_at_End"] = system_time_at_end;

  results_dict["Prompt"] = prompt;
  results_dict["Answer"] = answer;
  results_dict["Game_Mode"] = game_mode;
  results_dict["Num_1_Min"] = num_1_min_val;
  results_dict["Num_1_Max"] = num_1_max_val;
  results_dict["Num_2_Min"] = num_2_min_val;
  results_dict["Num_2_Max"] = num_2_max_val;
  results_dict["Interval"] = interval;

  // Displaying this result within the notification window:
  update_notification_window(system_time_at_end + ": Entered answer (" +
                             answer + ") to " + game_mode + " prompt (" +
                             prompt + ") in " + test_time + "s.");

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
  // results_array.clear();

  // Immediately beginning a new test:
  start_test();
}

void Main::_on_game_mode_item_selected(const int mode_id) {
  if (test_active == false) // It will be best to apply any changes
  // to this mode outside of an active test.
  {
    game_mode = id_game_mode_map[mode_id];
    test_intro = id_test_intro_map[mode_id];
    UtilityFunctions::print("game_mode: ", game_mode);
    UtilityFunctions::print("test_intro: ", test_intro);

    if (game_mode == "Multiplication")

    // Certain elements are only relevant for either the Multiplication
    // mode or the other mode, so we'll set their visibility
    // accordingly.
    // (If we had wanted to keep all objects visible, we could have used
    // set_editable rather than set_visible.
    // See
    // https://docs.godotengine.org/en/4.5/classes/class_canvasitem.html#class-canvasitem-property-visible
    // )
    {
      get_node<SpinBox>("Num2Min")->set_visible(true);
      get_node<RichTextLabel>("Num2Label")->set_visible(true);
      get_node<RichTextLabel>("Num2Separator")->set_visible(true);
      get_node<SpinBox>("Num2Max")->set_visible(true);
      get_node<RichTextLabel>("IntervalLabel")->set_visible(false);
      get_node<SpinBox>("Interval")->set_visible(false);

      // Num1Label will show a different set of text depending on which
      // mode is shown.
      get_node<RichTextLabel>("Num1Label")->set_text("Number 1 Range");
    }

    else {
      get_node<RichTextLabel>("Num2Label")->set_visible(false);
      get_node<RichTextLabel>("Num2Separator")->set_visible(false);
      get_node<SpinBox>("Num2Min")->set_visible(false);
      get_node<SpinBox>("Num2Max")->set_visible(false);
      get_node<RichTextLabel>("IntervalLabel")->set_visible(true);
      get_node<SpinBox>("Interval")->set_visible(true);

      get_node<RichTextLabel>("Num1Label")->set_text("Number Range \
(in base 10)");
    }
  }
}

void Main::_on_number_1_min_value_changed(const float value) {
  if (test_active == false) {
    num_1_min_val = int(value);
    UtilityFunctions::print("num_1_min_val: ", num_1_min_val);
  }
}

void Main::_on_number_1_max_value_changed(const float value) {
  if (test_active == false) {
    num_1_max_val = int(value);
    UtilityFunctions::print("num_1_max_val: ", num_1_max_val);
  }
}

void Main::_on_number_2_min_value_changed(const float value) {
  if (test_active == false) {
    num_2_min_val = int(value);
    UtilityFunctions::print("num_2_min_val: ", num_2_min_val);
  }
}

void Main::_on_number_2_max_value_changed(const float value) {
  if (test_active == false) {
    num_2_max_val = int(value);
    UtilityFunctions::print("num_2_max_val: ", num_2_max_val);
  }
}

void Main::_on_interval_value_changed(const float value) {
  if (test_active == false) {
    interval = int(value);
    UtilityFunctions::print("interval: ", interval);
  }
}

void Main::_on_correct_indicator_timer_timeout() {
  // This function sets our 'correct' indicator back to its default
  // value.

  get_node<ColorRect>("Correct_Indicator")->set_color(Color(0.2, 0.2, 0.2));
}

void Main::_on_response_window_text_changed(
    const godot::String player_response) {

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

      UtilityFunctions::print("Current player response:", player_response, ".");
      UtilityFunctions::print("Answer:", answer, ".");
      UtilityFunctions::print(
          "Does player response (in lowercase) equal answer?",
          player_response.to_lower() == answer);
    }

    // Hexadecimal answers will always be in lowercase format,
    // so just in case the player enters a response with capitalized
    // letters, we'll need to convert it to lowercase beorehand.
    if (player_response.to_lower() == answer) {
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

      get_node<ColorRect>("Correct_Indicator")->set_color(Color(0, 1, 0));
      // We'll only want this 'correct' color to appear for a brief
      // period of time, so we'll use the following line to launch
      // a timer that will quickly reset its value.
      get_node<ColorRect>("Correct_Indicator")
          ->get_node<Timer>("Correct_Indicator_Timer")
          ->start();

      if (print_extra_info == true) {
        UtilityFunctions::print("Calling end_test().");
      }
      end_test();
    }
  }
}

void Main::_on_save_button_pressed()
// The game can also be saved by pressing S, but it's nice to have
// a button available for this task as well.
{
  save_results();
}

void Main::save_results() {

    get_node<LineEdit>("Response_Window")->clear(); // Clears the 's'
    // out of the response window

  // It wouldn't make sense to save
  // an empty array--and attempting to do so could actually overwrite
  // a file that we had saved less than one second ago.
  if (results_array.size() == 0) {

    update_notification_window("You don't have any unsaved \
results at the moment.");

    return;
  }


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

    // Building a header row: (We'll also iterate through this
    // array for each results_array entry to add the corresponding
    // values for each header item into our .csv file.)
    PackedStringArray header_row{
        "Test_Number", "Test_Time", "System_Time_at_End",
        "Prompt",      "Answer",    "Game_Mode",
        "Num_1_Min",   "Num_1_Max", "Num_2_Min",
        "Num_2_Max",   "Interval"};

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
    // overwriting any of its previous data. See:
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

    if (csv_first_row != header_row)

    {
      update_notification_window("ERROR: headers within 0xflash_results.csv \
don't match those within the game's code. Correct the headers as needed, or \
rename the current results file so that a new one can get created in  \
its place.");
      return;
    }

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
    update_notification_window("Finished saving output as: " + filename);
    // On Linux Mint, these results should be available at:
    // /home/>your_username>/.local/share/godot/app_userdata/0xflash .

    // We'll clear out results_array here in order to prevent duplicate
    // results from getting saved.

    results_array.clear();
    if (print_extra_info == true) {
      UtilityFunctions::print("Cleared contents of result_array().");
    }
  }

void Main::_process(double delta) {

  auto input = Input::get_singleton();

  // We won't want the start_game action to begin a new test unless
  // no test is currently active.
  if ((input->is_action_just_pressed("start_game")) && (test_active == false)) {
    if (print_extra_info == true) {
      UtilityFunctions::print("Calling start_round.");
    }
    //  start_round() will change test_active to true right away,
    //  thus preventing this line from having any effect until we
    //  close out of the round.
    start_round();
  }

  if ((input->is_action_just_pressed("end_game")) && (test_active == true)) {
    end_round();
  }

  if ((input->is_action_just_pressed("save_results")) &&
      (test_active == false)) {
    save_results();

    if (print_extra_info == true) {
      UtilityFunctions::print("Contents of results_array:", results_array);
    }
  }
}
