// gcs.h
// Matthew C. Parrish
// 10/16/2014
//
// For CS 411 Fall 2014
// Assignment 3, Excercise A
// gcs's header file.
// Calculates the greatest contiguous sequence from a given list

#ifndef GCS_H_INCLUDED
#define GCS_H_INCLUDED

#include <algorithm>   // for std::max
using std::max;

// utility function to find maximum of three integers. Returns int a if all equal
int maxOfThree(int a, int b, int c)
{
    return max(max(a, b), c);
}

// utility function to find the maximum of four integers. Returns int a if all equal
int maxOfFour(int a, int b, int c, int d)
{
    return max(maxOfThree(a, b, c), d);
}

// gcsDataStruct, a struct for holding the gcs of:
//    Current greatest subarray
//    Current greatest subarray constructed including the first element
//    Current greatest subarray constructed including the last element
//    Current value of the entire array
struct gcsDataStruct
{
    int greatestSubarray;
    int leftHalf;
    int rightHalf;
    int total;
};

// gcsWorkHorse function
// Recursive function, given a random access iterator to the first and last
//    element in a list, calculates the greatest contiguous sums of the list.
//    Returns a gcsDataStruct with the values:
//       greatestSubarray is the largest contiguous sum in the list
//       leftHalf is the greatest contiguous sum from the first element
//       rightHalf is the greatest contiguous sum from the last element
//       total is the total sum of the list
template<typename RAIter>
gcsDataStruct gcsWorkhorse(RAIter first, RAIter last)
{
    gcsDataStruct returnStruct;

    // Base Cases
    //   If Empty, set everything to 0, done.
    if (first == last)
    {
        returnStruct.greatestSubarray = 0;
        returnStruct.leftHalf = 0;
        returnStruct.rightHalf = 0;
        returnStruct.total = 0;
        return returnStruct;
    }
    // If last element, populate the struct
    if (first == last-1)
    {
        returnStruct.greatestSubarray = *first;
        returnStruct.leftHalf = *first;
        returnStruct.rightHalf = *first;
        returnStruct.total = *first;
        return returnStruct;
    }

    // Calculates the middle of the list
    RAIter middle = first + (last - first)/2;

    // Recursive calls on both halves
    gcsDataStruct leftHandSide = gcsWorkhorse(first, middle);
    gcsDataStruct rightHandSide = gcsWorkhorse(middle, last);

    gcsDataStruct totalSoFar;

    // Calculates the total from both halves
    totalSoFar.total = leftHandSide.total + rightHandSide.total;

    // Calculates the gcs including the first element
    totalSoFar.leftHalf = maxOfFour(leftHandSide.leftHalf,
                                     leftHandSide.total,
                                     leftHandSide.total + rightHandSide.leftHalf,
                                     totalSoFar.total)

    // Calculates the gcs including the last element
    totalSoFar.rightHalf = maxOfFour(rightHandSide.rightHalf,
                                      rightHandSide.total,
                                      rightHandSide.total + leftHandSide.rightHalf,
                                      totalSoFar.total)

    // Calculates the greatest subarray
    totalSoFar.greatestSubarray = maxOfThree(leftHandSide.greatestSubarray,
                                    rightHandSide.greatestSubarray,
                                    leftHandSide.rightHalf + rightHandSide.leftHalf);

    return totalSoFar;
}

// gcs
// Takes two random access iterators
//   Returns either the greatest subarray of the list the iterators point to,
//   or 0, if the greatest subarray is negative
template<typename RAIter>
int gcs(RAIter first, RAIter last)
{
    gcsDataStruct biggestValues = gcsWorkhorse(first, last);
    return max(0, biggestValues.greatestSubarray);
}

#endif // GCS_H_INCLUDED
