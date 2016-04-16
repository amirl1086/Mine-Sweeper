//***EX2, WRITTEN BY: Amir Lavi, ID: 300202595***//
//===============================================//
//==============***MINESWEEPER.C***==============//
//===============================================//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mineSweeper.h"
#include "colorPrint.h"
#include "getch.h"

//function initBoard - will initialize the game board//
//===================================================//
bool initBoard(GameBoard *g, int rows, int cols, int level)
{
	g->rows = rows;
	g->cols = cols;
	g->isMineClicked = FALSE;
	g->hiddenTiles = rows * cols;
	g->board = (Tile**)malloc(sizeof(Tile*)*rows);
	if (g == NULL)
		return FALSE;
	int i = 0;
	while (i < rows) //malloc to each pointer
	{
		g->board[i] = (Tile*)malloc(sizeof(Tile)*cols);
		if (g->board[i] == NULL) //if 1 was faild
		{ //deleting all arrays from 0 to the index that failed
			freeMemory(g, i);
			return FALSE;
		}
		i++;
	}
	int j;
	//these loops will initialize the tiles
	for (i = 0; i < g->rows; i++)
	{
		for (j = 0; j < g->cols; j++)
		{
			g->board[i][j].numOfMines = 0;
			g->board[i][j].isFlagged = FALSE;
			g->board[i][j].isVisible = FALSE;
		}	
	}
	populateMines(g, level);
	return TRUE;
}
//function clickTile - will watch the game board for clicks//
//=========================================================//
void clickTile(GameBoard *g, int row, int col)
{
	//if out of the board limits
	if (row < 0 || row == g->rows || col < 0 || col == g->cols)
		return;
	//if clicked on a flag
	if (g->board[row][col].isFlagged)
		return;
	//if clicked on a mine
	if (g->board[row][col].isMine)
	{
		g->board[row][col].isVisible = TRUE;
		g->isMineClicked = TRUE;
		return;
	}
	//if clicked on a tile that visible
	if (g->board[row][col].isVisible)
		return;
	//if clicked on a tile nearby a mine
	if (g->board[row][col].numOfMines > 0)
	{
		g->hiddenTiles--;
		g->board[row][col].isVisible = TRUE;
		return;
	}
	//if clicked on a tile without a mine nearby
	g->hiddenTiles--;
	g->board[row][col].isVisible = TRUE;
	
	//recursion calls as requested
	clickTile(g, row - 1, col - 1);
	clickTile(g, row - 1, col);
	clickTile(g, row - 1, col + 1);
	clickTile(g, row, col - 1);
	clickTile(g, row, col + 1);
	clickTile(g, row + 1, col - 1);
	clickTile(g, row + 1, col);
	clickTile(g, row + 1, col + 1);
}

//function flagTile - will flag tiles on the game board//
//=====================================================//
void flagTile(GameBoard *g, int row, int col)
{
	//if the tile is visible
	if (g->board[row][col].isVisible)
		return;
	//if flagged a mine - correct choice (the player doesn't know)
	//it's for the indication if the player wins
	if (!g->board[row][col].isFlagged && g->board[row][col].isMine)
	{
		g->board[row][col].isFlagged = TRUE;
		g->totalMines--; //forget about this mine
		g->hiddenTiles--; //-1 to the revealed tiles
		return;
	}
	//if flagged a tile without a mine (wrong choice)
	if (!g->board[row][col].isFlagged && !g->board[row][col].isMine)
	{
		g->board[row][col].isFlagged = TRUE;
		return;
	}
	//remove flag from a correct choice (there is a mine in this tile)
	if (g->board[row][col].isFlagged && g->board[row][col].isMine)
	{
		g->board[row][col].isFlagged = FALSE;
		g->totalMines++; //remember again this mine
		g->hiddenTiles++; //+1 to the tiles to reveal
		return;
	}
	//remove flag from a wrong choice (no mine in this tile)
	if (g->board[row][col].isFlagged && !g->board[row][col].isMine)
	{
		g->board[row][col].isFlagged = FALSE;
		return;
	}
}

//function populateMines - populate mines according to the difficulty level//
//=========================================================================//
void populateMines(GameBoard *g, int level)
{
	time_t t;
	srand((unsigned) time(&t));
	int i = 0, randomRow, randomCol;
	
	// calculate the number of mines to populate
	if (level == EASY)
		g->totalMines = (EASY_LEVEL_PERCENT * g->hiddenTiles) / 100;
	if (level == MEDIUM)
		g->totalMines = (MEDIUM_LEVEL_PERCENT * g->hiddenTiles) / 100;
	if (level == HARD)
		g->totalMines = (HARD_LEVEL_PERCENT * g->hiddenTiles) / 100;
	
	while (i < g->totalMines)
	{
		// randomize the place to put a mine
		randomRow = rand() % g->rows;
		randomCol = rand() % g->cols;
		//if already been randomized
		if (g->board[randomRow][randomCol].isMine)
			continue;
		else
		{
			g->board[randomRow][randomCol].isMine = TRUE;
			i++;
		}
	}
	markNumbers(g);
}

