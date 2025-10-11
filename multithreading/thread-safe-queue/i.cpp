#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>

using namespace std;

template <typename T>
class ThreadSafeQueue {
 public:
  queue<T> queue;
  mutex mx;
  condition_variable cv;
  ThreadSafeQueue() {}

  void push(const T &item) {
	lock_guard<mutex> lock(mx);
	queue.push(item);
	cv.notify_one();
  }

  bool try_pop(T &out) {
	lock_guard<mutex> lock(mx);
	if (queue.empty()) {
	  return false;
	}
	out = queue.front();
	queue.pop();
	return true;
  }
  T wait_and_pop() {
	unique_lock<mutex> lock(mx);
	cv.wait(lock, [this]() { return !queue.empty(); });

	T front = queue.front();
	queue.pop();
	return front;
  }
  bool empty() const {
	lock_guard<mutex> lock(mx);
	if (queue.empty()) {
	  return true;
	}
	return false;
  }
  size_t size() const {
	lock_guard<mutex> lock(mx);
	return queue.size();
  }
};

int main() {
  ThreadSafeQueue<int> queue;
  mutex mcout;
  // Producer 1
  std::thread producer1([&]() {
	for (int i = 0; i < 5; ++i) {
	  queue.push(i);
	  {
		lock_guard<mutex> lock(mcout);
		std::cout << "Producer 1 pushed: " << i << std::endl;
	  }
	  std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
  });

  // Producer 2
  std::thread producer2([&]() {
	for (int i = 5; i < 10; ++i) {
	  queue.push(i);
	  {
		lock_guard<mutex> lock(mcout);
		std::cout << "Producer 2 pushed: " << i << std::endl;
	  }
	  std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
  });

  // Consumer 1
  std::thread consumer1([&]() {
	for (int i = 0; i < 5; ++i) {
	  int val = queue.wait_and_pop();
	  {
		lock_guard<mutex> lock(mcout);
		std::cout << "Consumer 1 popped: " << val << std::endl;
	  }
	}
  });

  // Consumer 2
  std::thread consumer2([&]() {
	for (int i = 0; i < 5; ++i) {
	  int val = queue.wait_and_pop();
	  {
		lock_guard<mutex> lock(mcout);
		std::cout << "Consumer 2 popped: " << val << std::endl;
	  }
	}
  });

  // Join all threads
  producer1.join();
  producer2.join();
  consumer1.join();
  consumer2.join();
  {
	lock_guard<mutex> lock(mcout);

	std::cout << "All threads finished." << std::endl;
  }
  return 0;
}