#include <thread>
#include <vector>
#include <iostream>
#include <numeric>
#include <functional>
#include <cstdlib>
#include <chrono>
#include <mutex>

class TimeMe {
    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop_;
    std::string label_;
    static std::mutex mtx_;
    public:
    TimeMe(const std::string& label) : label_(label), start_(std::chrono::high_resolution_clock::now()) {}
    ~TimeMe(){
        stop_ = std::chrono::high_resolution_clock::now();
        std::unique_lock<std::mutex> lock(TimeMe::mtx_);
        std::cout << label_ << " = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_ - start_).count() << " ms" << std::endl;
    }  
};
std::mutex TimeMe::mtx_;

template <class t>
struct AlignedType {
    alignas(64) t val;
    AlignedType(int value) : val(value) {}
};

template <class elementType>
class FalseSharingAccumulator {
    private:
    std::size_t numThreads_;
    void partialSum(std::vector<elementType>& input, const int start, const int stop, elementType& accumulator) {
        int lastElement = stop;
        if (input.size() < stop ) lastElement = input.size();
        //accumulator = std::accumulate(input.begin() + start, input.begin() + lastElement, 0);
        for (auto i = start; i < lastElement; ++i ) accumulator += input[i];
    }
    public:
    FalseSharingAccumulator() : numThreads_(std::thread::hardware_concurrency()) {}
    elementType elementwiseSum(std::vector<elementType>& input){
        auto b = std::chrono::high_resolution_clock::now();
        std::vector<AlignedType<elementType>> partialProduct(numThreads_,0);
        std::vector<std::thread> threads;
        int batchSize = input.size()/numThreads_ + 1;
        for (auto i = 0; i < numThreads_; ++i) {
            int start = i * batchSize;
            int stop = ( i + 1 ) * batchSize;
            threads.emplace_back(&FalseSharingAccumulator::partialSum, this, std::ref(input), start, stop, std::ref(partialProduct[i].val));
        }
        for (auto i = 0; i < numThreads_; ++i)
            threads[i].join();
        auto e = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(e-b);
        std::cout << "Duration " << duration.count() << "ms" << std::endl;
        return std::accumulate(partialProduct.begin(), partialProduct.end(), 0,
            [](const auto sum, const auto& next){
                return sum + next.val;
            });
    }
    int getNumThreads(){ return numThreads_;}
};

template <class t>
void fillWithRandomNumbers(std::vector<t>& vect){
    for (auto it = vect.begin(); it != vect.end(); ++it){ *it = rand();}
}
int main(){
    auto timer1(TimeMe("Total time"));
    std::vector<int> vect(1500000000);
    {
        auto timer1(TimeMe("Random generator"));
        fillWithRandomNumbers(vect);
    }
    FalseSharingAccumulator<int> accumulator;
    auto timer2(TimeMe("Accumulation time"));
    std::cout << accumulator.elementwiseSum(vect) << std::endl;
    return 0;
}
