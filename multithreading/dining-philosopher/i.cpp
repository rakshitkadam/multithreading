#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip> // For formatting time output

using namespace std;

// // --- Constants ---
// const int NUM_PHILOSOPHERS = 5;
// mutex forks[NUM_PHILOSOPHERS]; // one fork per philosopher
// mutex cout_mutex; // Mutex to protect cout for clean output

// // ANSI Color Codes for the philosophers
// const string COLORS[NUM_PHILOSOPHERS] = {
//     "\033[31m", // Red (P0)
//     "\033[32m", // Green (P1)
//     "\033[33m", // Yellow (P2)
//     "\033[34m", // Blue (P3)
//     "\033[35m"  // Magenta (P4)
// };
// const string RESET = "\033[0m"; // Reset color

// // --- Utility Functions ---

// // Get current time in seconds since the program started
// double get_current_time(const chrono::time_point<chrono::system_clock>& start_time) {
//     auto now = chrono::system_clock::now();
//     return chrono::duration<double>(now - start_time).count();
// }

// // Utility: random delay to simulate thinking/eating
// void random_sleep(int min_ms = 200, int max_ms = 600) {
//     static thread_local mt19937 gen(random_device{}());
//     uniform_int_distribution<> dist(min_ms, max_ms);
//     this_thread::sleep_for(chrono::milliseconds(dist(gen)));
// }

// // --- Dining Function with Logging ---

// // Global start time reference
// chrono::time_point<chrono::system_clock> global_start_time;

// void dine(int id) {
//     int left = id;
//     int right = (id + 1) % NUM_PHILOSOPHERS;

//     // Resource Ordering logic
//     int first = min(left, right);
//     int second = max(left, right);

//     while (true) {
//         // Thinking
//         {
//             lock_guard<mutex> lock(cout_mutex);
//             cout << fixed << setprecision(3) << "[" << get_current_time(global_start_time) << "s] "
//                  << COLORS[id] << "Philosopher " << id << RESET << " is **THINKING**\n";
//         }
//         random_sleep();

//         // Hungry - try to pick up forks
//         {
//             lock_guard<mutex> lock(cout_mutex);
//             cout << fixed << setprecision(3) << "[" << get_current_time(global_start_time) << "s] "
//                  << COLORS[id] << "Philosopher " << id << RESET << " is **HUNGRY**, trying to pick up forks " << first << " and " << second << "\n";
//         }

//         // Acquire both forks safely using std::lock()
//         std::lock(forks[first], forks[second]);

//         // Eating
//         {
//             lock_guard<mutex> lock(cout_mutex);
//             cout << fixed << setprecision(3) << "[" << get_current_time(global_start_time) << "s] "
//                  << COLORS[id] << "Philosopher " << id << RESET << " **STARTS EATING** (Holding F" << first << " & F" << second << ")\n";
//         }
//         random_sleep();

//         {
//             lock_guard<mutex> lock(cout_mutex);
//             cout << fixed << setprecision(3) << "[" << get_current_time(global_start_time) << "s] "
//                  << COLORS[id] << "Philosopher " << id << RESET << " **FINISHES EATING**\n";
//         }

//         // Unlock both forks after eating
//         forks[second].unlock();
//         forks[first].unlock();
//     }
// }

// // --- Main Function ---

// int main() {
//     global_start_time = chrono::system_clock::now();
	
//     vector<thread> philosophers;
//     for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
//         philosophers.emplace_back(dine, i);

//     for (auto& p : philosophers)
//         p.join();

//     return 0;
// }



class DiningPhilosopher 
{
	
	public: 
	
	int num = 4;
	mutex mx[4];
	
	void dineIn(int id) 
	{
		int fork1 = id;
		int fork2 = (id+1)%num;
		
		int left = min(fork1, fork2);
		int right = max(fork1, fork2);
		
		while(true) 
		{
			
			{
				lock(mx[left], mx[right]);
				// eat 
				this_thread::sleep_for(chrono::milliseconds(100));
				mx[left].unlock();
				mx[right].unlock();
			}
			
			// think
			this_thread::sleep_for(chrono::milliseconds(100));
			
		}
		
	}
};