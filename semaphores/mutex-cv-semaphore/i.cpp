#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;
class CountingSemaphore {
    public: 
    explicit CountingSemaphore(int count) {
        count_ = count;
    }
    void acquire() {
        unique_lock<mutex>lock(mx);
        cv.wait(lock, [this](){return count_ > 0;});
        count_--;

    }

    void release() {
        unique_lock<mutex>lock(mx);
        count_++;
        cv.notify_one();
    }
    private:
        mutex mx;
        condition_variable cv;
        int count_;
};

CountingSemaphore sem(2);


void accessResource(int id) {
    std::cout << "Thread " << id << " wants to acquire semaphore...\n";
    sem.acquire();

    std::cout << "Thread " << id << " has entered critical section.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate work

    std::cout << "Thread " << id << " is leaving critical section.\n";
    sem.release();
}
int main() {

    thread t1(accessResource, 1);
    thread t2(accessResource, 2);
    thread t3(accessResource, 3);
    thread t4(accessResource, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}