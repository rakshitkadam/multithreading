#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class Foo {
private:
    mutex mtx;
    condition_variable cv;
    int state;  // 0 = first, 1 = second, 2 = third

public:
    Foo() : state(0) {}

    void first() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this](){return state == 0;});
        cout << "first\n";
        state = 1;
        cv.notify_all();
    }

    void second() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this](){ return state == 1; });
        cout << "second\n";
        state = 2;
        cv.notify_all();
    }

    void third() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this](){ return state == 2; });
        cout << "third\n";
    }
};

int main() {
    Foo foo;

    thread t1(&Foo::first, &foo);
    thread t2(&Foo::second, &foo);
    thread t3(&Foo::third, &foo);

    // Start them in any order
    t3.join();
    t2.join();
    t1.join();

    return 0;
}

// How to extend it further to N : 
// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <vector>
// using namespace std;

// class OrderedExecutor {
// private:
//     mutex mtx;
//     condition_variable cv;
//     int state;
//     int total;

// public:
//     OrderedExecutor(int n) : state(0), total(n) {}

//     void run(int order, function<void()> task) {
//         unique_lock<mutex> lock(mtx);
//         cv.wait(lock, [this, order]() { return state == order; });

//         // Run the task for this order
//         task();

//         // Move to the next state and notify everyone
//         state++;
//         cv.notify_all();
//     }
// };

// int main() {
//     const int N = 5;
//     OrderedExecutor executor(N);

//     vector<thread> threads;
//     for (int i = 0; i < N; ++i) {
//         threads.emplace_back([i, &executor]() {
//             executor.run(i, [i]() {
//                 cout << "Task " << i << endl;
//             });
//         });
//     }

//     // Wait for all threads to finish
//     for (auto& t : threads) {
//         t.join();
//     }

//     return 0;
// }