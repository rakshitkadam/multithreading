// Reader preference locking



#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

std::vector<std::string> logbook;
mutex mx;
condition_variable cv;
bool writerFlag=false;
int readerCount = 0;

void writer_function(int writer_id, const std::string& message) {
    unique_lock<mutex> lock(mx);
    cv.wait(lock,[](){return !writerFlag && readerCount == 0;} );
    writerFlag =true;
    logbook.push_back(message);
    std::cout << "Writer " << writer_id << ": Wrote log \"" << message 
              << "\" at index " << logbook.size() - 1 << std::endl;
    writerFlag=false;
    lock.unlock();
    cv.notify_all();
}

void reader_function(int reader_id) {

    {
        unique_lock<mutex>lock(mx);
        cv.wait(lock,[](){return !writerFlag;});
        readerCount++;
    }
    
    std::cout << "Reader " << reader_id << ": Reading logs" << std::endl;
    for (size_t i = 0; i < logbook.size(); ++i) {
        std::cout << "  Reader " << reader_id << ": Log " << i 
                  << " => " << logbook[i] << std::endl;
    }
    {
        unique_lock<mutex>lock(mx);
        readerCount--;
        if(readerCount==0)
        lock.unlock();
        cv.notify_all();
    }
    

}

int main() {
    int writerThread = 5;
    int readerThread = 3;
    vector<thread>threads;

    for(int i=0;i<writerThread;i++) {
        threads.emplace_back(thread(writer_function,i,"Task is done by writer thread " + to_string(i)));
    }
    for(int i=0;i<readerThread;i++) {
        threads.emplace_back(thread(reader_function, i));
    }

    for(auto& t : threads) {
        t.join();
    }
}

// Shared mutex after c++ 17 is a library where it allows shared access over resources when shared_lock is used and for unique
//  access it can go with unique lock

// #include <iostream>
// #include <shared_mutex>
// #include <thread>

// std::shared_mutex shared_mtx;
// int shared_data = 0;

// void reader(int id) {
//     std::shared_lock lock(shared_mtx); // shared access
//     std::cout << "Reader " << id << " reads value: " << shared_data << "\n";
// }

// void writer(int id, int new_val) {
//     std::unique_lock lock(shared_mtx); // exclusive access
//     shared_data = new_val;
//     std::cout << "Writer " << id << " wrote value: " << new_val << "\n";
// }


// =================================================================================================


// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <vector>
// #include <chrono>

// class RWLock {
// private:
//     std::mutex mtx;
//     std::condition_variable cv;

//     int active_readers = 0;
//     int waiting_writers = 0;
//     bool writer_active = false;

// public:
//     void startRead() {
//         std::unique_lock<std::mutex> lock(mtx);
//         // Wait while there's a writer or writers waiting (writer priority)
//         cv.wait(lock, [this]() {
//             return !writer_active && waiting_writers == 0;
//         });
//         active_readers++;
//     }

//     void endRead() {
//         std::unique_lock<std::mutex> lock(mtx);
//         active_readers--;
//         if (active_readers == 0)
//             cv.notify_all(); // Notify waiting writers
//     }

//     void startWrite() {
//         std::unique_lock<std::mutex> lock(mtx);
//         waiting_writers++;
//         cv.wait(lock, [this]() {
//             return !writer_active && active_readers == 0;
//         });
//         waiting_writers--;
//         writer_active = true;
//     }

//     void endWrite() {
//         std::unique_lock<std::mutex> lock(mtx);
//         writer_active = false;
//         cv.notify_all(); // Notify all, so readers or writers can proceed
//     }
// };

// // Shared resource
// RWLock rwlock;
// int shared_data = 0;

// void reader(int id) {
//     while (true) {
//         rwlock.startRead();
//         std::cout << "[Reader " << id << "] reads value: " << shared_data << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//         rwlock.endRead();
//         std::this_thread::sleep_for(std::chrono::milliseconds(200));
//     }
// }

// void writer(int id) {
//     while (true) {
//         rwlock.startWrite();
//         shared_data++;
//         std::cout << ">>> [Writer " << id << "] writes value: " << shared_data << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(150));
//         rwlock.endWrite();
//         std::this_thread::sleep_for(std::chrono::milliseconds(400));
//     }
// }

// int main() {
//     std::vector<std::thread> threads;

//     // Start some readers
//     for (int i = 0; i < 3; ++i)
//         threads.emplace_back(reader, i + 1);

//     // Start some writers
//     for (int i = 0; i < 2; ++i)
//         threads.emplace_back(writer, i + 1);

//     for (auto& th : threads)
//         th.join();

//     return 0;
// }
