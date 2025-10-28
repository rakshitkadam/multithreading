

// Design and implement a queue management system for a single bathroom used in a voting agency shared by Democrats and Republicans. 
// The system must adhere to strict rules regarding usage and prioritize fairness while maximizing efficiency.

// Requirements:
// Single Bathroom: The bathroom can accommodate a maximum of 3 people at a time.
// Homogeneous Groups Only: At any given time, the bathroom must be used by either only Democrats (D) or only Republicans (R).
// Mixed groups like (2D, 1R) or (1D, 1R) are not allowed.
// Queue Management: People wait in a queue until their group can fully occupy the bathroom according to the constraints.
// Usage Duration: Each person takes a variable amount of time, calculated by a function f(N), where N is the person's name.
// Fairness: The system must ensure that eligible people are served as quickly as possible while adhering to the above constraints.
// Prioritization: The system should allow the most eligible group (based on availability and queue conditions) to occupy the
// bathroom whenever there's room.
// Input
// A queue of people arriving sequentially, where each person belongs to either group D or R.
// A function f(N) that returns the bathroom usage time for each individual.
// Output
// A schedule of bathroom usage that ensures:
// Maximum efficiency in bathroom usage.
// Fairness in serving Democrats and Republicans.
// Adherence to group-based constraints.
// Examples
// Example 1
// Input:
// Queue = ["D1", "D2", "R1", "R2", "D3", "D4", "R3"]
// f(N) returns: D1=3s, D2=4s, R1=5s, R2=3s, D3=2s, D4=6s, R3=4s

// Output:

// Time 0–7s: Bathroom used by [D1, D2, D3]
// Time 7–13s: Bathroom used by [D4]
// Time 13–16s: Bathroom used by [R1, R2, R3]
// Example: 2

// Input:  
// Queue = ["R1", "R2", "D1", "D2", "D3", "R3", "D4"] 
// f(N) returns: R1=2s, R2=3s, D1=4s, D2=5s, D3=3s, R3=2s, D4=6s

// Output:  
// - Time 0–5s: Bathroom used by [R1, R2, R3]  
// - Time 5–12s: Bathroom used by [D1, D2, D3]  
// - Time 12–18s: Bathroom used by [D4]  
// Constraints
// The bathroom usage time f(N) must be non-negative.
// Both Democrats and Republicans arrive in random order.

#include<bits/stdc++.h>
#include<mutex>
#include<condition_variable>
#include<atomic>

using namespace std;

// class BathroomManager 
// {
// 	public:
// 	condition_variable rcv;
// 	condition_variable dcv;
// 	mutex mx;
// 	int LIMIT = 3;
// 	queue<string>dq;
// 	queue<string>rq;
// 	int dcount = 0;
// 	int rcount = 0;
// 	enum Party{NONE, DEMOCRAT, REPUBLIC} currentParty = NONE;
// 	Party nextParty = NONE;
// 	// Called by a Democrat thread to request entry.
// 	void democratEnters(string name) 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		dq.push(name);  // Add to queue first!
		
// 		dcv.wait(lock, [this,name]()
// 		{
// 			return (currentParty==NONE || currentParty==DEMOCRAT) 
// 					&& dcount<LIMIT && !dq.empty() && dq.front() == name && (nextParty == NONE || nextParty == DEMOCRAT);
// 		});
		
// 		cout<<"Democrat with name :" + name + " entered"<<endl;
// 		dq.pop();
// 		currentParty = DEMOCRAT;
// 		dcount++;
		
// 	}

// 	// Called by a Democrat thread when it leaves.
// 	void democratLeaves(string name) 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		cout<<"Democrat with name :" + name + " leaves"<<endl;
// 		dcount--;
// 		if(dcount==0) 
// 		{
// 			currentParty = NONE;
// 			if(!rq.empty())  // Check if Republicans are waiting!
// 			{
// 				nextParty = REPUBLIC;
// 			}
// 			else 
// 			{
// 				nextParty = NONE;
// 			}
// 		}
// 		lock.unlock();
// 		dcv.notify_all();
// 		rcv.notify_all();
		
