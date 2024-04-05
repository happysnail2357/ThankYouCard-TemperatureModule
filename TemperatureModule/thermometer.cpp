//
// thermometer.cpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   An interface for an imaginary thermometer
//


#include "thermometer.hpp"

#include <stdlib.h>


int thermometer_read()
{
	return (rand() % (THERMOMETER_MAX_TEMP - THERMOMETER_MIN_TEMP + 1)) + THERMOMETER_MIN_TEMP;
}

