#include<bits/stdc++.h> 
#include<thread> 
#include<mutex> 
#include<optional> 
#include<vector>

using namespace std; 
template<typename K,typename V> 
class SnapshotMap  
{ 
	public :  
	struct Snapshot 
	{ 
		mutex mx; 
		unordered_map<K, V> map; 
		int snapshotId; 
	}; 
	unordered_map<K, V>map; 
	unordered_map<int,Snapshot>snapshots; 
	int lastSnapshotId=0; 
	mutex mx; 
	 
	void put(K key, V value)  
	{ 
		lock_guard<mutex>lock(mx); 
		map[key] = value; 
		 
	} 
	 
	optional<V> get(K key)  
	{ 
		lock_guard<mutex>lock(mx); 
		if(map.find(key)==map.end())  
		{ 
			return nullopt; 
		} 
		return map[key]; 
	} 
	int takeSnaphot()  
	{ 
		lock_guard<mutex>lock(mx); 

		lastSnapshotId++; 
		Snapshot& sn = snapshots[lastSnapshotId]; 
		sn.snapshotId=lastSnapshotId; 
		for(auto& it: map) 
		{ 
			sn.map.emplace(it.first, it.second); 
		} 
		return sn.snapshotId; 
	} 
	 
	optional<V> getValueFromSnapshot(K key, int snapshotId) 
	{ 
		Snapshot* sn = NULL; 
		mutex* smx = NULL; 
		{ 
			lock_guard<mutex>lock(mx); 
			if(snapshots.find(snapshotId) == snapshots.end())  
			{ 
				return nullopt; 
			} 
			sn = &snapshots[snapshotId]; 
			smx = &(sn->mx); 
		} 
		 
		lock_guard<mutex>lock(*smx); 
		if(sn->map.find(key) == sn->map.end())  
		{ 
			return nullopt; 
		} 
		return sn->map[key]; 
	} 
}; 

// Shared vector to store snapshot IDs for readers to use
vector<int> snapshot_ids;
mutex snapshot_ids_mutex;

// Task for writer threads
void writer_task(SnapshotMap<int, string>& smap, int thread_id) {
    cout << "Writer thread " << thread_id << " started." << endl;
    for (int i = 0; i < 50; ++i) {
        // Each thread writes to its own range of keys
        int key = (thread_id * 100) + i;
        string value = "T" + to_string(thread_id) + "_Val_" + to_string(i);
        smap.put(key, value);
        // Simulate some work
        this_thread::sleep_for(chrono::milliseconds(rand() % 5));
    }
    cout << "Writer thread " << thread_id << " finished." << endl;
}

// Task for the snapshotter thread
void snapshotter_task(SnapshotMap<int, string>& smap) {
    cout << "Snapshotter thread started." << endl;
    for (int i = 0; i < 5; ++i) {
        // Wait for some data to be written before taking a snapshot
        this_thread::sleep_for(chrono::milliseconds(50));
        int id = smap.takeSnaphot();
        
        // Safely add the new snapshot ID to the shared vector
        {
            lock_guard<mutex> lock(snapshot_ids_mutex);
            snapshot_ids.push_back(id);
        }
        cout << "--> Snapshot taken with ID: " << id << endl;
    }
    cout << "Snapshotter thread finished." << endl;
}

// Task for reader threads
void reader_task(SnapshotMap<int, string>& smap, int thread_id) {
    cout << "Reader thread " << thread_id << " started." << endl;
    for (int i = 0; i < 75; ++i) {
        // 1. Try reading from the live map
        smap.get(rand() % 300); // Read a random key

        // 2. Try reading from a random snapshot if one exists
        int snapshot_id_to_read = -1;
        {
            lock_guard<mutex> lock(snapshot_ids_mutex);
            if (!snapshot_ids.empty()) {
                // Pick a random snapshot ID from the available ones
                snapshot_id_to_read = snapshot_ids[rand() % snapshot_ids.size()];
            }
        }

        if (snapshot_id_to_read != -1) {
            int random_key = rand() % 300;
            smap.getValueFromSnapshot(random_key, snapshot_id_to_read);
        }
        
        // Simulate some work
        this_thread::sleep_for(chrono::milliseconds(rand() % 8));
    }
    cout << "Reader thread " << thread_id << " finished." << endl;
}

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