// 	}

// 	// Called by a Republican thread to request entry.
// 	void republicanEnters(string name) 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		rq.push(name);  // Add to queue first!
		
// 		rcv.wait(lock, [this,name]()
// 		{
// 			return (currentParty==NONE || currentParty==REPUBLIC) 
// 					&& rcount<LIMIT && !rq.empty() && rq.front() == name && (nextParty == NONE || nextParty == REPUBLIC);
// 		});
		
// 		cout<<"Republic with name :" + name + " entered"<<endl;
// 		rq.pop();
// 		currentParty = REPUBLIC;
// 		rcount++;
// 	}

// 	// Called by a Republican thread when it leaves.
// 	void republicanLeaves(string name) {
// 		unique_lock<mutex>lock(mx);
// 		cout<<"Republic with name :" + name + " leaves"<<endl;
// 		rcount--;
// 		if(rcount==0) 
// 		{
// 			currentParty = NONE;
// 			if(!dq.empty())  // Check if Democrats are waiting!
// 			{
// 				nextParty = DEMOCRAT;
// 			}
// 			else 
// 			{
// 				nextParty = NONE;
// 			}
// 		}
// 		lock.unlock();
// 		dcv.notify_all();
// 		rcv.notify_all();
		
// 	}
	


// };

// int f(const string &name) {
// 	// Different durations for each person
// 	if (name == "D1") return 3;
// 	if (name == "D2") return 4;
// 	if (name == "R1") return 5;
// 	if (name == "R2") return 3;
// 	if (name == "D3") return 2;
// 	if (name == "D4") return 6;
// 	if (name == "R3") return 4;
// 	return 3;
// }



// class BathroomManager 
// {
// 	public: 
// 	enum Party {NONE, R, D};
// 	Party cur = NONE;
// 	Party next = NONE;
// 	queue<string>demo;
// 	queue<string>rep;
// 	condition_variable dcv;
// 	condition_variable rcv;
// 	int demCount = 0;
// 	int repCount = 0;
// 	mutex mx;
// 	int LIMIT = 3;
// 	void democratEnters(std::string name) 
// 	{
// 		unique_lock<mutex>lock(mx);
		
// 		dcv.wait(lock,[this, name](){return demCount < LIMIT && (cur == NONE || cur == D)
// 			 && (next == NONE || next == R) && !demo.empty() && demo.front() == name; });
// 		cur = D;
// 		demCount ++ ;
// 		demo.push(name);
		
// 	}
// 	void democratLeaves(std::string name) 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		demo.pop();
// 		demCount--;
// 		if(demCount==0) 
// 		{
// 			cur = NONE;
// 			if(!rep.empty()) next = R;
// 		}
// 	}
// 	void republicanEnters(std::string name);
// 	void republicanLeaves(std::string name);
// };


// class BathroomManager 
// {
// 	public: 
	
// 	struct Democrat
// 	{
// 		std::chrono::steady_clock::duration enterTime;;
// 		std::chrono::steady_clock::duration exitTime;
// 		string name;
// 	};
	
