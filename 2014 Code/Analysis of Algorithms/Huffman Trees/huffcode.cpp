// huffcode.cpp
// Matthew Parrish
// 3 December 2014
//
// For CS 411 Fall 2014
// Assignment 6. 
// The cpp file for huffcode.h

// Based on huffcode.cpp, by Glenn G. Chappell
// Original documentation below:
	// huffcode.cpp  UNFINISHED
	// Glenn G. Chappell
	// 28 Nov 2014
	//
	// For CS 411/611 Fall 2014
	// Assignment 6, Exercise A
	// Source for class HuffCode

#include "huffcode.h"  // for class HuffCode declaration
#include <string>      // for std::to_string
using std::string;
#include <unordered_map>
using std::unordered_map;

// setWeights
// Given a map of characters and weights, creates a huffman tree,
//   and populates a number to character map for decoding,
//   and a character to number map for encoding.
void HuffCode::setWeights(const unordered_map<char, int> & theweights)
{
	HuffmanNode temp;

	// Populate huffTree with the given characters and weights
	temp.parentLocation= -1;
	temp.leftOrRightChild = -1;
	temp.usedYetInTree = false;
	for(auto i = theweights.begin(); i != theweights.end(); ++i)
	{
		temp.character = i->first;
		temp.weight = i->second;
		huffTree.push_back(temp);
	}

	// Finds the two nodes with the least weight, unless there is only the 'root' node left
	while(true)
	{
		int lowestWeightA = huffTree.begin()->weight;
		auto lowestWeightAIndex = huffTree.begin(); 

		int lowestWeightB = (huffTree.begin()+1)->weight;       
		auto lowestWeightBIndex = huffTree.begin()+1;

		for(auto i = huffTree.begin(); i != huffTree.end(); ++i)
		{

			if(lowestWeightAIndex->usedYetInTree)
				lowestWeightA = INT_MAX;
			if(lowestWeightBIndex->usedYetInTree)
				lowestWeightB = INT_MAX;

			// If current node has a lower weight than A
			if(i->weight < lowestWeightA && i->usedYetInTree == false )    
			{
				auto oldA = lowestWeightAIndex; // temp = old A
				lowestWeightA = i->weight;      // Replace A
				lowestWeightAIndex = i;

				// If new node is smaller than A, and the old A was smaller than B, sets B = the old A
				if(oldA->weight < lowestWeightB && oldA->usedYetInTree == false)
				{
					lowestWeightB = oldA->weight;
					lowestWeightBIndex = oldA;
				}
			}
			// If current node is not smaller than A, but is smaller than B
			else if(i->weight <= lowestWeightB && i->usedYetInTree == false)
			{
				lowestWeightB = i->weight;
				lowestWeightBIndex = i;
			}
		}
		
		// Make new node: New weight = a's weight + b's weight,
		//                parent pointer = null
		//                usedYetInTree = false
		//                char = null characer of some sort
		//                left or right = -1
		// And put into the vector

		// Update old nodes: parent pointer = location in vector where new node will go
		//                   A is left child, set = 0
		//                   B is right child, set = 1
		//                   usedInTree = true

		HuffmanNode newParent;
		newParent.weight = lowestWeightAIndex->weight + lowestWeightBIndex->weight;
		newParent.character = "";
		newParent.parentLocation = -1;
		newParent.leftOrRightChild = -1;
		newParent.usedYetInTree = false;
		
		lowestWeightAIndex->parentLocation = huffTree.size();
		lowestWeightBIndex->parentLocation = huffTree.size();

		lowestWeightAIndex->leftOrRightChild = 0;
		lowestWeightBIndex->leftOrRightChild = 1;

		lowestWeightAIndex->usedYetInTree = true;
		lowestWeightBIndex->usedYetInTree = true;

		huffTree.push_back(newParent);

		// For a Huffman tree, we will have a total of 2k-1 nodes, where k is the # of given characters
		if(huffTree.size() == theweights.size() * 2 - 1)
			break;
	}

	// Travels through the vector, and creates the code for each character given
	for(size_t i = 0; i < theweights.size(); ++i)
	{
		string letterCode = "";
		string tempString;
		int nextParent = i;
		while(nextParent != -1)
		{
			tempString = std::to_string(huffTree[nextParent].leftOrRightChild);
			if(tempString == "-1")
				break;
			letterCode = tempString + letterCode;
			nextParent = huffTree[nextParent].parentLocation;
		}
		stringToNum[huffTree[i].character] = letterCode;
		numToString[letterCode] = huffTree[i].character;
	}
}

// encode
// Encodes a string using the Huffman tree we made.
//    Call only after setWeights has been called.
string HuffCode::encode(const string & text) const
{
	std::string returnValue = "";
	std::string tempString;
	std::string currentChar;

	// Runs through text, and encodes each letter
 	for(size_t i = 0; i < text.size(); ++i)
	{
		currentChar = text[i]; // Taking the characters off one at a time to encode
		tempString = stringToNum.at(currentChar);
		returnValue += tempString; 
	}
    	return returnValue;  
}

// decode
// Decodes a string using the Huffman tree we made.
//    Call only after setWeights has been called.
string HuffCode::decode(const string & codestr) const
{
	string returnString = "";

	// Use codestr to populate a string with the correct chars from the numToChar map.
	string checkInMapFor = "";
	for(size_t i = 0; i < codestr.length(); ++i)
	{
		checkInMapFor += codestr[i];  // Put the next character from codestr into the string to check against the map
		int found = numToString.count(checkInMapFor);
		if(found)
		{
			returnString += numToString.at(checkInMapFor);
			checkInMapFor = "";
		}
	}

    	return returnString;
}

