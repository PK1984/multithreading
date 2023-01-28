#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <random>
#include <chrono>

class TimeMe {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::string label_;
    static std::mutex mtx_;
public:
    TimeMe(const std::string& label) : start_(std::chrono::high_resolution_clock::now()), label_(label) {}
    ~TimeMe(){
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_);
        std::unique_lock<std::mutex> lock(TimeMe::mtx_);
        std::cout << label_ << " = " << duration.count() << " us" << std::endl;
    }  
};
std::mutex TimeMe::mtx_;

std::vector<int> getRandomData(const int size){
    std::vector<int> output(size);
    std::generate(output.begin(), output.end(), std::rand);
    return output;
}

void sorter(std::vector<int>& vect, const int start, const int stop, const int id)
{
    auto timer = TimeMe("Time spent in worker " + std::to_string(id));
    std::sort(vect.begin() + start, vect.begin() + stop);
}

std::vector<int> sequentialSorter(std::vector<int> vect){
    auto timer = TimeMe("Time spent on sequential sorting");
    std::sort(vect.begin(), vect.end());
    return vect;
}



int main(){
    constexpr int iterations = 100000;
    auto timer = TimeMe("Iterations (" + std::to_string(iterations)+")");
    auto lambda = [](){};
    for (auto i = 0; i < iterations; ++i)
    {
        auto th = std::thread(lambda);
        th.detach();
    }
    return 0;
}
