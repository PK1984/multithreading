/* The following code looks for a needle in a haystack.
 * Improve the following code so that when the needle is found all the threads stop execution.
 * Compare the code with and without optimizations (-O0 ... -O3)
 * Does the global variable `found` need to be an atomic? */

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <chrono>

class TimeMe {
private:
    std::string label_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    static std::mutex mtx_;
public:
    TimeMe(const std::string& label) : label_(label), start_(std::chrono::high_resolution_clock::now()) {}
    ~TimeMe(){
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_);
        std::unique_lock<std::mutex> lock(TimeMe::mtx_);
        std::cout << label_ << " = " << duration.count() << " ms" << std::endl;
    }  
};
std::mutex TimeMe::mtx_;

std::atomic<bool> found{false};

void search(const std::vector<int>& input, std::size_t start, std::size_t workSize, const int needle){
	auto stop = input.size() > start + workSize ? start + workSize : input.size();
	if (std::find(input.begin() + start, input.begin() + stop, needle) != input.end()) found = true;
}

int main(){
	auto numThreads = 4;
	std::size_t haystackSize = 100000000;
	std::vector<int> haystack(haystackSize);
	std::iota(haystack.begin(), haystack.end(), 0);
	std::srand(std::time(nullptr));
	
	auto workSize = haystack.size() / numThreads + 1;
	auto needle = haystackSize / numThreads / 2;
	std::cout << " looking for : " << needle << std::endl;
	std::thread th1(search, std::cref(haystack), 0 * workSize, workSize, needle);
	std::thread th2(search, std::cref(haystack), 1 * workSize, workSize, needle);
	std::thread th3(search, std::cref(haystack), 2 * workSize, workSize, needle);
	std::thread th4(search, std::cref(haystack), 3 * workSize, workSize, needle);
	
	{
		auto timer = TimeMe("Waiting");
		while (!found) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
		//~ while (!found) {  std::this_thread::yield();}
	}
	{
		auto timer = TimeMe("Joining");
		th1.join(); th2.join(); th3.join(); th4.join(); 
	}
	return 0;
}
