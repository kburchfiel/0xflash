# 0XFlash: A Godot-based game for practicing your decimal-to-hexadecimal skills

[Work in progress]

By Ken Burchfiel

Released under the MIT License

(Note: As with my other repositories at https://github.com/kburchfiel/, I am not using generative-AI tools to create this project.)


## To-dos (incomplete list)

1. Find a way to save your dictionary entries in .csv format (or, at the very least, in a pipe-separated format, which might be a bit easier to process). 
* It looks like the FileAccess::store_csv_line() function within file_access.cpp will be a good option here. (And you can use the FileAccess code within your stop-start tracker project to help implement this setup.)

1. Find a way to randomly produce numbers from 10-255 in hexadecimal format (e.g. A-FF), then display them within Message_Window. (Have the program create a new number (1) at the start of a round and (2) when the player has entered one correctly.) 
* For this, try RandomNumberGenerator::randi_range() within random_number_generator.cpp--and consider using the current time (or, perhaps, the number of microseconds within the current system time) to set the seed. (You should be able to set the seed via RandomNumberGenerator::set_seed(uint64_t p_seed) .)
* To convert values to hexadecimal format, try std::format. (See https://stackoverflow.com/a/69326849/13097194)

1. Determine how long it took the player to enter each actual hexadecimal code. You could then see how well you progress on this front over time.

* This paragraph from the Godot documentation seems relevant:

"Important: The _from_system methods use the system clock that the user can manually set. Never use this method for precise time calculation since its results are subject to automatic adjustments by the user or the operating system. Always use get_ticks_usec() or get_ticks_msec() for precise time calculation instead, since they are guaranteed to be monotonic (i.e. never decrease)."

So, depending on how precise you need your duration to be, either get_ticks_usec() or get_ticks_msec() should be a good option. For instance, you could store the output of get_ticks_usec() as start_time; store the output of this function later on as end_time; then calculate the duration, in seconds, as (end_time - start_time)/1000000 . (Dividing by 1 million will convert microseconds to seconds.)

(For get_ticks_msec() and get_ticks_usec(), see godot-cpp/gen/src/classes/time.cpp)

* You could probably also use the <chrono> options within the C++ STL for this, though since you're creating a Godot game you may as well use the function that Godot provides.

1. Add buttons, menus, etc. that let the player (1) choose whether to convert from hexadecimal to decimal or the other way around; (2) choose the minimum and maximum decimal-number range (so that, if the A-FF range gets too easy, the player can go up to, say, FFF); and (3) determine how many numbers to enter for each round.

