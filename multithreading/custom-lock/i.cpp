#include<bits/stdc++.h>


class SpinLock 
{
	public:
	std::atomic_flag flag {false};
	
	void lock() 
	{
		while(flag.test_and_set(std::memory_order_acquire));
	}
	void unlock()
	{
		flag.clear(std::memory_order_release);
	}
};