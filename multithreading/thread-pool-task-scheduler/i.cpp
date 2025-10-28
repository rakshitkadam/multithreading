#include <bits/stdc++.h>
#include <mutex>
#include <semaphore>
#include <atomic>
#include <thread>
#include <condition_variable>

using namespace std;

// class TaskScheduler
// {
// 	public:
// 	struct Task{

// 		int id;
// 		function<void()>executionTask;
// 		int remDep;
// 		vector<int>dependents;
// 	};

// 	unordered_map<int, Task> tasks;
// 	queue<int>queue;
// 	mutex mx;
// 	vector<thread>threads;
// 	condition_variable taskCV;
// 	atomic<int>tasksRem;
// 	int NUM_THREADS = 4;
// 	condition_variable doneCV;
// 	bool shutdown;
// 	void addTask(int id, function<void()>task, vector<int>dependencies)
// 	{
// 		Task t = Task();
// 		t.id = id;
// 		t.executionTask = std::move(task);
// 		t.dependents = {};
// 		t.remDep=(dependencies.size());
// 		{
// 			lock_guard<mutex>lock(mx);
// 			tasks.emplace(id, std::move(t));
// 			for(int dependId : dependencies )
// 			{
// 				tasks[dependId].dependents.push_back(id);
// 			}
// 		}
// 	}

// 	void run()
// 	{
// 		{
// 		lock_guard<mutex>lock(mx);
// 		tasksRem.store(tasks.size());
// 		shutdown=(false);
// 		for (auto &p : tasks) {
// 				if (p.second.remDep == 0) {
// 					queue.push(p.first);
// 				}
// 			}
// 		for(int i=0;i<NUM_THREADS;i++)
// 		{
// 			threads.emplace_back(&TaskScheduler::executeTasks,this);
// 		}
// 		}
// 		taskCV.notify_all();
// 		{
// 			unique_lock<mutex>lock(mx);
// 			doneCV.wait(lock, [this](){return tasksRem.load()==0;});
// 		}
// 		{
// 			lock_guard<mutex>lock(mx);
// 			shutdown=true;
// 			taskCV.notify_all();
// 		}
// 		for(thread& t : threads)
// 		{
// 			if(t.joinable())
// 			{
// 				t.join();
// 			}
// 		}
// 		threads.clear();

// 	}
// 	void executeTasks()
// 	{
// 		while(true)
// 		{	Task* taskptr = NULL;
// 			{
// 				unique_lock<mutex> lock(mx);
// 				taskCV.wait(lock,[this](){return !queue.empty() || shutdown || tasksRem.load()==0;});
// 				if(shutdown) {return;}
// 				if(tasksRem.load()==0 && queue.size()==0)return ;
// 				if(queue.empty())
// 				{
// 					continue; //spurious wakeup
// 				}
// 				int taskId = queue.front();
// 				queue.pop();
// 				taskptr = &tasks[taskId];

// 			}
// 			taskptr->executionTask();
// 			int remTasks = 0;
// 			{
// 				unique_lock<mutex>lock(mx);
// 				for(int depId : taskptr->dependents)
// 				{
// 					tasks[depId].remDep -= 1;
// 					int rem = tasks[depId].remDep;
// 					if(rem==0)
// 					{
// 						queue.push(depId);
// 					}
// 				}

// 				if(queue.size())
// 				{
// 					taskCV.notify_one();
// 				}
// 				remTasks = tasksRem.fetch_sub(1);
// 			}
// 			{
// 				lock_guard<mutex>lock(mx);
// 				if(remTasks == 1)
// 				doneCV.notify_one();
// 			}
// 		}
// 	}

// };

class TaskScheduler 
{
	
	public:
	
	struct Task 
	{
		int id;
		function<void()>task;
		vector<int>dependents;
		int rem;
	};
	
	mutex mx;
	condition_variable cv;
	queue<shared_ptr<Task>>queue;
	unordered_map<int, shared_ptr<Task> > map;
	int tasks = 0;
	int workingTasks = 0;
	void task(int id, function<void()>task, vector<int> dependencies) 
	{
		lock_guard<mutex>lock(mx);
		shared_ptr<Task> curTask = make_shared<Task>();
		curTask->id = id;
		curTask->task = std::move(task);
		curTask->rem = dependencies.size();
		for(int dep: dependencies) 
		{
			map[dep]->dependents.push_back(id);
		}
		map[id] = curTask;
		tasks++;
	}
	
	void run() 
	{
		unique_lock<mutex>lock(mx);
		for(auto it: map) 
		{
			if(it.second->rem == 0) 
			{
				queue.push(it.second);
			}
		}
		vector<thread>threads;
		for(int i=0;i<std::thread::hardware_concurrency(); i++) 
		{
			threads.emplace_back(&TaskScheduler::execute, this);
		}
		
		cv.notify_all();
		
		for(thread& t: threads) if(t.joinable()) t.join();
	}
	
	void execute() 
	{
		shared_ptr<Task> curTask;
		{
			unique_lock<mutex>lock(mx);
			cv.wait(lock,[this]()
			{
				return queue.size() || tasks==0;
			});
			if(tasks==0)
			{
				cv.notify_all();
				return;
			}
			curTask = queue.front();
			tasks--;
			queue.pop();
			
			cv.notify_one();
			
		}
		
		curTask->task();
		
		{
			unique_lock<mutex>lock(mx);
			for(int dep: curTask->dependents) 
			{
				map[dep]->rem--;
				if(map[dep]->rem==0)
				{
					queue.push(map[dep]);
					cv.notify_one();
				}
			}
		}
	}
};

int main()
{
	// TaskScheduler scheduler;
	// scheduler.addTask(1, [](){ std::this_thread::sleep_for(std::chrono::milliseconds(800));std::cout << "Task 1\n"; }, {});
	// scheduler.addTask(2, [](){ std::this_thread::sleep_for(std::chrono::milliseconds(200));std::cout << "Task 2\n"; }, {1});
	// scheduler.addTask(3, [](){ std::this_thread::sleep_for(std::chrono::milliseconds(100));cout << "Task 3\n"; }, {1});
	// scheduler.addTask(4, [](){ std::cout << "Task 4\n"; }, {2, 3});

	// scheduler.run();
	// ifstream file("address of file");
	// if(file.is_open())
	// {
	// 	file.close();
	// }
	
	// ofstream fil("address of file");
	// if(file.is_open()) 
	// {
	// 	string s;
	// 	while(getline(file, s,))
	// }
	return 0;
}
