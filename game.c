//***EX2, WRITTEN BY: Amir Lavi, ID: 300202595***//
//===============================================//
//================***GAME.C***===================//
//===============================================//

#include <stdio.h>
#include "mineSweeper.h"
#include "colorPrint.h"
#include "getch.h"

//function main//
//=============//
int main()
{
	int rows, cols, level;
	while (!printMenu(&rows, &cols, &level)); //print menu
	
	GameBoard gb;
	bool boardInit = initBoard(&gb, rows, cols, level);
	if (!boardInit) //if initBoard failed
	{
		printf("ERROR, Try compiling again.\n");
		return 0;
	}
	else
	{
		int cursorCoords[2] = {0, 0}; //the start cursor
		Command key = getch();
		while (key != QUIT) //while q wasn't typed
		{
			clearScreen();
			printBoard(&gb, cursorCoords);
			//if all the flags are on mines and no hidden tiles left
			if (gb.hiddenTiles == 0 && gb.totalMines == 0)
			{
				printf("You Win!\n");
				freeMemory(&gb, gb.rows); //free the memory used
				return 0;
			}
			key = getch();
			switch (key)
			{
				//================FLAG_TILE================//
				case FLAG_TILE:
				{
					flagTile(&gb, cursorCoords[X], cursorCoords[Y]);
					break;
				}
				
				//================CLICK_TILE================//
				case CLICK_TILE:
				{
					clickTile(&gb, cursorCoords[X], cursorCoords[Y]);
					clearScreen();
					// printBoard is here again in case a mine was clicked
					// i want to print the board and show it in red
					printBoard(&gb, cursorCoords);
					if (gb.isMineClicked) //mine was clicked
					{
						printf("You Loose!\n");
						freeMemory(&gb, gb.rows); //free the memory used
						return 0;
					}
					break;
				}
				
				// very simple - will add or deduct 1 from
				// the coords array accordingly to the user will
				//================UP================//
				case UP:
				{
					if (cursorCoords[X] == 0)
						cursorCoords[X] = gb.rows - 1;
					else
						cursorCoords[X]--;	
					break;				
				}
					
				//================DOWN================//
				case DOWN:
				{
					if (cursorCoords[X] == gb.rows - 1)
						cursorCoords[X] = 0;
					else
						cursorCoords[X]++;	
					break;					
				}
					
				//================RIGHT================//
				case RIGHT:
				{
					if (cursorCoords[Y] == gb.cols - 1)
						cursorCoords[Y] = 0;
					else
						cursorCoords[Y]++;
					break;
				}
					
				//================LEFT================//
				case LEFT:
				{
					if (cursorCoords[Y] == 0)
						cursorCoords[Y] = gb.cols - 1;
					else
						cursorCoords[Y]--;
					break;
				}
				case QUIT: 
				{
					continue;
				}
			}
		}
	}
	return 0;
}


