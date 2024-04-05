//
// cards.cpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   A program to simulate the Minotaur's "Thank you" card fiasco
//


/* * * Headers * * */

#include "../linkedlist.hpp"
#include "bag.hpp"

#include <iostream>
#include <vector>

#include <thread>
#include <atomic>

#include <stdlib.h>
#include <time.h>


#define SEARCH_PERCENT_CHANCE	2


/* * * Constant Definitions * * */

const int32_t NUM_THREADS = 4;
const int32_t NUM_GIFTS = 500000;


/* * * Forward Declarations * * */

void servant(int32_t id, Bag* bag, LinkedList<Gift>* ll, std::atomic<int32_t>* numGifts, std::atomic<int32_t>* numCards, std::atomic<int32_t>* numSearches);


/* * * Entry Point * * */

int main(int argc, char* argv[])
{
	Bag bag;
	bag.fill(NUM_GIFTS);
	
	LinkedList<Gift> ll;
	
	std::atomic<int32_t> numGifts = 0;
	std::atomic<int32_t> numCards = 0;
	std::atomic<int32_t> numSearches = 0;
	
	std::vector<std::thread> thrds;
	
	// Spawn threads
	//
	for (int32_t threadID = 1; threadID <= NUM_THREADS; threadID++)
	{
		thrds.push_back(std::thread(servant, threadID, &bag, &ll, &numGifts, &numCards, &numSearches));
	}
	
	// Sync with threads
	//
	for (auto it = thrds.begin(); it != thrds.end(); it++)
	{
		(*it).join();
	}
	
	std::cout << "\nTotal number of gifts:            " << NUM_GIFTS << "\n";
	std::cout << "Number of gifts removed from bag: " << numGifts.load() << "\n";
	std::cout << "Number of thankyou cards written: " << numCards.load() << "\n";
	std::cout << "Number of searches:               " << numSearches.load() << "\n\n";
	
	return 0;
}


/* * * Function Definitions * * */

bool insertGiftIntoChain(Bag* bag, LinkedList<Gift>* ll)
{
	Gift gift = bag->getGift();
		
	if (gift.tag)
	{
		ll->insert(gift);
		
		return true;
	}
	
	return false;
}

bool writeThankyouCard(LinkedList<Gift>* ll)
{
	Gift gift;

	try
	{
		gift = ll->getHead();
	}
	catch(...)
	{
		return false;
	}
	
	if (gift.tag)
	{
		return true;
	}
	
	return false;
}


//std::mutex mylock;
void findGiftForMinotaur(LinkedList<Gift>* ll)
{
	Gift gift(rand() % NUM_GIFTS + 1);
	
	bool found = ll->find(gift);
	
	//mylock.lock();
	//if (found) std::cout << "Found the gift! #" << gift.tag << "\n";
	//mylock.unlock();
}

bool minotaurIsLookingForGift()
{
	if ((rand() % 100) < SEARCH_PERCENT_CHANCE)
	{
		return true;
	}
	
	return false;
}


// Performs the tasks carried out by the Minotaur's servants
//
//   1. Remove gifts from the bag and add them to the linked list.
//   2. Write a thankyou card for a gift and remove the gift from the list.
//   3. Check whether a particular gift is in the list at the Minotaur's request.
//
void servant(int32_t id, Bag* bag, LinkedList<Gift>* ll, std::atomic<int32_t>* numGifts, std::atomic<int32_t>* numCards, std::atomic<int32_t>* numSearches)
{
	srand(time(0) - (id * 10));
	
	while (true)
	{
		if (insertGiftIntoChain(bag, ll))
		{
			(*numGifts)++;
		}
		else if (ll->empty())
		{
			// All done!
			break;
		}
		
		if (minotaurIsLookingForGift())
		{
			findGiftForMinotaur(ll);
			
			(*numSearches)++;
		}
		else
		{
			if (writeThankyouCard(ll))
			{
				(*numCards)++;
			}
		}
	}
}
