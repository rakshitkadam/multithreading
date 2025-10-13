

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

class BathroomManager 
{
	public:
	condition_variable rcv;
	condition_variable dcv;
	mutex mx;
	int LIMIT = 3;
	queue<string>dq;
	queue<string>rq;
	int dcount = 0;
	int rcount = 0;
	enum Party{NONE, DEMOCRAT, REPUBLIC} currentParty = NONE;
	Party nextParty = NONE;
	// Called by a Democrat thread to request entry.
	void democratEnters(string name) 
	{
		unique_lock<mutex>lock(mx);
		dq.push(name);  // Add to queue first!
		
		dcv.wait(lock, [this,name]()
		{
			return (currentParty==NONE || currentParty==DEMOCRAT) 
					&& dcount<LIMIT && !dq.empty() && dq.front() == name && (nextParty == NONE || nextParty == DEMOCRAT);
		});
		
		cout<<"Democrat with name :" + name + " entered"<<endl;
		dq.pop();
		currentParty = DEMOCRAT;
		dcount++;
		
	}

	// Called by a Democrat thread when it leaves.
	void democratLeaves(string name) 
	{
		unique_lock<mutex>lock(mx);
		cout<<"Democrat with name :" + name + " leaves"<<endl;
		dcount--;
		if(dcount==0) 
		{
			currentParty = NONE;
			if(!rq.empty())  // Check if Republicans are waiting!
			{
				nextParty = REPUBLIC;
			}
			else 
			{
				nextParty = NONE;
			}
		}
		lock.unlock();
		dcv.notify_all();
		rcv.notify_all();
		
	}

	// Called by a Republican thread to request entry.
	void republicanEnters(string name) 
	{
		unique_lock<mutex>lock(mx);
		rq.push(name);  // Add to queue first!
		
		rcv.wait(lock, [this,name]()
		{
			return (currentParty==NONE || currentParty==REPUBLIC) 
					&& rcount<LIMIT && !rq.empty() && rq.front() == name && (nextParty == NONE || nextParty == REPUBLIC);
		});
		
		cout<<"Republic with name :" + name + " entered"<<endl;
		rq.pop();
		currentParty = REPUBLIC;
		rcount++;
	}

	// Called by a Republican thread when it leaves.
	void republicanLeaves(string name) {
		unique_lock<mutex>lock(mx);
		cout<<"Republic with name :" + name + " leaves"<<endl;
		rcount--;
		if(rcount==0) 
		{
			currentParty = NONE;
			if(!dq.empty())  // Check if Democrats are waiting!
			{
				nextParty = DEMOCRAT;
			}
			else 
			{
				nextParty = NONE;
			}
		}
		lock.unlock();
		dcv.notify_all();
		rcv.notify_all();
		
	}
	


};

int f(const string &name) {
	// Different durations for each person
	if (name == "D1") return 3;
	if (name == "D2") return 4;
	if (name == "R1") return 5;
	if (name == "R2") return 3;
	if (name == "D3") return 2;
	if (name == "D4") return 6;
	if (name == "R3") return 4;
	return 3;
}
void democrat(BathroomManager &bathroom, string name) {
	bathroom.democratEnters(name);
	this_thread::sleep_for(chrono::seconds(f(name))); // using the bathroom
	bathroom.democratLeaves(name);
}

void republican(BathroomManager &bathroom, string name) {
	bathroom.republicanEnters(name);
	this_thread::sleep_for(chrono::seconds(f(name))); // using the bathroom
	bathroom.republicanLeaves(name);
}
int main()
{
	BathroomManager bathroom;

	// Example input queue
	vector<string> queue = {"D1", "D2", "R1", "R2", "D3", "D4", "R3"};

	// Create threads in arrival order
	vector<thread> threads;
	for (auto &person : queue) {
		if (person[0] == 'D')
			threads.emplace_back(democrat, ref(bathroom), person);
		else
			threads.emplace_back(republican, ref(bathroom), person);
		this_thread::sleep_for(chrono::milliseconds(300)); // simulate staggered arrivals
	}

	// Wait for all to finish
	for (auto &t : threads) t.join();

	cout << "\nAll finished." << endl;
	return 0;
}