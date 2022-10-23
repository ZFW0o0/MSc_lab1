#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sudoku.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in) {
    cout << "Failed!\n";
  }
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3)) {
    cout << "  +===========+===========+===========+\n";
  } else {
    cout << "  +---+---+---+---+---+---+---+---+---+\n";
  }
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|\n";
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) {
    cout << (char) ('1'+r) << "   ";
  }
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}

/* add your functions here */
/* is_complete(board) takes a 9 × 9 array of characters representing a Sudoku
   board and returns true if all board positions are occupied by digits, and false
   otherwise */
bool is_complete(const char board[9][9])
{
  /* a nested for loop that goes over all the elements in the two-dimension board array and
  returns false whenever the element is a "."  */
  for (int r = 0; r < 9; r++)
  {
    for (int c = 0; c < 9; c++)
    {
      if (board[r][c] > '9' || board[r][c] < '1')
      {
	return false;
      } 
    } 
  }
  
  /* after going over all the elements in the array and no "." is found, the board should 
     be complete and so return true */
  return true;
}

/*make_move(position, digit, board) attempts to place a character type digit (from 1 to 9)
  onto a Sudoku board (9*9 character array) at a given position, position denoted by a 
  two-character string showing row (A to I) and column (1 to 9) coordinates (e.g. I8 means
  row I and column 8)  */
bool make_move(const char* position,char digit,char board[9][9])
{
  /*  return false if the position string is not a two-character string, or if position is 
      out of the range of the sudoku board  */
  int pos_length;
  pos_length = position.length();
  if (pos_length != 2)
  {
    return false;
  }
  
  if (position[0] < 'A' || position[0] > 'I')
  {
    return false;
  }

  if (position[1] < '1' || position[1] > '9')
  {
    return false;
  }

  /* returns false if the digit is not between 1-9 */
  if ((digit - '1') < 0 || (digit - '1') > 9)
  {
    return false;
  }
  
  /* covert character row coordinate to integer row number, e.g. 'B' converts to 1 */
  /* covert character column coordinate to integer column number, e.g. '3' converts to 2 */
  int row;
  row = position[0] - 'A';
  int column;
  column = position[1] - '1';

  /* return false if the position already has a digit */
  if (board[row][column] != '.')
  {
    return false
  }

  /* return false if placing the digit at the position would lead to more than one copy of the
   same digit in the same row */
  for (int i = 0; i < 9; i++)
  {
    if (board[row][i] == digit)
    {
      return false;
    }
  }

  /* return false if placing the digit at the position would lead to more than one copy of the
     same digit in the same column */
  for (int i = 0; i < 9; i++)
  {
    if (board[i][column] == digit)
    {
      return false;
    }
  }

  /* return false if placing the digit at the position would lead to more than one copy of the
     same digit in the same 3*3 sub-boards */
  /* sub_row represents the row number of the position in the sub-board it is located in,
     similarly, sub_column represents the column number in the sub-board. */
  int sub_row = row % 3;
  int sub_column = column % 3;
  /* the nested for loop walks through all the elements in the 3*3 sub-board of the position */ 
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (board[row - sub_row + i][column - sub_column + j] == digit)
      {
	return false;
      }     
    }
  }

  /* update the board array to reflect the placing of digit at position. */
  board[row][column] = digit;

  return true;
}

/* save_board outputs the two-dimensional character array board to a file with name filename.
 
 *@param filename, cstring for the file path to which to write board
 *@param board, array that stores the digits in sudoku board  
 *return: true if the file was successfully written, false otherwise
*/

bool save_board(const char* filename, char board[9][9])
{
  ofstream out(filename);
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      out << board[i][j];
    }
    out << endl;
  }

/* check whether file was successfully written.
   use load_board() to read the file that was written to 
   check if the output_board array created by load_board() is the same as the 
   board array argument passed into save_board() function
*/
  char output_board[9][9];
  load_board(filename, output_board);
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      if (output_board[i][j] != board[i][j])
      {
	return false;
      }
    }
  }
  
  return true;

}


/* coord_convert converts interger row and column number to string coordinate  */
string coord_convert(int row, int column)
{
  char position[3]; 
  position[0] = 'A' + row;
  position[1] = '1' + column;
  position[2] = '\0';

  return position;
}

/*digit_convert converts integer digit to char digit */
char digit_covert(int number)
{
  char digit;
  digit = '0' + number;

  return digit;
}

/* back_tracking() attempts to solve the Sudoku puzzle in input/output parameter board. 
 * use brute-force search to go over the empty grids in a sudoku board
 * try placing 1 at an empty grid
 * check for violation (no same digit in same row,column and sub-board)
 * if no violation move on to next empty grid
 * if there is violation, increase the digit by 1 and check for violation again.
 * if no valid digit from 1 to 9, leave grid blank, go back to previous grid 
 * repeat until the last grid  

 *@param board, unsolved sudoku board
 *@param row, column, the coordinate of the grid  

 *return true if there is a solution for the sudoku, false otherwise.
 */


int back_tracking(char board[9][9],int row,int column)
{
  static int count = 0; /*count records number of move making attempts, 
  			  greater count means harder sudoku */
  bool valid; 
  if (column == 9)
  {
    row++;
    column = 0;
  }
  if (row = 9)
  {
    return count;
  }
  if (board[row][column] != '.')
  {
    back_tracking(board, row, column + 1);
  }
    
  for (int i = 1; i <= 9; i++)
  {
    position = coord_convert(row, column);
    digit = digit_convert(i);
    if (make_move(position, digit, board))
    {
      board[row][column] = i;
      count++;         /* records number of move making attempts, 
			   greater count means harder sudoku */
      if (back_tracking(board, row, column + 1))
      {
	return count;
      }
      board[row][column] = '.'
    }

  }

  return 0;  
}

/*solve_board implements back_tracking() and return true if sudoku is solvable */ 
bool solve_board(char board[9][9])
{
  if (back_tracking(board, 0, 0))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* count_attempt() is called in back_tracking() whenever there is a 
successful make move, count records number of move making attempts, 
greater count means harder sudoku */
int count_attempt()
{
  static int count = 0;

  count++;
  return count; 
}
