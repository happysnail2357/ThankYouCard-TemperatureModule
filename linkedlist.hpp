//
// linkedlist.hpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   Declares a concurrent linked list type
//

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <mutex>
#include <atomic>


template<class T>
class LinkedList
{
public:

	void insert(T t);
	
	T getHead();
	
	bool find(T t);
	
	void remove(T t);
	
	int getSize();
	
	bool empty();
	
	LinkedList<T>* detach();
	
	LinkedList();
	LinkedList(LinkedList& ll);
	~LinkedList();
	
private:

	struct node
	{
		T data;
		struct node* next;
		
		void lock()
		{
			this->nodeLock.lock();
		}
		void unlock()
		{
			this->nodeLock.unlock();
		}
		
		node(T t, node* n)
		{
			this->data = t;
			this->next = n;
		}
		
	private:
	
		std::mutex nodeLock;
	};

	std::mutex listLock;

	node* head = nullptr;
	
	std::atomic<int> size = 0;
	
	
	LinkedList(LinkedList* ll);
};


#include "linkedlist.cpp"

#endif
