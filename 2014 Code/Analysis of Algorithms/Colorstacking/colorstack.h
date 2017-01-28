// colorstack.h
// Matthew C. Parrish
// November 20th 2014
//
// For CS 411 Fall 2014
// Assignment 5
// colorstack's .h file
// Header for functions to calculate the minimum weight of a piling of blocks to a specified height.

#ifndef COLORSTACK_H_INCLUDED
#define COLORSTACK_H_INCLUDED

#include <vector>
using std::vector;

typedef vector<int> Block;

int colorStack(const vector<Block> & blockVector, int height);
int colorStackHelper(const vector<Block> & , int, int );

#endif // COLORSTACK_H_INCLUDED
