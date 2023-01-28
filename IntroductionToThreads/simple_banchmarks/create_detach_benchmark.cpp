#include <iostream>
#include <mutex>
#include <thread>
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
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_);
        std::unique_lock<std::mutex> lock(TimeMe::mtx_);
        std::cout << label_ << " = " << duration.count() << " us" << std::endl;
    }  
};
std::mutex TimeMe::mtx_;

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
