#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <random>

using namespace std;

class BarberShop {
    private: 
    int maxChairs;
    queue<int>waitingCustomers;
    mutex mtx;
    condition_variable barberCV, customerCV;
    bool barberSleeping = true;

    public:
    BarberShop(int chairs) : maxChairs(chairs) {}

    void customerArrives(int id) {
        unique_lock<mutex> lock(mtx);

        if (waitingCustomers.size() == maxChairs) {
            cout << "Customer " << id << " leaves (no empty chairs)\n";
            return;
        }

        waitingCustomers.push(id);
        cout << "Customer " << id << " waiting. Queue size: " << waitingCustomers.size() << "\n";

        // Wake up the barber if he's sleeping
        if (barberSleeping) {
            barberSleeping = false;
            barberCV.notify_one();
        }

        // Wait until the barber calls you
        customerCV.wait(lock, [&]() {
            return waitingCustomers.front() == id;
        });

        waitingCustomers.pop();
        cout << "Customer " << id << " getting haircut\n";
        lock.unlock();

        // Simulate haircut time
        this_thread::sleep_for(chrono::milliseconds(400 + rand() % 400));
        cout << "Customer " << id << " done!\n";
    }

    void barberWork() {

        while (true) {
            unique_lock<mutex> lock(mtx);
            barberSleeping = true;
            // Wait for customers if none are present
            barberCV.wait(lock, [&]() { return !waitingCustomers.empty(); });
            barberSleeping = false;
            int next = waitingCustomers.front();
            cout << "Barber calls customer " << next << "\n";

            // Signal the next waiting customer
            customerCV.notify_all();
            lock.unlock();

            // Simulate haircut
            this_thread::sleep_for(chrono::milliseconds(500 + rand() % 500));
        }
    }
};

int main() {
    srand(time(0));
    BarberShop shop(3);  // 3 waiting chairs

    thread barber(&BarberShop::barberWork, &shop);

    // Generate customers periodically
    vector<thread> customers;
    for (int i = 0; i < 10; ++i) {
        this_thread::sleep_for(chrono::milliseconds(200 + rand() % 300));
        customers.emplace_back(&BarberShop::customerArrives, &shop, i);
    }

    for (auto& c : customers) c.join();
    barber.detach(); // runs indefinitely
}


//NOTE THAT THIS IS BRUTE FORCE VIA NOTIFY_ALL, WE CAN IMPROVE THIS USING NOTIFY_ONE AND MAINTAINING CV FOR EACH CUSTOMER
// Each customer has their own condition variable
// struct Customer {
//     int id;
//     condition_variable cv;
//     bool ready = false;
// };

// class BarberShop {
// private:
//     int maxChairs;
//     queue<shared_ptr<Customer>> waitingCustomers;
//     mutex mtx;
//     condition_variable barberCV;
//     bool barberSleeping = true;
//     bool shopOpen = true;

// public:
//     BarberShop(int chairs) : maxChairs(chairs) {}

//     void customerArrives(int id) {
//         unique_lock<mutex> lock(mtx);

//         // Check if shop full
//         if (waitingCustomers.size() == maxChairs) {
//             cout << "💺 Customer " << id << " leaves (no empty chairs)\n";
//             return;
//         }

//         // Create a new customer object
//         auto cust = make_shared<Customer>();
//         cust->id = id;

//         waitingCustomers.push(cust);
//         cout << "🧍 Customer " << id << " waiting. Queue size: " 
//              << waitingCustomers.size() << "\n";

//         // Wake up barber if sleeping
//         barberSleeping = false;
//         barberCV.notify_one();

//         // Wait for barber to call
//         cust->cv.wait(lock, [&]() { return cust->ready; });
//         lock.unlock();

//         // Simulate haircut
//         cout << "💇 Customer " << id << " getting haircut\n";
//         this_thread::sleep_for(chrono::milliseconds(400 + rand() % 400));
//         cout << "✅ Customer " << id << " done!\n";
//     }

//     void barberWork() {
//         while (true) {
//             unique_lock<mutex> lock(mtx);

//             // Sleep until there’s at least one waiting customer
//             barberCV.wait(lock, [&]() {
//                 return !waitingCustomers.empty() || !shopOpen;
//             });

//             if (!shopOpen && waitingCustomers.empty()) {
//                 cout << "🚪 Barber closing shop.\n";
//                 return;
//             }

//             // Get next customer
//             auto next = waitingCustomers.front();
//             waitingCustomers.pop();

//             cout << "💈 Barber calls customer " << next->id << "\n";

//             // Signal only this customer
//             next->ready = true;
//             next->cv.notify_one();
//             lock.unlock();

//             // Do haircut
//             this_thread::sleep_for(chrono::milliseconds(500 + rand() % 500));
//         }
//     }

//     void closeShop() {
//         {
//             lock_guard<mutex> lock(mtx);
//             shopOpen = false;
//         }
//         barberCV.notify_all();
//     }
// };

// int main() {
//     srand(static_cast<unsigned>(time(nullptr)));

//     BarberShop shop(3); // 3 waiting chairs
//     thread barber(&BarberShop::barberWork, &shop);

//     vector<thread> customers;
//     for (int i = 0; i < 10; ++i) {
//         this_thread::sleep_for(chrono::milliseconds(200 + rand() % 300));
//         customers.emplace_back(&BarberShop::customerArrives, &shop, i);
//     }

//     // Wait for all customers to finish
//     for (auto& c : customers) c.join();

//     // Close shop and wait for barber
//     shop.closeShop();
//     barber.join();

//     cout << "🛑 Shop closed for the day.\n";
//     return 0;
// }


