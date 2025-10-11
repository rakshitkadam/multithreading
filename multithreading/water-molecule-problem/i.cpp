#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

using namespace std;
// Better approach in second PART WHICH IS IMPLEMENTED BELOW THIS CODE
class H2O {
private:
    mutex mtx;
    condition_variable hcv;
    condition_variable ocv;
    
    // Shared state for molecule formation
    int hydrogenCount = 0;
    int oxygenCount = 0;

public:
    H2O() {
        // Initialize any required variables here
    }

    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock<mutex> lock(mtx);

        hcv.wait(lock,[this](){return hydrogenCount< 2;});
        
        
        hydrogenCount++;
        releaseHydrogen(); // Print or log "H"
        ocv.notify_one();

    }

    void oxygen(function<void()> releaseOxygen) {
        unique_lock<mutex> lock(mtx);
        
        ocv.wait(lock,[this](){return hydrogenCount== 2 && oxygenCount<1;});

        // TODO: Wait until this oxygen can proceed
        // e.g., wait until no other oxygen is waiting

        // Process this oxygen
        oxygenCount++;
        releaseOxygen(); // Print or log "O"

        // TODO: Check if a full molecule (2H + 1O) is ready
        // If so, reset counts and notify others
        hydrogenCount = 0;
        oxygenCount=0;
        hcv.notify_one();
        hcv.notify_one();
        
        // TODO: Otherwise, wait until molecule is formed
    }
};

int main() {
    H2O h2o;
    vector<thread> threads;

    // Simulate incoming atoms
    string input = "OOHHHHHH";  // You can change this input for testing
    for(int i=0;i<15;i++) input += "HHO";
    // for(int i=0;i<10;i++) {
    //     int x = rand()%2;
    //     if(x)input+="H";
    //     else input+="O";
    // }
    for (char c : input) {
        if (c == 'H') {
            threads.emplace_back(&H2O::hydrogen, &h2o, [](){ cout << "H"; });
        } else if (c == 'O') {
            threads.emplace_back(&H2O::oxygen, &h2o, [](){ cout << "O"; });
        }
    }

    for (auto& t : threads) t.join();
    cout << endl;

    return 0;
}


// VERY VERY IMP OF HOW WE CAN USE SEMAPHORE FOR CREATING A BARRIER.
// Purely using semaphore
#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <mutex>
#include <functional>

using namespace std;

class H2O {
private:
    std::counting_semaphore<2> sem_hyd{2}; // Allow 2 hydrogens
    std::counting_semaphore<1> sem_oxy{0}; // Allow 1 oxygen after 2 H
    int HCnt = 0;
    mutex m;

public:
    H2O() = default;

    void hydrogen(function<void()> releaseHydrogen) {
        sem_hyd.acquire();   // Wait for hydrogen slot
        releaseHydrogen();

        lock_guard<mutex> lock(m);
        HCnt++;
        if (HCnt == 2) {
            HCnt = 0;
            sem_oxy.release();  // Allow one oxygen to proceed
        }
    }

    void oxygen(function<void()> releaseOxygen) {
        sem_oxy.acquire();  // Wait until 2 hydrogens have arrived
        releaseOxygen();

        sem_hyd.release();  // Reopen hydrogen slots
        sem_hyd.release();
    }
};

int main() {
    H2O water;

    // Simulate 6 hydrogen atoms and 3 oxygen atoms (3 H2O molecules)
    string input = "HOHHOHHOH"; // Randomized order
    vector<thread> threads;

    mutex cout_mutex; // For synchronized output

    for (char atom : input) {
        if (atom == 'H') {
            threads.emplace_back([&]() {
                water.hydrogen([&]() {
                    lock_guard<mutex> lock(cout_mutex);
                    cout << "H";
                });
            });
        } else if (atom == 'O') {
            threads.emplace_back([&]() {
                water.oxygen([&]() {
                    lock_guard<mutex> lock(cout_mutex);
                    cout << "O";
                });
            });
        }
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }

    cout << "\nWater molecule simulation completed.\n";

    return 0;
}
