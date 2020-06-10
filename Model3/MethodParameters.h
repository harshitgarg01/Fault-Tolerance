#pragma once
#include <cmath>
#include <Windows.h>
#include <time.h>
#include <mutex>

using namespace std;



class MethodParameters
{
private:
	int* retryLimit = new int[6];
	int* state = new int[6];
	int* mode = new int[6];
	mutex synchMethods;
public:
	MethodParameters()
	{
		int loopItem;
		for (loopItem = 0; loopItem < 6; loopItem++)
		{
			this->retryLimit[loopItem] = 0;
			this->state[loopItem] = 0;
			this->mode[loopItem] = 0;
		}

	}
	int getRetryLimit(int index)
	{
		lock_guard<mutex> guard(synchMethods);
		return this->retryLimit[index];
	}
	void setRetryLimit(int index, int value)
	{
		lock_guard<mutex> guard(synchMethods);
		this->retryLimit[index] = value;
	}
	int getCurrentState(int index)
	{
		lock_guard<mutex> guard(synchMethods);
		return this->state[index];
	}
	void changeState(int index)
	{
		lock_guard<mutex> guard(synchMethods);
		this->state[index] = 1;
	}
	int getMode(int index)
	{
		lock_guard<mutex> guard(synchMethods);
		return this->mode[index];
	}
	void setMode(int index, int value)
	{
		lock_guard<mutex> guard(synchMethods);
		this->mode[index] = value;
	}
};