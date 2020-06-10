#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <atomic>
#include "MethodParameters.h"
#include "Method1.h"
#include "Method2.h"
#include "Method3.h"
#include "Method4.h"
#include "Method5.h"
#include "Method6.h"
#include "TimeStorage.h"

using namespace std;


vector <string> identifierStorage; //global variable to store input
vector <chrono::time_point<struct std::chrono::steady_clock, class std::chrono::duration<__int64, struct std::ratio<1, 1000000000> > >> identifierEntryTime;// time at which identifier is inputed
vector<TimeStorage> metrics1;//used to store the necessary metrics stored in csv  where metric1 stores for method1 and so on
vector<TimeStorage> metrics2;
vector<TimeStorage> metrics3;
vector<TimeStorage> metrics4;
vector<TimeStorage> metrics5;
vector<TimeStorage> metrics6;
atomic <bool> inputEnd; //global variable to indicate end of input intake
atomic <int> totalInputTaken;// global variable to indicate total inputs taken
atomic <int> inputBegin;//global variable to indicate beginning of identifier input
atomic <int> identifierCompletedCount;
vector <int> degradedTime;
vector <int> degradedMethodNumber;
vector <int> identifierPassed; // used to cause dependency among methods
atomic <int> vectorPassedsize;
int* testingMethod = new int[6];//only used to provide same seeds for comparison among models not needs to be part of the actual program. Also holds for testingSeed, allSeedsUsed and controlAllSeedUsed
int* testingSeed = new int[1800];
int** allSeedsUsed = new int* [6];
int degradedMaximum; //maximum degraded time provided in DegradedMode.txt
mutex controlOutput;// to synchronize output call
mutex controlMethod1;// to synch the calling of methods and metrics.
mutex controlMethod2;
mutex controlMethod3;
mutex controlMethod4;
mutex controlMethod5;
mutex controlMethod6;
mutex controlIdentifierStorage;
mutex controlIdentifierEntryTime;
mutex controlThreadClosure;
mutex controlAvailableThread;
mutex controlAllSeedsUsed;
mutex controlIdentifierPassed;

