#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <semaphore>
using namespace std;


counting_semaphore<2> sem(2);

void accessResource(int id) {
    std::cout << "Thread " << id << " is trying to acquire the semaphore...\n";
    sem.acquire(); // Wait

    std::cout << "Thread " << id << " has acquired the semaphore.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work

    std::cout << "Thread " << id << " is releasing the semaphore.\n";
    sem.release(); // Signal
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