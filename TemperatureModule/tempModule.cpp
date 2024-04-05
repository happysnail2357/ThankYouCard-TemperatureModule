//
// tempModule.cpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   A module that handles thermometers, and creates reports based on their readings
//


#include "tempModule.hpp"
#include "thermometer.hpp"

#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <time.h>


void waitForInput(bool* input)
{
	char in;
	
	std::cin.read(&in, 1);
	
	*input = true;
}

void printTime(int time)
{
	int hour = time / 60;
	int minute = time % 60;
	
	std::cout << hour << ":" << ((minute < 10) ? "0" : "") << minute;
	std::cout.flush();
}


void tempModule::run()
{
	std::cout << "\ntempModule starting up...\n\n";
	
	bool kill = false;
	bool input = false;
	LinkedList<tempReading> sharedMem;
	
	std::atomic<int> clock = 1;
	
	std::vector<std::thread> thrds;
	
	for (int threadID = 1; threadID <= TEMP_MODULE_NUM_THREADS; threadID++)
	{
		thrds.push_back(std::thread(asyncRead, threadID, &sharedMem, &clock, &kill));
		std::cout << "Initializing thermometer #" << threadID << "\n";
	}
	
	std::cout << "\nTime: 0:00";
	std::cout.flush();
	
	std::thread waitThread(waitForInput, &input);
	
	while (true)
	{
		if (input)
		{
			std::cout << "\ntempModule shutting down...\n\n";
			
			kill = true;
			
			waitThread.join();
			
			// Sync with thermometer threads
			//
			for (auto it = thrds.begin(); it != thrds.end(); it++)
			{
				(*it).join();
			}
			
			return;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_PER_SIMULATED_MINUTE));
		
		int time = clock.fetch_add(1);
		
		if (time % 60 == 0)
		{
			LinkedList<tempReading>* data = sharedMem.detach();
			
			std::thread printer(handleReport, data, time);
			
			printer.detach();
		}
		else if (time % 15 == 0)
		{
			std::cout << " ";
			printTime(time);
		}
	}
}


void tempModule::asyncRead(int id, LinkedList<tempReading>* sharedMem, std::atomic<int>* clock, bool* kill)
{
	srand(time(0) - (id * 10));
	
	while (*kill == false)
	{
		tempReading currTemp(id, thermometer_read(), clock->load());
		
		sharedMem->insert(currTemp);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_PER_SIMULATED_MINUTE));
	}
}

void tempModule::handleReport(LinkedList<tempReading>* data, int time)
{
	std::cout << "\n\nHOURLY REPORT\n  _____________________________________\n | Number of temperature readings: " << data->getSize() << "\n";
	
	int minTemps[NUM_MAX_MIN_TEMPS]{ THERMOMETER_MAX_TEMP + 1 };
	int maxTemps[NUM_MAX_MIN_TEMPS]{ THERMOMETER_MIN_TEMP - 1 };
	int minSlot = NUM_MAX_MIN_TEMPS - 1;
	int maxSlot = NUM_MAX_MIN_TEMPS - 1;
	
	int intervalMaxDiff = 0;
	int intervalMaxDiffTime = -1;
	int intervalNext = 0;
	
	int interval[INTERVAL_LENGTH];
	
	
	int currentTime = -1;
	
	int tempSum = 0;
	int tempCount = 0;
	
	int totalSum = 0;
	int totalCount = 0;
	
	tempReading curr;
	
	while (!data->empty())
	{
		curr = data->getHead();
		
		if (curr.timestamp != currentTime)
		{
			if (currentTime >= 0)
			{
				int averageTemp = tempSum / tempCount;
				tempSum = 0;
				tempCount = 0;
				
				for (int i = minSlot; i < NUM_MAX_MIN_TEMPS; i++)
				{
					if (minTemps[i] > averageTemp)
					{
						minTemps[i] = averageTemp;
						
						if (minSlot) minSlot--;
						break;
					}
				}
				
				for (int i = maxSlot; i < NUM_MAX_MIN_TEMPS; i++)
				{
					if (maxTemps[i] < averageTemp)
					{
						maxTemps[i] = averageTemp;
						
						if (maxSlot) maxSlot--;
						break;
					}
				}
				
				interval[intervalNext % INTERVAL_LENGTH] = averageTemp;
				intervalNext++;
				
				if (intervalNext >= INTERVAL_LENGTH)
				{
					int min, max;
					
					min = max = interval[0];
					
					for (int i = 1; i < INTERVAL_LENGTH; i++)
					{
						if (interval[i] < min)
						{
							min = interval[i];
						}
						else if (interval[i] > max)
						{
							max = interval[i];
						}
					}
					
					int diff = max - min;
					
					if (diff > intervalMaxDiff)
					{
						intervalMaxDiff = diff;
						intervalMaxDiffTime = currentTime;
					}
				}
			}
			
			currentTime = curr.timestamp;
		}
		
		tempSum += curr.degrees;
		tempCount++;
		
		totalSum += curr.degrees;
		totalCount++;
	}
	
	int avgTemp = totalSum / totalCount;
	
	std::cout << " | Low Temps:  ";
	
	for (int i = 0; i < NUM_MAX_MIN_TEMPS; i++)
	{
		if (i)
		{
			std::cout << ", ";
		}
		
		std::cout << minTemps[i];
	}
	
	std::cout << "\n | High Temps: ";
	
	for (int i = 0; i < NUM_MAX_MIN_TEMPS; i++)
	{
		if (i)
		{
			std::cout << ", ";
		}
		
		std::cout << maxTemps[i];
	}
	
	std::cout << "\n | Avg Temp:   " << avgTemp;
	
	std::cout << "\n | Largest Change: " << intervalMaxDiff << " from ";
	printTime(intervalMaxDiffTime - 10);
	std::cout << " to ";
	printTime(intervalMaxDiffTime);
	
	std::cout << "\n\nTime: ";
	printTime(time);
	
	std::cout.flush();
	
	delete data;
}

