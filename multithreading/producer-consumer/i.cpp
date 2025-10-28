#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;
queue<int>q;
mutex mx;
condition_variable ccv;
condition_variable pcv;
int LIMIT = 5;
bool flag=false;
// void consumer() {
//     while(true) {
//         unique_lock<mutex> lock(mx);
//         cv.wait(lock, []{return !q.empty();});
//         int x = q.front();
//         q.pop();
//         cout<<" Task #"<<x<<" Consumed"<<endl;

//         if(q.empty() && flag==true) {
//             return;
//         }
//         cv.notify_one();
//     }
// }

// void producer() {
//     for(int i=0;i<10;i++) {
//         unique_lock<mutex>lock(mx);
//         cv.wait(lock, []{return q.size()< limit;});
//         q.push(i);
//         cout<<" Task #"<<i<<" Produced"<<endl;
//         cv.notify_one();
//     }
//     {
//         lock_guard<mutex> lock(mx);
//         flag = true;
//     }
//     cv.notify_one();
// }
void producer() 
{
	for(int i=0;i<10;i++)
	{
		unique_lock<mutex>lock(mx);
		pcv.wait(lock, [](){return q.size()<LIMIT;});
		q.push(i);
		ccv.notify_one();
	}
	
}

void consumer() 
{
	while(true) 
	{
		unique_lock<mutex>lock(mx);
		ccv.wait(lock, [](){return !q.empty() || flag;});
		if(flag) return;
		int x = q.front();
		q.pop();
		cout<<x<<endl;
		pcv.notify_one();
	}
}

int main() {
	cout<<"Hello World"<<endl;
	thread producer_thread(producer);
	thread consumer_thread(consumer);

	producer_thread.join();
	consumer_thread.join();


	return 0;
}