// 	struct Repub
// 	{
// 		std::chrono::steady_clock::duration enterTime;;
// 		std::chrono::steady_clock::duration exitTime;
// 		string name;
// 	};
// 	queue<shared_ptr<Democrat>>demo;
// 	queue<shared_ptr<Repub> >rep;
// 	condition_variable dcv;
// 	condition_variable rcv;
// 	chrono::time_point<std::chrono::steady_clock> start;
// 	int demCount = 0;
// 	int repCount = 0;
// 	mutex mx;
// 	int LIMIT = 3;
// 	std::chrono::steady_clock::duration scheduledExit;
// 	BathroomManager ()
// 	{
// 		start = std::chrono::steady_clock::now();
// 		scheduledExit = chrono::steady_clock::now() - start;
// 	}
// 	void democratEnters(std::string name, int time) 
// 	{
// 		auto timeArrived = std::chrono::steady_clock::now();
// 		unique_lock<mutex>lock(mx);
// 		shared_ptr<Democrat> dem = make_shared<Democrat>();
// 		dem->enterTime = timeArrived-start;
// 		cout<<chrono::duration_cast<chrono::seconds>(dem->enterTime).count()<<endl;
// 		demo.push(dem);
// 		dcv.wait(lock, [this, dem]()
// 		{
// 			return repCount == 0 && demCount<LIMIT && ((demCount == 0 && (rep.empty() ||
// 			 (rep.size() && rep.front()->enterTime > demo.front()->enterTime ))) || (demCount>0 && scheduledExit > dem->enterTime));
			
// 		});
// 		scheduledExit = dem->enterTime + f(name) -> time require to do bathroom
// 		demCount++;
// 		while(demCount< LIMIT) 
// 		{
// 			if (the time requried by next decomcrat < scheduledExit) 
// 			{
// 				pop the element from demo, schedule the scheduleExit accordingly and notify the dcv 
// 			}
// 		}
		
		
// 	}
// 	void democratLeaves(std::string name) 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		notify_all dcv, rcv
// 	}
// 	void republicanEnters(std::string name);
// 	void republicanLeaves(std::string name);
// };

// void democrat(BathroomManager &bathroom, string name) {
// 	bathroom.democratEnters(name);
// 	this_thread::sleep_for(chrono::seconds(f(name))); // using the bathroom
// 	bathroom.democratLeaves(name);
// }

// void republican(BathroomManager &bathroom, string name) {
// 	bathroom.republicanEnters(name);
// 	this_thread::sleep_for(chrono::seconds(f(name))); // using the bathroom
// 	bathroom.republicanLeaves(name);
// }
// int main()
// {
// 	BathroomManager bathroom;

// 	// Example input queue
// 	vector<string> queue = {"D1", "D2", "R1", "R2", "D3", "D4", "R3"};

// 	// Create threads in arrival order
// 	vector<thread> threads;
// 	for (auto &person : queue) {
// 		if (person[0] == 'D')
// 			threads.emplace_back(democrat, ref(bathroom), person);
// 		else
// 			threads.emplace_back(republican, ref(bathroom), person);
// 		this_thread::sleep_for(chrono::milliseconds(300)); // simulate staggered arrivals
// 	}

// 	// Wait for all to finish
// 	for (auto &t : threads) t.join();

// 	cout << "\nAll finished." << endl;
// 	return 0;
// }

#include <iostream>
#include <queue>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

using namespace std::chrono;

enum class Party { Democrat, Republican };

struct Person {
    Party party;
    std::string name;
    steady_clock::duration enterTime;   // relative to start
    steady_clock::duration duration;    // how long they use bathroom
};

class BathroomManager {
public:
    BathroomManager(int limit = 3)
        : LIMIT(limit),
          demCount(0), repCount(0),
          start(steady_clock::now()),
          scheduledExit(steady_clock::duration::min()),
          currentParty(std::nullopt) {}

