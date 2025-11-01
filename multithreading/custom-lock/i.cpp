#include<bits/stdc++.h>
#include<condition_variable>
using namespace std;


class Lock 
{
	public:
	std::atomic_flag flag{false};
	void lock() 
	{
		while(flag.test_and_set()) 
		{
			std::this_thread::yield();
			//That’s what yield() does — your thread steps aside momentarily so others can proceed.
		}
	}
	
	void unlock() 
	{
		flag.clear();
	}
};



class Queue 
{
	public:
	int capac;
	int curSize;
	Lock lock;
	condition_variable_any cv;
	class Node 
	{
		public:
		int value;
		Node* next;
		Node() 
		{
			next = NULL;
		}
	};
	Node* tail;
	Node* head;
	Queue(int limit) 
	{
		capac = limit;
		head = NULL;
		tail = NULL;
		curSize = 0;
	}
	bool push(int val) 
	{
		lock.lock();
		if(curSize==capac) 
		{
			lock.unlock();
			return false;
		}
		Node* cur = new Node();
		cur->value = val;
		if(head == NULL) 
		{
			head = cur;
		}
		else 
		{
			
			tail->next = cur;
		}
		curSize ++ ;
		tail = cur;
		lock.unlock();
		return true;
	}
	
	int pop() 
	{
		lock.lock();
		if(curSize == 0) 
		{
			lock.unlock();
			return -1;
		}
		int cur = head->value;
		Node* curNode = head;
		head = head->next;
		delete curNode;
		curSize--;
		lock.unlock();
		return cur;
	}
	
	int wait_and_pop() 
	{
		cv.wait(lock, [&]()
		{
			return curSize > 0;
		});
		int cur = head->value;
		Node* curNode = head;
		head = head->next;
		curSize--;
		delete curNode;
		lock.unlock();
		return cur;
	}
	
	void print() 
	{
		lock.lock();
		Node* cur = head;
		while(cur) 
		{
			cout<<cur->value<<" ";
			cur = cur->next;
		}
	}
	
};


int main() 
{
	Queue q(4);
	vector<thread>threads;
	for(int i=0;i<5;i++) 
	{
		threads.emplace_back(&Queue::push, &q, i);
	}
	
	threads.emplace_back(&Queue::wait_and_pop, &q);
	threads.emplace_back(&Queue::pop, &q);

	for(auto& thr: threads) 
	{
		if(thr.joinable())thr.join();
	}
	q.print();
	return 0;
}





















// class SpinLock 
// {
// 	public:
// 	std::atomic_flag flag {false};
	
// 	void lock() 
// 	{
// 		while(flag.test_and_set(std::memory_order_acquire));
// 	}
// 	void unlock()
// 	{
// 		flag.clear(std::memory_order_release);
// 	}
// };
