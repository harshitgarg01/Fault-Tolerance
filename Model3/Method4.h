#pragma once
#include <cmath>
#include <chrono>
#include <thread>
#include "MethodParameters.h"

using namespace std;



class Method4
{
public:
	bool modeGuesser(string identifier, MethodParameters* instance) 
	{
		this_thread::sleep_for(chrono::milliseconds(20 + rand() % 20));
		int check = instance->getMode(3);
		if (check == 0)
		{
			int chooser = rand() % 2;
			if (chooser == 0)
			{
				return this->normalMode(identifier, instance);
			}
			else
			{
				return this->degradedMode(identifier);
			}
		}
		else if (check == 1)
		{
			return this->normalMode(identifier, instance);
		}
		else
		{
			return this->degradedMode(identifier);
		}

	}
private:
	bool normalMode(string identifier, MethodParameters* instance)
	{
		int chooser = rand() % 2;
		if (chooser == 0)
		{
			return false;
		}
		else
		{
			int value = instance->getRetryLimit(3);
			if (value >= 2)
			{
				return false;
			}
			else
			{
				instance->setRetryLimit(3, value + 1);
				return true;
			}

		}
	}
	bool degradedMode(string identifier)
	{
		return false;
	}
};