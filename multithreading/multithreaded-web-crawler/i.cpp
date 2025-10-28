// Implement a multi-threaded web crawler. The crawler starts from a given URL and uses multiple threads to explore all
// pages accessible from this starting point. Each URL points to a webpage, and the crawling process retrieves all URLs 
// present on a page to visit them recursively.
// The system will call the crawl with startUrl and you will have to block the calling thread until all the URLs are 
// parsed and then return those URLs.

// Signature:

// class WebCrawler {
// 	List<String> crawl(String startUrl) {
// 		// Your code here
// 	}
// }
// Requirements:
// The crawling process should:

// Avoid revisiting the same URL more than once.
// Crawl only pages belonging to the same hostname as the starting URL.
// Multithreading:

// Utilize multiple threads to speed up the crawling process.
// Ensure thread safety to handle shared data structures (e.g., visited URLs).
// Constraints:

// startUrl is guaranteed to belong to the same hostname for the crawling process.
// URL comparisons should be case-sensitive and exact.
// Key Considerations:
// Thread Safety: Design the solution to avoid race conditions while accessing shared resources like the visited URLs set.
// Hostname Validation: Use the hostname of the startUrl to filter URLs that do not belong to the same domain.
// Concurrency: Efficiently distribute the crawling workload across threads to optimize performance.
#include<bits/stdc++.h>
#include<mutex>
#include<thread>
#include<condition_variable>

using namespace std;

// class WebCrawler 
// {
// 	public: 
// 	int NUM_THREADS = 4;
// 	unordered_map<string, bool>visited;
// 	queue<string>queue;
// 	mutex mx;
// 	condition_variable cv;
// 	atomic<int> activeThreads;
// 	mutex fetchmx;
	
// 	// ----- extra variables for nice printing
// 	unordered_map<thread::id, int> threadColorMap;
// 	mutex colorMapMutex;
// 	atomic<int> nextColorIndex{0};
// 	const string RESET = "\033[0m";
// const vector<string> COLORS = {
// 	"\033[31m", // Red
// 	"\033[32m", // Green
// 	"\033[33m", // Yellow
// 	"\033[34m", // Blue
// 	"\033[35m", // Magenta
// 	"\033[36m", // Cyan
// 	"\033[91m", // Bright Red
// 	"\033[92m", // Bright Green
// 	"\033[93m", // Bright Yellow
// 	"\033[94m", // Bright Blue
// 	"\033[95m", // Bright Magenta
// 	"\033[96m"  // Bright Cyan
// };
// 	WebCrawler() 
// 	{
// 		activeThreads.store(0);
		
// 	}
	
// 	vector<string> crawl(string startUrl) 
// 	{
// 		vector<thread>threads;
// 		vector<string>urls;
// 		{
// 			lock_guard<mutex>lock(mx);
// 			for(int i=0;i<NUM_THREADS;i++) 
// 			{
// 				threads.emplace_back(&WebCrawler::executeCrawl,this, std::ref(urls) );
// 			}
			
// 			queue.push(startUrl);
// 			visited[startUrl] = true;
// 			cv.notify_all();
// 		}
		
// 		for(thread& t: threads) if(t.joinable())t.join();
// 		return urls;
// 	}
	
// 	void executeCrawl(vector<string>& crawledStrings) 
// 	{
// 		while(true){
// 		string url = "";
// 		{
// 			unique_lock<mutex>lock(mx);
// 			cv.wait(lock, [this](){return !queue.empty() || activeThreads.load() == 0;});
// 			if(queue.empty() && activeThreads.load() ==0) {
// 				cv.notify_all();
// 				return;
// 			}
			
// 			activeThreads.fetch_add(1);
// 			url = queue.front();
// 			string color = getThreadColor();
// 			cout << color<< "Thread worker #" << std::this_thread::get_id() << " executed crawl for url: " << url
// 	 		<< RESET << endl;			
// 			crawledStrings.push_back(url);
// 			queue.pop();
// 		}
		
// 		// fetch the urls on this page
// 		const vector<string>urls = fetchForUrl(url);
// 		{
// 			lock_guard<mutex>lock(mx);
// 			for(const string& curUrl: urls) 
// 			{
// 				if(visited.find(curUrl) !=visited.end()) continue;
// 				visited[curUrl] = true;
// 				queue.push(curUrl);
			
// 				cv.notify_one();
// 			}
// 			activeThreads.fetch_sub(1);
// 		}
		
			
// 		}
// 	}
// 	vector<string> fetchForUrl(string& url)
// 	{
// 		lock_guard<mutex>lock(fetchmx);
// 		vector<string> lists;
// 		if(url.length()<7)
// 		for(int i=0;i<10;i++) 
// 		{
// 			lists.emplace_back(url + to_string(i));
// 		}
// 		return lists;
// 	}
// 	string getThreadColor() {
// 	thread::id tid = this_thread::get_id();
// 	lock_guard<mutex> lock(colorMapMutex);
// 	if (threadColorMap.find(tid) == threadColorMap.end()) {
// 		threadColorMap[tid] = nextColorIndex++ % COLORS.size();
// 	}
// 	return COLORS[threadColorMap[tid]];
// }
// };
int main() 
{
	// WebCrawler crawler;
	// vector<string>urls = crawler.crawl("-----");
	// for(string& url : urls) 
	// {
	// 	cout<<url<<endl;
	// }
	string sub = "raks";
	string parent = "jras";
	auto it = parent.find(sub);
	if(it == string::npos) 
	{
		cout<<"Not found"<<endl;
	}
	else cout<<it<<endl;
	return 0;
}

