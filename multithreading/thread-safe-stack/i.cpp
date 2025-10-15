#include<bits/stdc++.h>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<atomic>
using namespace std;

// template<typename T>
// class ThreadSafeStack {
// private:
//     struct Node {
//         T data;
//         Node* next;
//         Node(const T& value) : data(value), next(nullptr) {}
//     };

//     Node* head;
//     mutable std::mutex mx;
//     std::condition_variable cv;
//     std::atomic<size_t> sizee;

// public:
//     ThreadSafeStack() : head(nullptr), sizee(0) {}

//     ~ThreadSafeStack() {
//         std::lock_guard<std::mutex> lock(mx);
//         while (head) {
//             Node* tmp = head;
//             head = head->next;
//             delete tmp;
//         }
//     }

//     void push(const T& value) {
//         Node* newNode = new Node(value);
//         {
//             std::lock_guard<std::mutex> lock(mx);
//             newNode->next = head;
//             head = newNode;
//             sizee.fetch_add(1, std::memory_order_release);
//         }
//         cv.notify_one();
//     }

//     void pop() {
//         std::unique_lock<std::mutex> lock(mx);
//         cv.wait(lock, [this]() { return head != nullptr; });

//         Node* oldHead = head;
//         head = head->next;
//         sizee.fetch_sub(1, std::memory_order_release);
//         lock.unlock();

//         delete oldHead;
//     }

//     std::optional<T> top() const {
//         std::lock_guard<std::mutex> lock(mx);
//         if (!head) {
//             return std::nullopt;
//         }
//         return head->data;
//     }

//     bool try_pop(T& result) {
//         std::lock_guard<std::mutex> lock(mx);
//         if (!head) {
//             return false;
//         }
//         Node* oldHead = head;
//         result = oldHead->data;
//         head = oldHead->next;
//         sizee.fetch_sub(1, std::memory_order_release);

//         delete oldHead;
//         return true;
//     }

//     bool empty() const {
//         return sizee.load(std::memory_order_acquire) == 0;
//     }

//     size_t size() const {
//         return sizee.load(std::memory_order_acquire);
//     }
// };




// DOUBLY LINKED LIST
// template<typename T>
// class ThreadSafeStack 
// {
// 	public: 
	
// 	struct Node{ 
// 		Node* left;
// 		Node* right;
// 		T val;
// 	};
	
// 	Node* head;
// 	Node* cur;
// 	mutex mx;
// 	atomic<size_t> sizee;
// 	condition_variable cv;
	
// 	ThreadSafeStack() 
// 	{
// 		lock_guard<mutex>lock(mx);
// 		head = new Node();
// 		head->left = NULL;
// 		head->right = NULL;
// 		sizee.store(0);
// 		cur = head;
// 	}
// 	void push(const T& item) {
// 		Node* newNode = new Node();
// 		lock_guard<mutex>lock(mx);
		
// 		cur->right = newNode;
// 		newNode->left= cur;
// 		newNode->right = NULL;
// 		newNode->val = item;
// 		cur = newNode;
// 		sizee.fetch_add(1);
// 		cv.notify_one();
// 	}
	
// 	void pop() 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		cv.wait(lock, [this](){return !this->empty();});
// 		Node* prev = cur->left;
// 		cur->left = NULL;
// 		cur->right = NULL;
// 		delete cur;
// 		cur = prev;
// 		sizee.fetch_sub(1);
		
// 	}
	
// 	optional<T> top() const 
// 	{
// 		lock_guard<mutex>lock(mx);
// 		if(this->empty())return nullopt; 
// 		return cur->val;
// 	}

// 	bool empty() const 
// 	{
// 		return sizee.load()==0;
// 	}

// 	size_t size() const 
// 	{
// 		return sizee.load();
// 	}

// 	bool try_pop(T& result) 
// 	{
// 		lock_guard<mutex>lock(mx);
// 		if(this->empty()) 
// 		{
// 			return false;
// 		}
// 		result = cur->val;
// 		Node* prev = cur->left;
// 		cur->left = NULL;
// 		cur->right = NULL;
// 		delete cur;
// 		cur = prev;
// 		sizee.fetch_sub(1);
// 		return true;
// 	}
// 	~ThreadSafeStack() 
// 	{
// 		lock_guard<mutex>lock(mx);
// 		while(cur!=head) 
// 		{
// 			Node* pre = cur->left;
// 			cur->left = NULL;
// 			cur->right = NULL;
// 			delete cur;
// 			cur = pre;
// 		}
// 		head->left = NULL;
// 		head->right = NULL;
// 		delete head;
// 	}
// };

int main() {
	std::chrono::time_point timeStart = std::chrono::steady_clock::now();
	std::this_thread::sleep_for(std::chrono::milliseconds(120));
	
	std::chrono::time_point timeEnd = std::chrono::steady_clock::now();
	
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart) ;
	cout<<duration.count()<<endl;
	
	return 0;
}