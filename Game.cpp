#include "Game.h"
/*
Init
*/
Game::Game(Board *pBoard, Pieces *pPieces, IO *pIO, int pScreenHeight)
{
	mScreenHeight = pScreenHeight;

	// Get the pointer to the Board and Pieces classes
	mBoard = pBoard;
	mPieces = pPieces;
	mIO = pIO;

	// Game initialization
	InitGame();
}
/*
Returns a random int between two integers

Parameters: pA - lower bound
			pB - upper bound
*/
int Game::GetRand(int pA, int pB)
{
	return rand() % (pB - pA + 1) + pA;
}
/*
Initial parameters of the game
*/
void Game::InitGame()
{
	//init random numbers
	srand((unsigned int)time(NULL)); // seed rand fcn

	//First Piece
	mPiece = GetRand(0, 6);
	mRotation = GetRand(0, 3);
	mPosX = (BOARD_WIDTH / 2) + (*mPieces).GetXInitialPosition(mPiece, mRotation);
	mPosY = (*mPieces).GetYInitialPosition(mPiece, mRotation);

	//Next Piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
	mNextPosX = BOARD_WIDTH + 5;
	mNextPosY = 5;
}
/*
Creates a random piece
*/
void Game::CreateNewPiece()
{
	// new piece
	mPiece = mNextPiece;
	mRotation = mNextRotation;
	mPosX = (BOARD_WIDTH / 2) + (*mPieces).GetXInitialPosition(mPiece, mRotation);
	mPosY = (*mPieces).GetYInitialPosition(mPiece, mRotation);

	//random next piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
}
/*
Draws a piece onto the board
*/
void Game::DrawPiece(int pX, int pY, int pPiece, int pRotation)
{
	color mColor;

	int mPixelsX = (*mBoard).GetXPosInPixels(pX);
	int mPixelsY = (*mBoard).GetYPosInPixels(pY);

	for (int r = 0; r < PIECE_BLOCKS; r++)
	{
		for (int c = 0; c < PIECE_BLOCKS; c++)
		{
			switch ((*mPieces).GetBlockType(pPiece, pRotation, c, r))
			{
				case 1: mColor = GREEN;
					break;
				case 2: mColor = BLUE; 
					break;
			}

			if ((*mPieces).GetBlockType(pPiece, pRotation, c, r) != 0)
				(*mIO).DrawRectangle(mPixelsX + r * BLOCK_SIZE, mPixelsY + c * BLOCK_SIZE, (mPixelsX + r * BLOCK_SIZE) + BLOCK_SIZE - 1, (mPixelsY + c * BLOCK_SIZE) + BLOCK_SIZE - 1, mColor);
		}
	}
}

/*
Draws the two lines that determine limits of the board
*/
void Game::DrawBoard()
{
	// Calculate the limits of the board in pixels  
	int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
	int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
	int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);

	// Check that the vertical margin is not to small
	//assert (mY > MIN_VERTICAL_MARGIN);

	// Rectangles that delimits the board
	mIO->DrawRectangle(mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, BLUE);

	mIO->DrawRectangle(mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, BLUE);

	// Check that the horizontal margin is not to small
	//assert (mX1 > MIN_HORIZONTAL_MARGIN);

	// Drawing the blocks that are already stored in the board
	mX1 += 1;
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{
			// Check if the block is filled, if so, draw it
			if (!mBoard->IsFreeBlock(i, j))
				mIO->DrawRectangle(mX1 + i * BLOCK_SIZE,
					mY + j * BLOCK_SIZE,
					(mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
					(mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1,
					RED);
		}
	}
}
/*
Draws all objects
*/
void Game::DrawScene()
{
	DrawBoard();                                                   // Draw the delimitation lines and blocks stored in the board
	DrawPiece(mPosX, mPosY, mPiece, mRotation);                    // Draw the playing piece
	DrawPiece(mNextPosX, mNextPosY, mNextPiece, mNextRotation);    // Draw the next piece
}