template <typename T>
void retryLogicImplementation(MethodParameters* instance, int numberInputCompleted, int* availableThread, int* threadClosure, int threadNumber, int index, T worker)
{
	int flag = 0, retryCount = 0, expoCount = 0;
	long long int delayTime, delayTimeCutOff;
	chrono::duration<double> timeTillNow;
	bool result = false;
	controlIdentifierStorage.lock();
	string identifier = identifierStorage[numberInputCompleted];
	controlIdentifierStorage.unlock();
	while (result != true)
	{

		if (flag != 0)
		{
			delayTime = (rand() % 10); 
			delayTime += pow(2, expoCount) * 10; //exponential backoff
			controlIdentifierEntryTime.lock();
			timeTillNow = chrono::high_resolution_clock::now() - identifierEntryTime[numberInputCompleted];
			controlIdentifierEntryTime.unlock();
			delayTimeCutOff = ((long long int)(timeTillNow.count() * 1000000)) % (degradedMaximum * 1000000);
			if (delayTimeCutOff + delayTime > degradedMaximum * 1000000.0) //to prevent exponential delaytime from getting too high
			{
				delayTime = degradedMaximum * 1000000.0 - delayTimeCutOff;
				expoCount = -1;
			}
			this_thread::sleep_for(chrono::microseconds(delayTime));
			expoCount++;
		}
		while (instance->getRetryLimit(index) == 2)
		{
			continue;
		}
		flag++;
		retryCount++;
		if (index == 0)
		{
			controlMethod1.lock();
			metrics1[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics1[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod1.unlock();
			if (result == true)
			{
				controlMethod1.lock();
				metrics1[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics1[numberInputCompleted].attempts = retryCount;
				controlMethod1.unlock();
				controlIdentifierPassed.lock();
				identifierPassed[numberInputCompleted] = index + 1; //to update the value that signals the other method that it can begin executing the given identifier.
				controlIdentifierPassed.unlock();
			}
		}
		else if (index == 1)
		{
			controlMethod2.lock();
			metrics2[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics2[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod2.unlock();
			if (result == true)
			{
				controlMethod2.lock();
				metrics2[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics2[numberInputCompleted].attempts = retryCount;
				controlMethod2.unlock();
				controlIdentifierPassed.lock();
				identifierPassed[numberInputCompleted] = index + 1;
				controlIdentifierPassed.unlock();
			}
		}
		else if (index == 2)
		{
			controlMethod3.lock();
			metrics3[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics3[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod3.unlock();
			if (result == true)
			{
				controlMethod3.lock();
				metrics3[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics3[numberInputCompleted].attempts = retryCount;
				controlMethod3.unlock();
				controlIdentifierPassed.lock();
				identifierPassed[numberInputCompleted] = index + 1;
				controlIdentifierPassed.unlock();
			}
		}
		else if (index == 3)
		{
			controlMethod4.lock();
			metrics4[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics4[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod4.unlock();
			if (result == true)
			{
				controlMethod4.lock();
				metrics4[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics4[numberInputCompleted].attempts = retryCount;
				controlMethod4.unlock();
				controlIdentifierPassed.lock();
				identifierPassed[numberInputCompleted] = index + 1;
				controlIdentifierPassed.unlock();
			}
		}
		else if (index == 4)
		{
			controlMethod5.lock();
			metrics5[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics5[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod5.unlock();
			if (result == true)
			{
				controlMethod5.lock();
				metrics5[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics5[numberInputCompleted].attempts = retryCount;
				controlMethod5.unlock();
				controlIdentifierPassed.lock();
				identifierPassed[numberInputCompleted] = index + 1;
				controlIdentifierPassed.unlock();
			}
		}
		else
		{
			controlMethod6.lock();
			metrics6[numberInputCompleted].attemptsStartingTime.push_back(chrono::high_resolution_clock::now());
			result = worker.modeGuesser(identifier, instance);
			metrics6[numberInputCompleted].attemptsEndingTime.push_back(chrono::high_resolution_clock::now());
			controlMethod6.unlock();
			if (result == true)
			{
				controlMethod6.lock();
				metrics6[numberInputCompleted].identifierCompletionTime = chrono::high_resolution_clock::now();
				metrics6[numberInputCompleted].attempts = retryCount;
				controlMethod6.unlock();
				controlIdentifierPassed.lock();
				identifierPassed[numberInputCompleted] = index + 1;
				controlIdentifierPassed.unlock();
			}
		}
	}
	controlOutput.lock();
	identifierCompletedCount++;
	cout << "Did the program gave the result: " << result << " -> " << identifierCompletedCount << endl; //whether the program was able to sufficiently apply retry logic within 11 retries 
	cout << "Amount of retries required for \"" << identifier << "\" on the Method" << index + 1 << " are " << retryCount << " on the thread number " << threadNumber << endl;
	controlOutput.unlock();
	controlThreadClosure.lock();
	threadClosure[threadNumber] = 1;
	controlThreadClosure.unlock();
	controlAvailableThread.lock();
	availableThread[threadNumber] = 1;
	controlAvailableThread.unlock();
	return;
}

template <typename T>
void randomSeedSetUp(MethodParameters* instance, int numberInputCompleted, int* availableThread, int* threadClosure, int threadNumber, int index, T worker)
{
	/*  //This is used to provide unique seed to thread for randomness
	FILETIME fileTime;
	GetSystemTimeAsFileTime(&fileTime);
	ULARGE_INTEGER theTime;
	theTime.LowPart = fileTime.dwLowDateTime;
	theTime.HighPart = fileTime.dwHighDateTime;
	__int64 fileTime64Bit = theTime.QuadPart;
	srand((unsigned)fileTime64Bit); //used to get different seeds for each method for randomness
	retryLogicImplementation<T>(instance, numberInputCompleted, availableThread, threadClosure, threadNumber,index,worker);
	return;
	*/
	if (numberInputCompleted == 1799) // this is only to get same seeds for all model for comparison
	{
		srand(1799);
		controlAllSeedsUsed.lock();
		allSeedsUsed[index][1799] = 1;
		controlAllSeedsUsed.unlock();
	}
	else
	{
		srand((testingMethod[index] * numberInputCompleted) % 1799);
		controlAllSeedsUsed.lock();
		allSeedsUsed[index][(testingMethod[index] * numberInputCompleted) % 1799] = 1;
		controlAllSeedsUsed.unlock();
	}

	retryLogicImplementation<T>(instance, numberInputCompleted, availableThread, threadClosure, threadNumber, index, worker);
}


void particularMethodExecutor(MethodParameters* instance, int index)
{
	int threadNumber = 0, numberInputCompleted = 0, loopItem, breakLoop = 0;
	int* threadClosure = new int[10];
	int* availableThread = new int[10];
	thread arrayOfThreads[10];
	for (loopItem = 0; loopItem < 10; loopItem++)
	{
		threadClosure[loopItem] = 0;
		availableThread[loopItem] = 1;
	}
	while (inputBegin != 1)
	{
		this_thread::sleep_for(chrono::milliseconds(100 * index));
	}

	while (!inputEnd || !(totalInputTaken == numberInputCompleted))
	{
		controlIdentifierStorage.lock();
		if (numberInputCompleted + 1 <= (int)identifierStorage.size())
		{
			controlIdentifierStorage.unlock();
			while (true) //this loop is what relates the methods together so that unless the identifierPassed vector doesnot gets a certain value it doesnot allow the thread to begin execution of this method.
			{
				breakLoop = 0;
				for (loopItem = 0; loopItem < vectorPassedsize; loopItem++)
				{
					controlIdentifierPassed.lock();
					if (identifierPassed[loopItem] == index)
					{
						identifierPassed[loopItem] = -1;
						controlIdentifierPassed.unlock();
						breakLoop = 1;
						break;
					}
					controlIdentifierPassed.unlock();

				}
				if (breakLoop == 1)
				{
					break;
				}
			}
			controlAvailableThread.lock();
			availableThread[threadNumber] = 0;
			controlAvailableThread.unlock();
			if (index == 0)
			{
				Method1 worker;
				arrayOfThreads[threadNumber] = thread(randomSeedSetUp<Method1>, instance, loopItem, availableThread, threadClosure, threadNumber, index, worker);
			}
			else if (index == 1)
			{
				Method2 worker;
				arrayOfThreads[threadNumber] = thread(randomSeedSetUp<Method2>, instance, loopItem, availableThread, threadClosure, threadNumber, index, worker);
			}
			else if (index == 2)
			{
				Method3 worker;
				arrayOfThreads[threadNumber] = thread(randomSeedSetUp<Method3>, instance, loopItem, availableThread, threadClosure, threadNumber, index, worker);
			}
			else if (index == 3)
			{
				Method4 worker;
				arrayOfThreads[threadNumber] = thread(randomSeedSetUp<Method4>, instance, loopItem, availableThread, threadClosure, threadNumber, index, worker);
			}
			else if (index == 4)
			{
				Method5 worker;
				arrayOfThreads[threadNumber] = thread(randomSeedSetUp<Method5>, instance, loopItem, availableThread, threadClosure, threadNumber, index, worker);
			}
			else if (index == 5)
			{
				Method6 worker;
				arrayOfThreads[threadNumber] = thread(randomSeedSetUp<Method6>, instance, loopItem, availableThread, threadClosure, threadNumber, index, worker);
			}
			numberInputCompleted++;
			while (true)
			{
				breakLoop = 0;
				for (loopItem = 0; loopItem < 10; loopItem++)
				{
					controlAvailableThread.lock();
					if (availableThread[loopItem] == 1)
					{
						controlAvailableThread.unlock();
						threadNumber = loopItem;
						breakLoop = 1;
						break;
					}
					controlAvailableThread.unlock();
				}
				if (breakLoop == 1)
				{
					break;
				}
			}
			controlThreadClosure.lock();
			if (threadClosure[threadNumber] != 0)
			{
				arrayOfThreads[threadNumber].join();
				threadClosure[threadNumber] = 0;
				controlThreadClosure.unlock();
			}
			else
			{
				controlThreadClosure.unlock();
			}
			this_thread::sleep_for(chrono::milliseconds(100));
		}
		else
		{
			while (numberInputCompleted + 1 > (int)identifierStorage.size())
			{
				controlIdentifierStorage.unlock();
				if (inputEnd)
				{
					controlIdentifierStorage.lock();
					break;
				}
				this_thread::sleep_for(chrono::milliseconds(100 * index));
				controlIdentifierStorage.lock();
			}
			controlIdentifierStorage.unlock();
		}
	}
	for (loopItem = 0; loopItem < 10; loopItem++)
	{
		controlAvailableThread.lock();
		controlThreadClosure.lock();
		if (threadClosure[loopItem] != 0 || availableThread[loopItem] == 0)
		{
			controlThreadClosure.unlock();
			controlAvailableThread.unlock();
			arrayOfThreads[loopItem].join();
		}
		else
		{
			controlThreadClosure.unlock();
			controlAvailableThread.unlock();
		}

	}
	return;
}

int stringToInt(string degradedSec, int size)
{
	int integer = 0, loopItem;
	for (loopItem = 0; loopItem < size; loopItem++)
	{
		integer = integer * 10 + (degradedSec[loopItem] - '0');
	}
	return integer;
}


void setDegradationTime(MethodParameters* instance, int degradedPeriod, int index)
{
	while (inputBegin != 1)
	{
		continue;
	}
	instance->setMode(index, 1);
	int delay = (rand() * (index + 1)) % 1800;
	cout << index << "->" << delay << endl;
	this_thread::sleep_for(chrono::seconds(delay));
	instance->setMode(index, 2);
	this_thread::sleep_for(chrono::seconds(degradedPeriod));
	instance->setMode(index, 1);
	return;
}


void retryLimitingHandler(MethodParameters* instance, int index)
{
	while (instance->getCurrentState(index) == 0)
	{
		this_thread::sleep_for(chrono::seconds(1));
		instance->setRetryLimit(index, 0);
	}
	return;
}

int main()
{
	int countInput = 0, flag = 0, loopItem, innerLoop1, innerLoop2, degradedInt;
	string inputFile, degradedSec, identifier;
	char methodNumber;
	vector <string> identifierCollector;
	inputBegin = 0;
	totalInputTaken = 0;
	identifierCompletedCount = 0;
	inputEnd = false;
	MethodParameters* instance = new MethodParameters();
	thread retryLimitingThread[6];
	thread particularMethodImplementation[6];
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		retryLimitingThread[loopItem] = thread(retryLimitingHandler, instance, loopItem);
	}
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		particularMethodImplementation[loopItem] = thread(particularMethodExecutor, instance, loopItem);
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		allSeedsUsed[loopItem] = new int[1800];
		for (innerLoop1 = 0; innerLoop1 < 1800; innerLoop1++)
		{
			allSeedsUsed[loopItem][innerLoop1] = 0;
		}
	}
	ifstream fileInstance;
	fileInstance.open("DegradedMode.txt");
	if (fileInstance.is_open())
	{
		while (getline(fileInstance, inputFile))
		{
			flag = 0;
			degradedSec = "";
			for (loopItem = 0; loopItem < inputFile.length(); loopItem++)
			{
				if (flag == 2 && (inputFile[loopItem] > 57 || inputFile[loopItem] < 48))
				{
					break;
				}
				else if (flag == 2)
				{
					degradedSec = degradedSec + inputFile[loopItem];
				}
				else if (flag == 1 && inputFile[loopItem] < 58 && inputFile[loopItem] >47)
				{
					flag = 2;
					degradedSec = degradedSec + inputFile[loopItem];
				}
				else if (flag == 0 && inputFile[loopItem] < 58 && inputFile[loopItem] >47)
				{
					flag = 1;
					methodNumber = inputFile[loopItem];
				}
				else
				{
					continue;
				}
			}
			degradedInt = stringToInt(degradedSec, degradedSec.length());
			degradedMaximum = max(degradedMaximum, degradedInt);
			cout << degradedSec << " -> " << methodNumber << endl;
			degradedTime.push_back(degradedInt);
			degradedMethodNumber.push_back(methodNumber - '1');
		}
		fileInstance.close();
	}
	else
	{

		cout << "Unable to open the degradation setup file";
	}
	srand(time(0));
	thread degradedModeSetup[6];
	cout << degradedTime.size() << endl;
	for (loopItem = 0; loopItem < degradedTime.size(); loopItem++)
	{
		degradedModeSetup[loopItem] = thread(setDegradationTime, instance, degradedTime[loopItem], degradedMethodNumber[loopItem]);
	}
	fileInstance.open("InputFile.txt");
	getline(fileInstance, inputFile);
	for (loopItem = 0; loopItem < inputFile.length(); loopItem++)
	{
		identifier = "";
		while (inputFile[loopItem] != ' ' && loopItem != inputFile.length())
		{
			identifier = identifier + inputFile[loopItem];
			loopItem++;
		}
		identifierCollector.push_back(identifier);
		cout << identifier << " ";
	}
	for (loopItem = 0; loopItem < 1800; loopItem++)
	{
		testingSeed[loopItem] = loopItem;
	}
	testingMethod[0] = 1;
	testingMethod[1] = 2;
	testingMethod[2] = 3;
	testingMethod[3] = 5;
	testingMethod[4] = 11;
	testingMethod[5] = 13;
	cout << endl;
	flag = 0;
	while (countInput != identifierCollector.size())
	{
		controlMethod1.lock();
		metrics1.push_back(*new TimeStorage);
		controlMethod1.unlock();
		controlMethod2.lock();
		metrics2.push_back(*new TimeStorage);
		controlMethod2.unlock();
		controlMethod3.lock();
		metrics3.push_back(*new TimeStorage);
		controlMethod3.unlock();
		controlMethod4.lock();
		metrics4.push_back(*new TimeStorage);
		controlMethod4.unlock();
		controlMethod5.lock();
		metrics5.push_back(*new TimeStorage);
		controlMethod5.unlock();
		controlMethod6.lock();
		metrics6.push_back(*new TimeStorage);
		controlMethod6.unlock();
		controlIdentifierEntryTime.lock();
		identifierEntryTime.push_back(chrono::high_resolution_clock::now());
		controlIdentifierEntryTime.unlock();
		controlIdentifierStorage.lock();
		identifierStorage.push_back(identifierCollector[countInput]);
		controlIdentifierStorage.unlock();
		controlIdentifierPassed.lock();
		identifierPassed.push_back(0);
		vectorPassedsize++;
		controlIdentifierPassed.unlock();
		countInput++;
		if (flag == 0)
		{
			inputBegin = 1;
			flag = 1;
		}
		this_thread::sleep_for(chrono::microseconds(999850)); // to get a unique identifier input after every one second
	}
	totalInputTaken = countInput;
	inputEnd = true;
	for (loopItem = 0; loopItem < degradedTime.size(); loopItem++)
	{
		degradedModeSetup[loopItem].join();
	}
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		particularMethodImplementation[loopItem].join();
	}
	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		instance->changeState(loopItem);
		retryLimitingThread[loopItem].join();
	}
	auto endTime = chrono::high_resolution_clock::now();
	chrono::duration<double> finalDuration = endTime - identifierEntryTime[0];
	cout << "Total time taken by the application is " << finalDuration.count() << " seconds" << endl;
	cout << identifierCompletedCount << endl;

	for (loopItem = 0; loopItem < 6; loopItem++)
	{
		for (innerLoop1 = 0; innerLoop1 < 1800; innerLoop1++)
		{
			if (allSeedsUsed[loopItem][innerLoop1] == 0)
			{
				cout << loopItem << "->" << innerLoop1 << endl;
			}

		}
	}

	ofstream outputCSV;
	outputCSV.open("retryStats.csv");
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics1[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics1[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics1[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics1[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics1[innerLoop1].attemptsEndingTime[innerLoop2] - metrics1[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics2[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics2[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics2[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics2[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics2[innerLoop1].attemptsEndingTime[innerLoop2] - metrics2[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics3[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics3[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics3[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics3[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics3[innerLoop1].attemptsEndingTime[innerLoop2] - metrics3[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics4[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics4[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics4[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics4[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics4[innerLoop1].attemptsEndingTime[innerLoop2] - metrics4[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics5[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics5[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics5[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics5[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics5[innerLoop1].attemptsEndingTime[innerLoop2] - metrics5[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
	outputCSV << endl;
	outputCSV << "identifier,retries,identifier time,retrytime" << endl;
	for (innerLoop1 = 0; innerLoop1 < totalInputTaken; innerLoop1++)
	{
		chrono::duration<double> timeDiff = metrics6[innerLoop1].identifierCompletionTime - identifierEntryTime[innerLoop1];
		outputCSV << identifierStorage[innerLoop1] << "," << metrics6[innerLoop1].attempts << "," << timeDiff.count() << ",";
		int vectorSize = metrics6[innerLoop1].attemptsStartingTime.size();
		for (innerLoop2 = 0; innerLoop2 < vectorSize; innerLoop2++)
		{
			timeDiff = metrics6[innerLoop1].attemptsStartingTime[innerLoop2] - identifierEntryTime[innerLoop1];
			outputCSV << timeDiff.count() << ",";
			timeDiff = metrics6[innerLoop1].attemptsEndingTime[innerLoop2] - metrics6[innerLoop1].attemptsStartingTime[innerLoop2];
			outputCSV << timeDiff.count() << ",";
		}
		outputCSV << endl;
	}
}