#include<bits/stdc++.h> 
#include<thread> 
#include<mutex> 
#include<optional> 
#include<vector>

using namespace std; 
// template<typename K,typename V> 
// class SnapshotMap  
// { 
// 	public :  
// 	struct Snapshot 
// 	{ 
// 		mutex mx; 
// 		unordered_map<K, V> map; 
// 		int snapshotId; 
// 	}; 
// 	unordered_map<K, V>map; 
// 	unordered_map<int,Snapshot>snapshots; 
// 	int lastSnapshotId=0; 
// 	mutex mx; 
	 
// 	void put(K key, V value)  
// 	{ 
// 		lock_guard<mutex>lock(mx); 
// 		map[key] = value; 
		 
// 	} 
	 
// 	optional<V> get(K key)  
// 	{ 
// 		lock_guard<mutex>lock(mx); 
// 		if(map.find(key)==map.end())  
// 		{ 
// 			return nullopt; 
// 		} 
// 		return map[key]; 
// 	} 
// 	int takeSnaphot()  
// 	{ 
// 		lock_guard<mutex>lock(mx); 

// 		lastSnapshotId++; 
// 		Snapshot& sn = snapshots[lastSnapshotId]; 
// 		sn.snapshotId=lastSnapshotId; 
// 		for(auto& it: map) 
// 		{ 
// 			sn.map.emplace(it.first, it.second); 
// 		} 
// 		return sn.snapshotId; 
// 	} 
	 
// 	optional<V> getValueFromSnapshot(K key, int snapshotId) 
// 	{ 
// 		Snapshot* sn = NULL; 
// 		mutex* smx = NULL; 
// 		{ 
// 			lock_guard<mutex>lock(mx); 
// 			if(snapshots.find(snapshotId) == snapshots.end())  
// 			{ 
// 				return nullopt; 
// 			} 
// 			sn = &snapshots[snapshotId]; 
// 			smx = &(sn->mx); 
// 		} 
		 
// 		lock_guard<mutex>lock(*smx); 
// 		if(sn->map.find(key) == sn->map.end())  
// 		{ 
// 			return nullopt; 
// 		} 
// 		return sn->map[key]; 
// 	} 
// }; 

class SnapshotMap {
public:
	atomic<int>last{0};
	struct KeyData
	{
		map<int,int>map;
		mutex mx;
	};
	unordered_map<int, shared_ptr<KeyData>> map;
	mutex mx;
	void put(int key, int value) 
	{
		shared_ptr<KeyData> data;
		{
			unique_lock<mutex>lock(mx);
			if(map.find(key) == map.end()) 
			{
				shared_ptr<KeyData> newEntry = make_shared<KeyData>();
				map[key] = newEntry;
			}
			data = map[key];
		}
		unique_lock<mutex>lock(data->mx);
		data->map[last] = value;
	}

	// Retrieve the current value for a key
	std::optional<int> get(int key) 
	{
		shared_ptr<KeyData>data;
		{
			unique_lock<mutex>lock(mx);
			if(map.find(key) == map.end()) return nullopt;
			data = map[key];
		}
		unique_lock<mutex>lock(data->mx);
		if(data->map.size()==0)return nullopt;
		return data->map.rbegin()->second;
	}

	// Take a snapshot and return its unique snapshot ID
	int takeSnapshot() 
	{
		return last.fetch_add(1);
	}

	// Retrieve the value of a key from a specific snapshot
	std::optional<int> getValueFromSnapshot(int key, int snapshotId) 
	{
		shared_ptr<KeyData> data;
		{
			unique_lock<mutex>lock(mx);
			if(map.find(key) == map.end()) return nullopt;
			data = map[key];
		}
		unique_lock<mutex>lock(data->mx);
		auto it = data->map.upper_bound(snapshotId);
		if(it == data->map.begin()) return nullopt;
		it--;
		return it->second;
	}
};
// template<typename K, typename V>
// class SnapshotMap 
// {
// 	public: 
// 	struct Snapshot
// 	{
// 		int id;
// 		unordered_map<K,V> map;
// 		mutex mx;
// 	};
// 	mutex mx;
// 	condition_variable cv;
// 	unordered_map<K,V>map;
// 	unordered_map<int,shared_ptr<Snapshot>>snapshots;
// 	atomic<int>lastSnapshot{0};
// 	void put(K key, V value) 
// 	{
// 		unique_lock<mutex>lock(mx);
// 		map[key] = value;
// 	}
// 	std::optional<V> get(K key)
// 	{
// 		unique_lock<mutex>lock(mx);
// 		if(map.find(key) == map.end()) return nullopt;
// 		return map[key];
// 	}
// 	int takeSnaphot() 
// 	{
// 		shared_ptr<Snapshot> s = make_shared<Snapshot>();
// 		s->id = lastSnapshot.fetch_add(1);
		
