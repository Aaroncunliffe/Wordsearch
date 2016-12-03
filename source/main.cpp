// -------- README --------- //
//  able to search words of any length in both normal and orthogonal searches (orthogonal is not fixed pattern but akin to a pathfinding algorithm)
//  dynamically searches for any length word but the max is set by a constant
//  number of lines and line length of the search grid is set by the constants (but can be expanded)
//  has the functionality for upward diagonals too but is disabled incase any words are found during the demonstration that aren't in the mark scheme

//Includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//Constant varaibles for grid size and search term length
const int ROWLENGTH = 14;
const int COLUMNDEPTH = 4;
const int MAXWORDLENGTH = 15;

struct coordinates
{
    int X;
    int Y;
};

// A struct for each search term loaded from the file 
struct term
{
    string word;
    bool found;
    coordinates directionCoords[MAXWORDLENGTH]; //Copes with up to 10 letters
};

//Forward Declaration
void FullSearch(string puzzleFile, string wordFile, string outputFileName);
void ResetGlobalArrays();
void OpenFile(ifstream &file, string directory);
void LoadSearchTerms(ifstream &file, vector<term> &searchTerms, int &count);
void LoadPuzzleFile(ifstream &file);
void SearchGrid(vector<term> &terms, int numOfTerms);
bool Search(int row, int col, int termIndex);
bool OrthogonalSearch(int row, int col, int termIndex);
bool furtherSearch(int row, int col, int termIndex, int letter);

//Directions for orthogonal searching
const int orthX[4] = { -1, 0, 0, 1 };
const int orthY[4] = { 0,-1, 1, 0 };

//standard searching directions
const int x[6] = { -1,-1, 0, 0, 1, 1 };
const int y[6] = { 0, 1,-1, 1, 0, 1 };

// To be used for upwards diagonals too
//const int x[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
//const int y[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

// constants for number of directions
const int numOfSearchDirections = 6;
const int numOfOrthDirections = 4;
const char replaceChar = '.';

// Storage
char puzzleGrid[ROWLENGTH][COLUMNDEPTH]; //2D array that contains the grid loaded from the text file
char solvedGrid[ROWLENGTH][COLUMNDEPTH]; //2D array that will have the characters replaced and it will be written to the file
vector<term> searchTerms; // A vector of type Term that will be used to store the search terms.


int main()
{
    //          Grid to search         Words to search          Name of output file
    FullSearch(".\\files\\text1.txt", ".\\files\\search1.txt", ".\\files\\Text1Filtered.txt"); // File that contains the grid, File containing the words, File name to output too 

    // Arrays must be reset between each file search call
    ResetGlobalArrays();

    FullSearch(".\\files\\text2D.txt", ".\\files\\search2D.txt", ".\\files\\Text2DFiltered.txt");

    //FullSearch("Wordsfile", "gridfile", "outputfile"); <--- Syntax for full search (normal, orthogonal)

    cout << endl;
    system("pause");
    return 0;
}

