#include <chrono>
#include <iostream>
#include <mutex>
#include <new>
#include <numeric>
#include <thread>
#include <vector>

class TimeMe {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::string label_;
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

template <class type>
struct AlignedType {
//    alignas(std::hardware_desctructive_interference_size) type val;
    alignas(64) type val;
    AlignedType(type value) : val(value) {}
};

template <class type>
struct UnalignedType {
    type val;
    UnalignedType(type value) : val(value) {}
};

template <class alignmentType, class elementType>
class FalseSharingAccumulator {
private:
    std::size_t numThreads_;
    void partialSum(std::vector<elementType>& input, const int start, const int stop, elementType& accumulator) {
        int lastElement = stop;
        if (input.size() < stop ) lastElement = input.size();
        for (auto i = start; i < lastElement; ++i ) accumulator += input[i];
    }
public:
    FalseSharingAccumulator() : numThreads_(std::thread::hardware_concurrency()) {}
    elementType elementwiseSum(std::vector<elementType>& input)
    {
        std::vector<alignmentType> partialProduct(numThreads_,0);
        std::cout << "The output structure is aligned to: " << sizeof(alignmentType(0)) << " bytes" << std::endl;

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
};

int main(){
    /* get total execution time */
    auto timer1(TimeMe("Total execution time"));

    /* initialize container used for input data storage */
    const int numElements = 200000000;
    const int initialValue = 7;
    std::vector<int> vect(numElements,initialValue);

    /* initialize accumulator, run it and time it */
    {
        FalseSharingAccumulator<AlignedType<int>, int> accumulator;
        auto timer(TimeMe("Multithreaded accumulate"));
        if ( accumulator.elementwiseSum(vect) == 567 ) std::cout << "Guessed it! Lucky me!"; // using the output so that the compiler does not optimize too much
    }

    /* initialize accumulator, run it and time it */
    {
        FalseSharingAccumulator<UnalignedType<int>, int> accumulator;
        auto timer(TimeMe("Multithreaded accumulate"));
        if ( accumulator.elementwiseSum(vect) == 567 ) std::cout << "Guessed it! Lucky me!"; // using the output so that the compiler does not optimize too much
    }

    return 0;
}
