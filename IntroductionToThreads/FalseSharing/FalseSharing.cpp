#include <thread>
#include <vector>
#include <iostream>
#include <numeric>
#include <functional>
#include <cstdlib>
#include <chrono>
#include <mutex>
#include <new>

class TimeMe {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::string label_;
    static std::mutex mtx_;
public:
    TimeMe(const std::string& label) : label_(label), start_(std::chrono::high_resolution_clock::now()) {}
    ~TimeMe(){
        auto stop = std::chrono::high_resolution_clock::now();
        std::unique_lock<std::mutex> lock(TimeMe::mtx_);
        std::cout << label_ << " = " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start_).count() << " ms" << std::endl;
    }  
};
std::mutex TimeMe::mtx_;

template <class type>
struct AlignedType {
//    alignas(std::hardware_desctructive_interference_size) type val;
//    alignas(64) type val;
    type val;
    AlignedType(type value) : val(value) {}
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
//    std::cout << sizeof(AlignedType<double>(0));
    auto timer1(TimeMe("Total time"));
    /*    {
        auto timer1(TimeMe("Random generator"));
        fillWithRandomNumbers(vect);
    }   */
    std::vector<int> vect(200000000,9);
    FalseSharingAccumulator<int> accumulator;
    auto timer2(TimeMe("Accumulation time"));
    if ( accumulator.elementwiseSum(vect) == 4567891266 ) std::cout << "Lucky me!"; // just so that the compiler does not optimize everything out
    return 0;
}
