//
// linkedlist.cpp
//
//   by Paul Puhnaty - 4/5/2024
//
//   Defines a concurrent linked list type
//


#include <exception>


template<class T>
void LinkedList<T>::insert(T t)
{
	this->listLock.lock();
	
	if (this->head)
	{
		this->head->lock();
		
		if (this->head->data > t)
		{
			node* ins = new node(t, this->head);
			
			this->head = ins;
			
			this->head->next->unlock();
			this->listLock.unlock();
		}
		else
		{
			this->listLock.unlock();
			
			node* before = this->head;
			node* curr = this->head->next;
			
			while (curr)
			{
				curr->lock();
				
				if (curr->data > t)
				{
					break;
				}
				
				before->unlock();
				before = curr;
				curr = curr->next;
			}
			
			node* ins = new node(t, curr);
		
			before->next = ins;
			
			before->unlock();
			if (curr) curr->unlock();
		}
	}
	else
	{
		this->head = new node(t, nullptr);
		
		this->listLock.unlock();
	}
	
	this->size++;
}
	
template<class T>
T LinkedList<T>::getHead()
{
	this->listLock.lock();
	
	if (this->head)
	{
		this->head->lock();
		
		T ret = this->head->data;
		
		node* after = this->head->next;
		
		delete this->head;
		
		this->head = after;
		
		this->size--;
		
		this->listLock.unlock();
		
		return ret;
	}
	
	this->listLock.unlock();
	
	throw std::runtime_error("LinkedList is empty");
}

template<class T>
bool LinkedList<T>::find(T t)
{
	this->listLock.lock();
	
	if (this->head)
	{
		this->head->lock();
		
		this->listLock.unlock();
		
		if (this->head->data == t)
		{
			this->head->unlock();
			
			return true;
		}
	}
	else
	{
		this->listLock.unlock();
		
		return false;
	}
	
	node* before = this->head;
	node* curr = this->head->next;
	
	while (curr)
	{
		curr->lock();
		before->unlock();
		
		if (curr->data == t)
		{
			curr->unlock();
			
			return true;
		}
		
		before = curr;
		curr = curr->next;
	}
	
	before->unlock();
	
	return false;
}

template<class T>
void LinkedList<T>::remove(T t)
{
	this->listLock.lock();
	
	if (this->head)
	{
		this->head->lock();
		
		if (this->head->data == t)
		{
			node* after = this->head->next;
			
			delete this->head;
			
			this->head = after;
			
			this->listLock.unlock();
			
			return;
		}
		
		this->listLock.unlock();
	}
	else
	{
		this->listLock.unlock();
		
		return;
	}
	
	node* before = this->head;
	node* curr = this->head->next;
	
	while (curr)
	{
		curr->lock();
		
		if (curr->data == t)
		{
			node* after = curr->next;
			
			delete curr;
			
			before->next = after;
			
			before->unlock();
			
			this->size--;
			
			return;
		}
		
		before->unlock();
		before = curr;
		curr = curr->next;
	}
	
	before->unlock();
}

template<class T>
int LinkedList<T>::getSize()
{
	return this->size.load();
}

template<class T>
bool LinkedList<T>::empty()
{
	this->listLock.lock();
	bool e = (this->head == nullptr);
	this->listLock.unlock();
	return e;
}

template<class T>
LinkedList<T>* LinkedList<T>::detach()
{
	this->listLock.lock();
	
	if (this->head) this->head->lock();
	
	LinkedList<T>* copy = new LinkedList<T>(this);
	
	if (this->head) this->head->unlock();
	
	this->head = nullptr;
	this->size.store(0);
	
	this->listLock.unlock();
	
	return copy;
}


template<class T>
LinkedList<T>::~LinkedList()
{
	while (this->head)
	{
		node* rem = this->head;
		
		this->head = this->head->next;
		
		delete rem;
	}
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>* ll)
{
	this->head = ll->head;
	this->size.store(ll->size.load());
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>& ll)
{
	this->head = ll.head;
	this->size.store(ll.size.load());
}

template<class T>
LinkedList<T>::LinkedList()
{
}