// 		{
// 			unique_lock<mutex>lock(mx);
// 			for(auto& it: map) 
// 			{
// 				s->map.emplace(it.first, it.second);
// 			}
// 			int sid = s->id;
// 			snapshots.emplace(s->id, std::move(s));
// 			return sid;
// 		}
		
// 	}
// 	std::optional<V> getValueFromSnapshot(K key, int snapshotId) 
// 	{
// 		shared_ptr<Snapshot> s;
// 		{
// 			unique_lock<mutex>lock(mx);
// 			if(snapshots.find(snapshotId) == snapshots.end()) return nullopt;
// 			s = snapshots[snapshotId];
// 		}
		
// 		unique_lock<mutex>lock(s->mx);
// 		if(s->map.find(key) == s->map.end()) return nullopt;
// 		return s->map[key];
// 	}
// };
// // Shared vector to store snapshot IDs for readers to use
// vector<int> snapshot_ids;
// mutex snapshot_ids_mutex;

// // Task for writer threads
// void writer_task(SnapshotMap<int, string>& smap, int thread_id) {
// 	cout << "Writer thread " << thread_id << " started." << endl;
// 	for (int i = 0; i < 50; ++i) {
// 		// Each thread writes to its own range of keys
// 		int key = (thread_id * 100) + i;
// 		string value = "T" + to_string(thread_id) + "_Val_" + to_string(i);
// 		smap.put(key, value);
// 		// Simulate some work
// 		this_thread::sleep_for(chrono::milliseconds(rand() % 5));
// 	}
// 	cout << "Writer thread " << thread_id << " finished." << endl;
// }

// // Task for the snapshotter thread
// void snapshotter_task(SnapshotMap<int, string>& smap) {
// 	cout << "Snapshotter thread started." << endl;
// 	for (int i = 0; i < 5; ++i) {
// 		// Wait for some data to be written before taking a snapshot
// 		this_thread::sleep_for(chrono::milliseconds(50));
// 		int id = smap.takeSnaphot();
		
// 		// Safely add the new snapshot ID to the shared vector
// 		{
// 			lock_guard<mutex> lock(snapshot_ids_mutex);
// 			snapshot_ids.push_back(id);
// 		}
// 		cout << "--> Snapshot taken with ID: " << id << endl;
// 	}
// 	cout << "Snapshotter thread finished." << endl;
// }

// // Task for reader threads
// void reader_task(SnapshotMap<int, string>& smap, int thread_id) {
// 	cout << "Reader thread " << thread_id << " started." << endl;
// 	for (int i = 0; i < 75; ++i) {
// 		// 1. Try reading from the live map
// 		smap.get(rand() % 300); // Read a random key

// 		// 2. Try reading from a random snapshot if one exists
// 		int snapshot_id_to_read = -1;
// 		{
// 			lock_guard<mutex> lock(snapshot_ids_mutex);
// 			if (!snapshot_ids.empty()) {
// 				// Pick a random snapshot ID from the available ones
// 				snapshot_id_to_read = snapshot_ids[rand() % snapshot_ids.size()];
// 			}
// 		}

// 		if (snapshot_id_to_read != -1) {
// 			int random_key = rand() % 300;
// 			smap.getValueFromSnapshot(random_key, snapshot_id_to_read);
// 		}
		
// 		// Simulate some work
// 		this_thread::sleep_for(chrono::milliseconds(rand() % 8));
// 	}
// 	cout << "Reader thread " << thread_id << " finished." << endl;
// }

int main() { 
	srand(time(0)); // Seed for random number generation
	SnapshotMap<int, string> smap; 
	vector<thread> threads;

	cout << "Starting multithreaded test..." << endl;

	// Launch 3 writer threads
	for (int i = 0; i < 3; ++i) {
		threads.emplace_back(writer_task, ref(smap), i + 1);
	}
	
	// Launch 1 snapshotter thread
	threads.emplace_back(snapshotter_task, ref(smap));

	// Launch 5 reader threads
	for (int i = 0; i < 5; ++i) {
		threads.emplace_back(reader_task, ref(smap), i + 1);
	}

	// Wait for all threads to complete their execution
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	cout << "\nAll threads have finished." << endl;
	
	// Verification: Check a value from the final map
	optional<string> val = smap.get(105); // This key should have been written by writer thread 1
	if (val) {
		cout << "Final value for key 105 is: '" << *val << "'" << endl;
	} else {
		cout << "Final value for key 105 not found." << endl;
	}

	// Verification: Check a value from the first snapshot
	if(!snapshot_ids.empty()) {
		optional<string> snap_val = smap.getValueFromSnapshot(5, snapshot_ids[0]); // Key 5 from writer 1
		if(snap_val) {
			cout << "Value for key 5 in snapshot " << snapshot_ids[0] << " is: '" << *snap_val << "'" << endl;
		} else {
			cout << "Value for key 5 not found in snapshot " << snapshot_ids[0] << " (This is possible depending on timing)." << endl;
		}
	}
	
	return 0; 
}
