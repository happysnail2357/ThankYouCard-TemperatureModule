//
// bag.cpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   Defines a type for the Minotuar's gift bag
//


#include "bag.hpp"

#include <algorithm>
#include <random>
#include <time.h>


// Initialize the bag with "gifts"
//
void Bag::fill(int32_t numGifts)
{
	this->giftTags.reserve(numGifts);
	
	for (int32_t i = 1; i <= numGifts; i++)
	{
		this->giftTags.push_back(i);
	}

	auto rng = std::default_random_engine{ static_cast<long unsigned int>(time(0)) };
	std::shuffle(this->giftTags.begin(), this->giftTags.end(), rng);
	
	this->top.store(0);
}

// Atomically get a gift from the bag
//
Gift Bag::getGift()
{
	int32_t giftNum = this->top.fetch_add(1);
	
	if (giftNum < giftTags.size())
	{
		return Gift(this->giftTags[giftNum]);
	}
	else
	{
		return Gift();
	}
}


// Gift constructors

Gift::Gift(int32_t t)
{
	this->tag = t;
}

Gift::Gift(Gift& g)
{
	this->tag = g.tag;
}

Gift::Gift()
{
	this->tag = 0;
}