void FullSearch(string puzzleFileName, string wordFileName, string outputFileName)
{

    ifstream puzzleFile; // File containing the grid
    ifstream searchFile; // File containing the words
    ofstream outFile;	 // File to output the results to
    outFile.open(outputFileName);

    int numberOfTerms = 0;

    //Opens files
    OpenFile(puzzleFile, puzzleFileName);
    OpenFile(searchFile, wordFileName);


    LoadSearchTerms(searchFile, searchTerms, numberOfTerms); // Loads the search terms into a vector
    LoadPuzzleFile(puzzleFile);

    cout << "Word Search:" << endl << endl;
    outFile << "Word Search: " << endl << endl;

    //Prints out the grid as it is unsolved
    for (int i = 0; i < COLUMNDEPTH; i++)
    {
        for (int j = 0; j < ROWLENGTH; j++)
        {
            cout << puzzleGrid[j][i];
            outFile << puzzleGrid[j][i];
            solvedGrid[j][i] = puzzleGrid[j][i]; //Also copies the puzzle grid to the "solved array"

        }
        cout << endl;
        outFile << endl;
    }

    cout << endl;
    outFile << endl;

    //Searches through each letter in the grid or row and attempts to match to words from the searchTerms vector
    SearchGrid(searchTerms, numberOfTerms);

    // Cycles through each search term in the vector and prints out whether it was found or not and the position
    for (int i = 0; i < numberOfTerms; i++)
    {
        if (searchTerms.at(i).found)
        {
            cout << searchTerms.at(i).word << " found on line " << searchTerms.at(i).directionCoords[0].Y + 1<< " position " << searchTerms.at(i).directionCoords[0].X + 1 << endl;
            outFile << searchTerms.at(i).word << " found on line " << searchTerms.at(i).directionCoords[0].Y + 1 << " position " << searchTerms.at(i).directionCoords[0].X + 1  << endl;
        }
        else
        {
            cout << searchTerms.at(i).word << " not found" << endl;
            outFile << searchTerms.at(i).word << " not found" << endl;
        }

    }

    // Cycles through each search term in the vector and travels along the stored directions to replace the characters with dots in the solvedGrid
    for (int w = 0; w < numberOfTerms; w++)
    {
        if (!searchTerms.at(w).found)
        {
            break;
        }

        string searchTerm = searchTerms.at(w).word;

        for (int i = 0; i <= searchTerm.length(); i++)
        {
            int X = searchTerms.at(w).directionCoords[i].X;
            int Y = searchTerms.at(w).directionCoords[i].Y;

            // Array boundary check for setting characters in the solved grid
            if (X >= 0 && X < ROWLENGTH &&
                Y >= 0 && Y < COLUMNDEPTH)
            {
                solvedGrid[X][Y] = replaceChar; //Changes the character
            }

        }
    }


    cout << endl;
    outFile << endl;

    //Prints out the solved grid after the ccharacters have been replaced by the replacecharacter. also outputs to file
    for (int i = 0; i < COLUMNDEPTH; i++)
    {
        for (int j = 0; j < ROWLENGTH; j++)
        {
            outFile << solvedGrid[j][i];
            cout << solvedGrid[j][i];

        }
        outFile << endl;
        cout << endl;
    }

    //Clean up of files
    searchFile.close();
    puzzleFile.close();
    outFile.close();
}

// Resets all values to default inbetween files
void ResetGlobalArrays()
{
    //PuzzleGrid
    memset(puzzleGrid, 0, sizeof(puzzleGrid));

    //SolvedGrid
    memset(solvedGrid, 0, sizeof(solvedGrid));

    //SearchTerms (vector)
    searchTerms.clear();

}

// Generic function to open a file and display if there has been a problem
void OpenFile(ifstream &file, string directory)
{
    file.open(directory); // Attempts to open the file
    if (file.fail()) //If the file if not found displays an error and pauses
    {
        cout << "File not found" << endl;
        system("pause");
    }

}

// Loads the search terms from the file into a vector
void LoadSearchTerms(ifstream &file, vector<term> &searchTerms, int &count)
{
    while (!file.eof()) // While not at the end of the file
    {
        string searchTerm;
        while (file >> searchTerm)  // Reads in each string from the text file
        {
            searchTerms.push_back(*(new term({
                searchTerm, //String of the term
                false // bool whether it has been found
            })));

            count++; // Increments the number of terms counter, this is returned back to main   
        }
    }
}

// Loads the puzzle grid into the 2d array
void LoadPuzzleFile(ifstream &file)
{
    int r = 0; // X
    int c = 0; // Y 
    string line;

    while (getline(file, line)) // Loads the ful line from the array
    {
        for (int i = 0; i < line.length(); i++) // For each character in the Line
        {
            puzzleGrid[r][c] = line.at(r); // Adds it to the position of the 2D array
            r++; // Increments the row position index
        }
        c++; // When at the last character of the line, increments the column index
        r = 0; // Resets the row index to 0 when moving onto a new line
    }

}

// This function calls the other search functions below
// It loops through each individual character in the array
void SearchGrid(vector<term> &terms, int numOfTerms)
{
    for (int w = 0; w < numOfTerms; w++)
    {
        // Sets all direction co ordinates for the selected word to -1 (outside the boundary) to be set later)
        for (int i = 0; i < MAXWORDLENGTH; i++)
        {
            searchTerms.at(w).directionCoords[i].X = -1; // Ensures that 'false' values (remaining 0's) 
            searchTerms.at(w).directionCoords[i].Y = -1; // do not make it to the final solved grid
        }

        for (int c = 0; c < COLUMNDEPTH; c++)
        {
            for (int r = 0; r < ROWLENGTH; r++)
            {

                if (!searchTerms.at(w).found)
                {
                    //Runs a search starting at each individual character in the grid
                    if (Search(r, c, w))
                    {
                        // Sets the start position in the struct
                        searchTerms.at(w).directionCoords[0].X = r;
                        searchTerms.at(w).directionCoords[0].Y = c;
                    }
                    // If the terms aren't found during the first search
                    else if (OrthogonalSearch(r, c, w))
                    {
                        // Sets the start position in the struct
                        searchTerms.at(w).directionCoords[0].X = r;
                        searchTerms.at(w).directionCoords[0].Y = c;
                    }
                }
            } // end of row 
        } // end of end of column
    } // end of number of terms


}

