//
// tempModule.hpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   A module that handles thermometers, and creates reports based on their readings
//


#ifndef ROVER_TEMP_MODULE_HEADER
#define ROVER_TEMP_MODULE_HEADER

#include "../linkedlist.hpp"

#include <atomic>

#define TEMP_MODULE_NUM_THREADS				8
#define MILLISECONDS_PER_SIMULATED_MINUTE	50
#define INTERVAL_LENGTH						10
#define NUM_MAX_MIN_TEMPS					5


class tempReading
{
public:

	int threadID{};
	int degrees{};
	int timestamp{};
	
	bool operator==(const tempReading& t)
	{
		if (this->threadID == t.threadID)
			if (this->timestamp == t.timestamp)
				if (this->degrees == t.degrees)
					return true;
		
		return false;
	}
	
	bool operator>(const tempReading& t)
	{
		if (this->timestamp == t.timestamp)
		{
			return (this->degrees > t.degrees);
		}
		
		return (this->timestamp > t.timestamp);
	}
	bool operator<(const tempReading& t)
	{
		if (this->timestamp == t.timestamp)
		{
			return (this->degrees < t.degrees);
		}
		
		return (this->timestamp < t.timestamp);
	}
	
	tempReading() {}
	tempReading(int id, int deg, int timestamp)
	{
		this->threadID = id;
		this->degrees = deg;
		this->timestamp = timestamp;
	}
	tempReading(tempReading& t)
	{
		this->threadID = t.threadID;
		this->degrees = t.degrees;
		this->timestamp = t.timestamp;
	}
};


class tempModule
{
public:

	static void run();

private:
	
	static void asyncRead(int id, LinkedList<tempReading>* sharedMem, std::atomic<int>* clock, bool* kill);
	
	static void handleReport(LinkedList<tempReading>* data, int time);
};


#endif
