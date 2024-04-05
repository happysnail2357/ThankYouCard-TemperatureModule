//
// rover.cpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   Simulates a Mars rover that measures temperature
//


#include "tempModule.hpp"

#include <thread>


int main()
{
	std::thread tempModThrd(tempModule::run);
	
	//
	// .
	// .
	// .
	//
	// Super cool Mars rover stuff
	//
	// .
	// .
	// .
	//
	
	tempModThrd.join();
	
	return 0;
}