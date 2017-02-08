// colorstack.cpp
// Matthew C. Parrish
// November 20th 2014
//
// For CS 411 Fall 2014
// Assignment 5
// colorstack's .cpp file
// functions to calculate the minimum weight of a piling of blocks to a specified height.

#include "colorstack.h"
#include <climits>     // For INT_MAX

const int BOTTOM = 0;    // Index of the bottom 'color' of a Block
const int TOP    = 1;    // Index of the top 'color' of a Block
const int HEIGHT = 2;    // Index of the height of a Block
const int WEIGHT = 3;    // Index of the weight of a Block

vector<vector<int> > bestWeightTable;
int howManyColors;


// colorStack
// Returns the minimum weight of a stack of blocks, with height >= given height.
int colorStack(const vector<Block> & givenBlocks, int requiredHeight)
{
	// Logic for finding the range of how many colors we have total	
	int minColor = INT_MAX;	
	int maxColor = 0;
	for(size_t i = 0; i < givenBlocks.size(); i++)
	{
		if(givenBlocks[i][TOP] > maxColor)
			maxColor = givenBlocks[i][TOP];

		if(givenBlocks[i][BOTTOM] > maxColor)
			maxColor = givenBlocks[i][BOTTOM];

		if(givenBlocks[i][TOP] < minColor)
			minColor = givenBlocks[i][TOP];

		if(givenBlocks[i][BOTTOM] < minColor)
			minColor = givenBlocks[i][BOTTOM];	
	}
	howManyColors = maxColor - minColor + 1;
    
	vector<Block> myBlocks = givenBlocks;
	for(size_t i = 0; i < myBlocks.size(); i++)
	{
		myBlocks[i][TOP] -= minColor;
		myBlocks[i][BOTTOM] -= minColor;
	}
    
	// Sets bestWeightTable to the proper size, all values initilized to -1
	bestWeightTable.resize(requiredHeight+1);
	for(size_t i = 0; i < bestWeightTable.size(); i++)
	{
		bestWeightTable[i].resize(howManyColors);
		for(size_t j = 0; j < bestWeightTable[i].size(); j++)
			bestWeightTable[i][j] = -1;
	}
    
	// Calculates the best weight for the required height
	int currentBestWeight = INT_MAX;
	for(int i = 0; i < howManyColors; i++)	
	{
		int tempBestWeight = colorStackHelper(myBlocks, requiredHeight, i);

		if (tempBestWeight < currentBestWeight)
			currentBestWeight = tempBestWeight;
	}

	// If no stack exists that reaches the specified height, return -1
	if(currentBestWeight == INT_MAX)
		return -1;
    
	// if we've found a return value, we reached our goal
	return currentBestWeight;
}


// colorStackHelper
// Recursive function to determine the lightest weight pile of blocks with high >= requiredHeight,
//   and with the specified top color.
int colorStackHelper(const vector<Block> & givenBlocks, int requiredHeight, int topColor)
{
	// BASE CASE
	if(bestWeightTable[requiredHeight][topColor] != -1)
		return bestWeightTable[requiredHeight][topColor];

	// For each block: If the top color is one the one we want,
	//   and the block's height >= required height,
	//   then compare the block's weight to the best weight so far. 
	//   If new block is better, new block becomes best block.
	int currentBestWeight = INT_MAX;
	for(size_t i = 0; i < givenBlocks.size(); i++)
	{
		if(givenBlocks[i][TOP] == topColor)
		{
			if(givenBlocks[i][HEIGHT] >= requiredHeight)
			{
				if(givenBlocks[i][WEIGHT] < currentBestWeight)
					currentBestWeight = givenBlocks[i][WEIGHT];
			}
			else	
			{
				int tempWeight = colorStackHelper(givenBlocks,
								  requiredHeight - givenBlocks[i][HEIGHT],
								  givenBlocks[i][BOTTOM]);
				if(tempWeight != INT_MAX)
				{
					tempWeight += givenBlocks[i][WEIGHT];

					if(tempWeight < currentBestWeight)
						currentBestWeight = tempWeight;
				}
			}
		}
	}

	bestWeightTable[requiredHeight][topColor] = currentBestWeight;
	return currentBestWeight;
}