    // Person tries to enter bathroom
    void enter(Party party, const std::string& name, steady_clock::duration useDuration) {
        auto now = steady_clock::now();
        auto relativeNow = now - start;

        std::unique_lock<std::mutex> lock(mx);

        // Create person object
        auto person = std::make_shared<Person>();
        person->party = party;
        person->name = name;
        person->enterTime = relativeNow;
        person->duration = useDuration;

        // Push to appropriate queue
        if (party == Party::Democrat) {
            demo.push(person);
        } else {
            rep.push(person);
        }

        // Wait until conditions allow entry
        cv.wait(lock, [&]() {
            // Conditions:
            // 1) Bathroom empty or current party matches this person
            bool partyMatches = (!currentParty.has_value() || currentParty.value() == party);
            // 2) No opposite party currently inside
            bool noOppositeInside = (party == Party::Democrat ? repCount == 0 : demCount == 0);
            // 3) Occupancy limit not exceeded
            bool limitNotReached = (party == Party::Democrat ? demCount < LIMIT : repCount < LIMIT);
            // 4) Person is at front of their queue
            bool isFrontOfQueue = false;
            if (party == Party::Democrat && !demo.empty() && demo.front() == person) {
                isFrontOfQueue = true;
            } else if (party == Party::Republican && !rep.empty() && rep.front() == person) {
                isFrontOfQueue = true;
            }

            // 5) Enter only if person's enterTime <= scheduledExit (or bathroom empty)
            bool timeOk = (scheduledExit == steady_clock::duration::min() || person->enterTime <= scheduledExit);

            return partyMatches && noOppositeInside && limitNotReached && isFrontOfQueue && timeOk;
        });

        // Allowed to enter: pop from queue, update counts and scheduledExit
        if (party == Party::Democrat) {
            demo.pop();
            demCount++;
            currentParty = Party::Democrat;
        } else {
            rep.pop();
            repCount++;
            currentParty = Party::Republican;
        }

        // Update scheduledExit = max(current scheduledExit, person enterTime + duration)
        auto personExitTime = person->enterTime + person->duration;
        if (scheduledExit == steady_clock::duration::min() || personExitTime > scheduledExit) {
            scheduledExit = personExitTime;
        }

        std::cout << name << " (" << (party == Party::Democrat ? "Democrat" : "Republican")
                  << ") entered at " << duration_cast<milliseconds>(person->enterTime).count()
                  << "ms, will exit at " << duration_cast<milliseconds>(scheduledExit).count() << "ms\n";
    }

    // Person leaves the bathroom
    void leave(Party party, const std::string& name) {
    auto now = steady_clock::now();
    auto relativeNow = now - start;

    std::unique_lock<std::mutex> lock(mx);

    if (party == Party::Democrat) {
        if (demCount > 0) demCount--;
    } else {
        if (repCount > 0) repCount--;
    }

    std::cout << name << " (" << (party == Party::Democrat ? "Democrat" : "Republican")
              << ") left at " << duration_cast<milliseconds>(relativeNow).count() << "ms\n";

    if (demCount == 0 && repCount == 0) {
        scheduledExit = steady_clock::duration::min();
        currentParty = std::nullopt;
    }

    cv.notify_all();
}


private:
    std::queue<std::shared_ptr<Person>> demo;
    std::queue<std::shared_ptr<Person>> rep;

    std::condition_variable cv;
    std::mutex mx;

    const int LIMIT;
    int demCount;
    int repCount;

    steady_clock::time_point start;
    steady_clock::duration scheduledExit;
    std::optional<Party> currentParty;
};



// ---------- Testing code ----------

void simulatePerson(BathroomManager& bm, Party party, std::string name, milliseconds useTime) {
    bm.enter(party, name, useTime);
    std::this_thread::sleep_for(useTime); // simulate bathroom usage
    bm.leave(party, name);
}

int main() {
    BathroomManager bm(3);

    std::thread t1(simulatePerson, std::ref(bm), Party::Democrat, "Alice", milliseconds(1500));
    std::thread t2(simulatePerson, std::ref(bm), Party::Democrat, "Bob", milliseconds(1000));
    std::thread t3(simulatePerson, std::ref(bm), Party::Republican, "Charlie", milliseconds(2000));
    std::thread t4(simulatePerson, std::ref(bm), Party::Democrat, "Dana", milliseconds(1200));
    std::thread t5(simulatePerson, std::ref(bm), Party::Republican, "Eve", milliseconds(500));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
