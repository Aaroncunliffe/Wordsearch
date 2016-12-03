# Wordsearch (Assignment)

This program was a university assignment for completing a wordsearch (limited to 1 source file).
Given 2 text files, one the wordsearch grid and the other a list of words to search for.
Constants for ROWLENGTH and COLUMNDEPTH can be found in the file, defaults are ROWLENGTH = 14, COLUMNDEPTH = 4.

This program searches left, right, up, down, and downwards diagonals (to meet spec, code can be uncommented for upward diagonals).
replaces all found letters with a constant character (default is '.')
Outputs results to console and to a text file, also lists the coordinates of each found word.

Orthogonal search: 
Essentially pathfinding, the program recursively searches for the letter all around the current position,
if it finds a match it jumps to that position and searches all around for the next letter. until the full word is found
or no letter around the current position matches what it is looking for.

Note: Over commented.
