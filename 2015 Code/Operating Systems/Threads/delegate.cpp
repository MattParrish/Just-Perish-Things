// delegate.cpp
// Matthew Parrish
// CS321, Spring 2015
// Assignment 2
// Write a program to make threads and compute a value from them

// Actual code to create the threads is from race6.cpp, by Glenn G. Chappell
// original documentation below:
//		race6.cpp
//		Glenn G. Chappell
//		4 Feb 2015
//
//		For CS 321 Spring 2015
//		Fix Race Condition: Scoped Lock Guard + Efficiency

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <thread>
using std::thread;
#include <vector>
using std::vector;
#include <queue>
using std::queue;
#include <limits>       // For std::numberic_limits
#include <ios>          // For std::streamsize
#include <cstdlib>
using std::exit;
#include <mutex>
using std::mutex;
using std::lock_guard;
#include "sa2a.h"

const int NUMTHREADS = 6;  
int count;                 
mutex count_mutex; 

queue<int> inputQue;
queue<int> outputQue;

int getInputValue()
{
	int getValueFromUser;
	while(true)
	{
		if(cin >> getValueFromUser && getValueFromUser >= 0)
			return getValueFromUser;
		else
		{
			cout << "Error in input, please enter a positive integer, or 0 to exit: ";

			// Fix cin's state, and purge the rest of the bad input
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		}
	}
}

void run()
{
    int argVal;
	int retVal;
	bool isEmpty = false;
	bool shouldIgo = false;

	// Check to see if the input queue is empty
	{ lock_guard<mutex> count_guard(count_mutex);
	isEmpty = inputQue.empty();
	}

	// If the input queue is NOT empty, then more data to read,
	//     get it, and call sa2a on it
	while(!isEmpty) 
	{
		{ lock_guard<mutex> count_guard(count_mutex);

			// Check for if a context switch had emptied the queue, if queue DOES
				// have at least one item in it, pop it off, set a bool saying that
				// it owns an int to run
			if(!inputQue.empty()) {
				argVal = inputQue.front();
				inputQue.pop();
				shouldIgo = true;
			}
			else
				shouldIgo = false;
		}
		
		// If the thread actually has an int, run the function.
		// Prevents race conditions
		if(shouldIgo)
		{
			// No lock here to allow concurrency
			retVal = sa2a(argVal);
		
			// Lock it to put our found values into the output queue, so it's safe
			{ lock_guard<mutex> count_guard(count_mutex);
				outputQue.push(argVal);
				outputQue.push(retVal);
				isEmpty = inputQue.empty();
			}
		}
	}
}


// Main program
// Spawn a number of threads, each runnning function "run". Join all and
// print result.
int main()
{
    count = 0;

	int inputValue;

	cout << "Delegated Computation" << endl << endl;
	cout << "Enter a positive integer (or 0 to end input): ";
	inputValue = getInputValue();

	while (inputValue > 0)
	{
		inputQue.push(inputValue);

		count++;

		cout << "Enter a positive integer (or 0 to end input): ";
		inputValue = getInputValue();
	}

    cout << "Done getting input."<< endl;
    cout << "GET TO WORK SLAVES. (This program does not condone the use of slave labor)" << endl;

    // Vector of thread objects, not yet holding actual threads
    vector<thread> ts(NUMTHREADS);

    // Spawn all threads
    for (int i = 0; i < NUMTHREADS; ++i)
    {
        try
        {
            ts[i] = thread(run);
        }
        catch(...)
        {
            cout << endl;
            cout << "ERROR: It appears that we can only spawn "
                 << i << " threads on this system." << endl;
            cout << "  Try reducing NUMTHREADS to " << i
                 << " and recompiling." << endl;
            exit(1);
        }
    }

    // Join all spawned threads

	int check = 0;

	while (check < count)
	{
		if(!outputQue.empty())
		{
			cout << "sa2a(" << outputQue.front() << ") = ";
			outputQue.pop();
			cout << outputQue.front() << endl;
			outputQue.pop();
			check++;
		}
	}
	   
	for (int i = 0; i < NUMTHREADS; ++i)
    {
        ts[i].join();
    }

	cout << "Well done slaves! All done now!" << endl;
	return 0;
}

