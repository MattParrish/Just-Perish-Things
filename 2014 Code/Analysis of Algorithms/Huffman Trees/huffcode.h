// huffcode.h
// Matthew Parrish
// 3 December 2014
//
// For CS 411 Fall 2014
// Assignment 6. 
// Make a class that will create a Huffman tree, and use it to encode and decode strings.
// Header for class HuffCode

// Based on huffcode.h, by Glenn G. Chappell
// Original documentation below:
	// huffcode.h  UNFINISHED
	// Glenn G. Chappell
	// 28 Nov 2014
	//
	// For CS 411/611 Fall 2014
	// Assignment 6, Exercise A
	// Header for class HuffCode

#ifndef FILE_HUFFCODE_H_INCLUDED
#define FILE_HUFFCODE_H_INCLUDED

#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map
#include <vector>
using std::vector;

// struct HuffmanNode
// the nodes of our Huffman Tree.
struct HuffmanNode
{
	std::string character;       // NULL if internal node
	int weight;
	int parentLocation;          // -1 If not have one
	int leftOrRightChild;        // 0 is left, 1 is right, -1 if not set
	bool usedYetInTree;          // False if node does not have a parent              
};

// Class HuffCode
// Encoding & decoding using a Huffman code
class HuffCode {

// ***** HuffCode: ctors, dctor, op= *****
public:

    // Compiler-generated default ctor, copy ctor, copy =, dctor used

// ***** HuffCode: general public functions *****
public:

    void setWeights(const std::unordered_map<char, int> & theweights);

    std::string encode(const std::string & text) const;

    std::string decode(const std::string & codestr) const;

// ***** HuffCode: data members *****
private:
	vector<HuffmanNode> huffTree;
	std::unordered_map<std::string, std::string> stringToNum;
	std::unordered_map<std::string, std::string> numToString;
};  // End class HuffCode


#endif //#ifndef FILE_HUFFCODE_H_INCLUDED

