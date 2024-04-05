//
// bag.hpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   Declares a type for the Minotuar's gift bag
//

#ifndef BAG_HPP
#define BAG_HPP

#include <vector>
#include <atomic>


struct Gift
{
	int32_t tag;
	
	Gift(int32_t t);
	Gift(Gift& g);
	Gift();
	
	bool operator==(const Gift& t)
	{
		return (this->tag == t.tag);
	}
	
	bool operator>(const Gift& t)
	{
		return (this->tag > t.tag);
	}
};


class Bag
{
public:

	void fill(int32_t numGifts);
	
	Gift getGift();

private:

	std::vector<int32_t> giftTags;
	
	std::atomic<int32_t> top;
};


#endif
