# 0XFlash: A Godot-based game for practicing your decimal-to-hexadecimal skills

[Work in progress]

By Ken Burchfiel

Released under the MIT License

(Note: As with my other repositories at https://github.com/kburchfiel/, I am not using generative-AI tools to create this project.)


## To-dos (incomplete list)

0. Try out an alternative to the Array-of-Dictionaries setup that uses a std::vector of std::map entries with std::string keys and std::variant<int, std::string> values. (Though the dictionary setup will still work fine.) 
0.5. Find a way to save your dictionary entries in .csv format (or, at the very least, in a pipe-separated format, which might be a bit easier to process). (The best option might be to append these entries to a giant string (with pipe separators in between), then save that string as a .csv file.
1. Find a way to randomly produce numbers from 10-255 in hexadecimal format (e.g. A-FF), then display them within Message_Window. (Have the program create a new number (1) at the start of a round and (2) when the player has entered one correctly.)
2. Find a way to time how long it takes for the player to enter a certain number of hexadecimal values (maybe 10, 20, etc.), then store that value--ideally in .csv format so that the output can easily be exported. (Eventually, you could even find a way to store all of the results within a single log that then gets updated periodically.)
3. Also consider keeping track of how long it took the player to enter each actual hexadecimal code. You could then see how well you progress on this front over time.
4. Add buttons, menus, etc. that let the player (1) choose whether to convert from hexadecimal to decimal or the other way around; (2) choose the minimum and maximum decimal-number range (so that, if the A-FF range gets too easy, the player can go up to, say, FFF); and (3) determine how many numbers to enter for each round.