//function markNumbers - will mark how much mines every tile has nearby it//
//========================================================================//
void markNumbers(GameBoard *g)
{
	int i, j;
	for (i = 0; i < g->rows; i++)
		for (j = 0; j < g->cols; j++)
			if(!g->board[i][j].isMine) //don't count for mines
				g->board[i][j].numOfMines = countMines(g, i, j);
}

//function printBoard - will print the game board//
//===============================================//
void printBoard(GameBoard *g, int cursorCoords[2])
{
	int i, j;
	for (i = 0; i < g->rows; i++)
	{
		for (j = 0; j < g->cols; j++)
		{
			if (!g->board[i][j].isVisible) //if tile is hidden
			{ //if tile has flag on it
				if (g->board[i][j].isFlagged) 
				{
					//if the cursor is on the i, j tile
					if (i == cursorCoords[X] && j == cursorCoords[Y])
						colorPrint(FG_Black, BG_Green, ATT_Def, "|>");
					else
						colorPrint(FG_Black, BG_Magenta, ATT_Def, "|>");
				}
				else //if no flag, just hidden
				{
					//if the cursor is on the i, j tile
					if (i == cursorCoords[X] && j == cursorCoords[Y])
						colorPrint(FG_Black, BG_Green, ATT_Def, "  ");
					else
						colorPrint(FG_Black, BG_Magenta, ATT_Def, "  ");
				}
			}
			else //if the tile is revealed
			{
				if (g->board[i][j].numOfMines > 0)
				{
					char num [2];
					num[0] = ' ';
					num[1] = (char) g->board[i][j].numOfMines + 48;
					//if the cursor is on the i, j tile
					if (i == cursorCoords[X] && j == cursorCoords[Y])
						colorPrint(FG_Black, BG_Green, ATT_Def, num);
					else
						//different color for each number
						switch (g->board[i][j].numOfMines)
						{
							case 1:
								colorPrint(FG_Blue, BG_White, ATT_Def, num);
								break;
							case 2:
								colorPrint(FG_Yellow, BG_White, ATT_Def, num);
								break;
							case 3:
								colorPrint(FG_Red, BG_White, ATT_Def, num);
								break;
							case 4:
								colorPrint(FG_Green, BG_White, ATT_Def, num);
								break;
							case 5:
								colorPrint(FG_Cyan, BG_White, ATT_Def, num);
								break;
							case 6:
								colorPrint(FG_Magenta, BG_White, ATT_Def, num);
								break;
							case 7:
								colorPrint(FG_Black, BG_White, ATT_Def, num);
								break;
						}
				}
				else //if the tile has no mines nearby it, or a mine itself
				{
					if (g->board[i][j].isMine)
						colorPrint(FG_Black, BG_Red, ATT_Def, " *");
					else
					{
						//if the cursor is on the i, j tile
						if (i == cursorCoords[X] && j == cursorCoords[Y])
							colorPrint(FG_Black, BG_Green, ATT_Def, "  ");
						else
							colorPrint(FG_Black, BG_White, ATT_Def, "  ");
					}
				}
			}
		}
		printf("\n");
	}
}

//function printMenu - will print the menu//
//========================================//
bool printMenu(int *rows, int *cols, int *level)
{
	printf("Please enter number of rows, cols and level: ");
	scanf("%d", rows);
	scanf("%d", cols);
	scanf("%d", level);
	if (*level < 1 || *level > 3)
	{
		printf("Illegal level, try again!\n");
		return FALSE;
	}	
	if (*cols>MAX_BOARD_DIM || *rows>MAX_BOARD_DIM || *cols<1 || *rows<1)
	{
		printf("Illegal dimensions, try again!\n");
		return FALSE;
	}
	return TRUE;
}

//function countMines - will count how much mines the tiles has nearby it//
//=======================================================================//
int countMines(GameBoard *g, int row, int col)
{
	if (g->board[row][col].isMine)
		return 0;
	int mineCounter = 0;
	int j, i;
	for (i = row - 1; i <= row + 1; i++)
	{
		for (j = col - 1; j <= col + 1; j++)
		{
			//these conditions will check if the loop is out of the board
			//for each tile ij,  will go through the 8 tiles around it
			if (i < 0)
				i++;
			if (j < 0)
				j++;
			if (i >= g->rows)
				break;
			if (j >= g->cols)
				break;
			if (i == row && j == col)
				continue;
			//end.
			else
				if (g->board[i][j].isMine)
					mineCounter++;
		}
		if (i >= g->rows)
			break;
	}
	return mineCounter;
}

//function freeMemory - will free the memory//
//==========================================//
void freeMemory(GameBoard *g, int rowsToDel)
{
	//valgrind returns values as requested
	int i = 0;
	while (i < rowsToDel)
	{
		free(g->board[i]);
		i++;
	}
	free(g->board);
}
