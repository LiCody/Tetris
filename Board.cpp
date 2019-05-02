#include "Board.h"
/*
Init
*/
Board::Board(Pieces *pPieces, int pScreenHeight)
{
	// Get the screen height
	mScreenHeight = pScreenHeight;

	// Get the pointer to the pieces class
	mPieces = pPieces;

	//Init the board blocks with free positions
	InitBoard();
}
/*
Initializes the board with free positions
*/
void Board::InitBoard()
{
	for (int r = 0; r < BOARD_WIDTH; r++)
	{
		for (int c = 0; c < BOARD_HEIGHT; c++)
		{
			mBoard[r][c] = POS_FREE;
		}
	}
}
/*
Fill in space on the board with given

Parametes:  pX - horizontal position in blocks
			pY - vertical position in blocks
			pPiece - the type of piece to draw
			pRotation - 1 of 4 rotations
*/
void Board::StorePiece(int pX, int pY, int pPiece, int pRotation)
{
	for (int r1 = pX, r2 = 0; r1 < pX + PIECE_BLOCKS; r1++, r2++)
	{
		for (int c1 = pY, c2 = 0; c1 < pY + PIECE_BLOCKS; c1++, c2++)
		{
			if ((*mPieces).GetBlockType(pPiece, pRotation, c2, r2) != 0)
			{
				mBoard[r1][c1] = POS_FILLED;
			}
		}
	}
}
/*
Checks if the game is over by checking if a block is in the upper position
*/
bool Board::IsGameOver()
{
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		if (mBoard[i][0] == POS_FILLED)
			return true;
	}
	return false;
}
/*
Deletes a line of the board by moving all above lines down

Parameters: pY - vertical position in blocks to delete
*/
void Board::DeleteLine(int pY)
{
	for (int c = pY; c > 0; c--)
	{
		for (int r = 0; r < BOARD_WIDTH; r++)
		{
			mBoard[r][c] = mBoard[r][c - 1];
		}
	}
}
/*
Deletes all the lines that should be removed
*/
void Board::DeletePossibleLines()
{
	for (int c = 0; c < BOARD_HEIGHT; c++)
	{
		int r = 0;
		while (r < BOARD_WIDTH)
		{
			if (mBoard[r][c] != POS_FILLED)
				break;
			r++;
		}

		if (r == BOARD_WIDTH)
			DeleteLine(c);
	}
}
/*
Returns true if the block on the board is empty, false if it is filled

Parameters: pX - horizontal position in blocks
			pY - vertical position in blocks
*/
bool Board::IsFreeBlock(int pX, int pY)
{
	if (mBoard[pX][pY] == POS_FREE)
		return true;
	else
		return false;
}
/*
Returns horizontal position in pixels of given block

Parameters: pPos - Horizontal position of the block on the board
*/
int Board::GetXPosInPixels(int pPos)
{
	return((BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2))) + (pPos * BLOCK_SIZE));
}
/*
Returns vertical position in pixels of given block

Parameters: pPos - Horizontal position of the block on the board
*/
int Board::GetYPosInPixels(int pPos)
{
	return((BOARD_POSITION - (BLOCK_SIZE * (BOARD_HEIGHT / 2))) + (pPos * BLOCK_SIZE));
}
/*
Checks if the piece can be stored at the position onto the board without any collision 
Returns true if movement is possible and false otherwise

Parameters: pX - horiztonal position in blocks
			pY - vertical position in blocks
			pPiece - piece to draw
			pRotation - 1 of 4 rotations
*/
bool Board::IsPossibleMovement(int pX, int pY, int pPiece, int pRotation)
{
	for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++)
	{
		for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++)
		{
			//Checks for piece outside the limits of the board
			if (i1 < 0 || i1 > BOARD_WIDTH - 1 || j1 > BOARD_HEIGHT - 1)
			{
				if ((*mPieces).GetBlockType(pPiece, pRotation, j2, i2) != 0)
					return false;
			}

			//Checks if piece has collisioned with a block on the board
			if (j1 >= 0)
			{
				if (((*mPieces).GetBlockType(pPiece, pRotation, j2, i2) != 0) && (!IsFreeBlock(i1, j1)))
					return false;
			}
		}
	}

	return true;
}