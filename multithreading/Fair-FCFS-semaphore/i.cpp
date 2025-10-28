#include<bits/stdc++.h>
#include<mutex>
#include<condition_variable>

using namespace std;

// class FairSemaphore
// {
// 	public: 
// 	int LIMIT = 4;
// 	mutex mx;
// 	int count = 0;
// 	queue<shared_ptr<condition_variable> >queue;
	
// 	void acquire() 
// 	{
// 		unique_lock<mutex>lock(mx) ;
// 		shared_ptr<condition_variable> cv = make_shared<condition_variable>();
// 		queue.push(cv);
// 		cv->wait(lock,[this,&cv](){return count<LIMIT && queue.front() == cv;});
// 		count++;
// 		queue.pop();
// 	}
// 	void release() 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		count--;
// 		shared_ptr<condition_variable> cv = queue.front();
// 		cv->notify_one();
// 	}
// };
int main() 
{
	return 0;
}
//WRONG IMPLEMENTATION BELOW
// class FCFSSemaphore 
// {
// 	public: 
// 	int LIMIT = 4;
// 	mutex mx;
// 	queue<shared_ptr<condition_variable>>queue;
// 	condition_variable cv;
// 	int count = 0;
	
// 	void acquire() 
// 	{
// 		shared_ptr<condition_variable> ptr = make_shared<condition_variable>();
// 		unique_lock<mutex>lock(mx);
// 		cv.wait(lock,[this](){return count<LIMIT;});
// 		count++;
// 		queue.push(ptr);
// 		ptr->wait(lock, [this,ptr](){return queue.front() != ptr;});
// 		queue.pop();
// 	}
	
// 	void release() 
// 	{
// 		{
// 			unique_lock<mutex>lock(mx);
// 			shared_ptr<condition_variable> ptr = queue.front();
			
// 			count--;
// 			ptr->notify_one();
// 		}
// 	}
// };