// Default search for non orthogonal words. travel in all 8 directions from the r and c position passed in as a paramter
bool Search(int row, int col, int termIndex)
{
    string searchTerm = searchTerms.at(termIndex).word; // local variable to store the word (Saves writing getter from vector every time?)

    if (puzzleGrid[row][col] != searchTerm.at(0))
    {
        return false; // Not the first letter of the word
    }

    int length = searchTerm.length();

    for (int dir = 0; dir < numOfSearchDirections; dir++) // 8 Possible directions from the correct letter match
    {
        int letter; // Variable to hold which letter the search is currently up too.

                    //Moves the row and column pointers by the "direction" 
        int r = row + x[dir];
        int c = col + y[dir];


        for (letter = 1; letter < length; letter++) // Loops from letter position to length
        {
            //Boundary check
            if (r >= ROWLENGTH || r < 0 || c >= COLUMNDEPTH || c < 0)
            {
                break; // Breaks when outside the array boundary
            }

            //match check
            if (puzzleGrid[r][c] == searchTerm.at(letter))
            {
                //Stores the r and c co-ordinates into the words struct for tracing later
                searchTerms.at(termIndex).directionCoords[letter].X = r;
                searchTerms.at(termIndex).directionCoords[letter].Y = c;
            }
            else
            {
                break;
            }

            //if above statements do not cause a break 2 or more consecutive characters must match, moves the current row and column by the direction
            r += x[dir];
            c += y[dir];
        }

        //If all letters in the word is matched, letter should = length at this point
        if (letter == length)
        {
            searchTerms.at(termIndex).found = true;
            return true;
        }

    } // end of direction for loop

      //Default return
    return false;
}


// The first step for Orthogonal search matches the first letter then calls the recursive function
bool OrthogonalSearch(int row, int col, int termIndex)
{
    string searchTerm = searchTerms.at(termIndex).word;

    if (puzzleGrid[row][col] != searchTerm.at(0)) //Ensures the first letter matches
    {
        return false;
    }

    // Only gets this far if the first letter matches
    if (furtherSearch(row, col, termIndex, 1)) // Calls the further search function on the second letter (that then calls itself on each subsequent letter)
    {
        return true;
    }

    return false; // Returns false as a default path
}

//Recursive function that calls itself each letter and if the letter reaches the length of the word returns true
bool furtherSearch(int row, int col, int termIndex, int letter)
{
    string searchTerm = searchTerms.at(termIndex).word; // Stores the search term as a local string for easier access

    for (int dir = 0; dir < numOfOrthDirections; dir++) // 4 Possible directions from the correct letter match
    {
        //Increments the row and column index by the orthX "direction"
        int r = row + orthX[dir];
        int c = col + orthY[dir];

        int length = searchTerm.length();
        length -= 1;

        //Boundary checks
        if (r >= ROWLENGTH || r < 0 || c >= COLUMNDEPTH || c < 0)
        {
            r = -1; // If leaves the boundary of the "matrix" set's the values to -1 
            c = -1; // This ensures that the solved grid does not print false positive values
        }

        if (puzzleGrid[r][c] == searchTerm.at(letter)) // matches the Row and column ( with the direction modifiers) to the letter of the searchterm
        {
            searchTerms.at(termIndex).directionCoords[letter].X = r; // Stores the direction index of the letter so the path can be traced back later
            searchTerms.at(termIndex).directionCoords[letter].Y = c;

            if (letter == length) //if found every character 
            {
                searchTerms.at(termIndex).found = true; // Word is found
                return true;
            }
            else // Not at end of word
            {
                if (furtherSearch(r, c, termIndex, letter + 1)) // Recursively calls this function again. now looking for the next letter
                {

                    return true; // Returns true if the recursive function matches
                }
            }

        }
    }
    return false; // Returns false as a default path
}