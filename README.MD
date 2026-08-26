# 0XFlash: A Godot-based game for practicing your decimal-to-hexadecimal skills

[Work in progress]

By Ken Burchfiel

Released under the MIT License

(Note: As with my other repositories at https://github.com/kburchfiel/, I am not using generative-AI tools to create this project.)


## To-dos (incomplete list)

1. Find a way to save your dictionary entries in .csv format rather than the current pipe-separated format.
* It looks like the FileAccess::store_csv_line() function within file_access.cpp will be a good option here. 

1. Add buttons, menus, etc. that let the player (1) choose whether to convert from hexadecimal to decimal or the other way around; (2) choose the minimum and maximum decimal-number range (so that, if the A-FF range gets too easy, the player can go up to, say, FFF); and (3) determine how many numbers to enter for each round. 

1. Consider expanding the game to include certain other math-related tests, such as single-digit x double-digit multiplication.

1. Figure out which additional variables can be made `const`.