#include <chrono>
#include <iostream>
#include <mutex>
#include <new>
#include <numeric>
#include <thread>
#include <vector>

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

/* at the cost of additional memory, one can assure that no false sharing takes place
 * here, the value stored in the structure (int in below example) is aligned to
 * the cache line size (on modern architectures 64 bytes) */
template <class type>
struct AlignedType {
    alignas(std::hardware_destructive_interference_size) type val;
//    alignas(64) type val;
    AlignedType(type value) : val(value) {}
};

/* the structure that holds the accumulators for all the threads uses default alignment
 * this may result in writing to the same cache line by all the threads, incurring performance penalties */
template <class type>
struct UnalignedType {
    type val;
    UnalignedType(type value) : val(value) {}
};

/* std::accumulate with -O0 is a tad slow, for fair comparision between
 * single and multi-threaded accumulate, the below code is provided */
template <class type>
int accumulate(const std::vector<type>& input) {
	type accu{0};
	for (auto i = 0ul; i < input.size(); ++i) accu += input[i];
	return accu;
}

template <class alignmentType, class elementType>
class FalseSharingAccumulator {
private:
    std::size_t numThreads_;
    void partialSum(const std::vector<elementType>& input, const std::size_t start, const std::size_t stop, elementType& accumulator) {
        std::size_t lastElement = stop;
        if (input.size() < stop ) lastElement = input.size();
        for (auto i = start; i < lastElement; ++i ) accumulator += input[i];
    }
public:
    FalseSharingAccumulator() : numThreads_(std::thread::hardware_concurrency()) {}
    elementType elementwiseSum(std::vector<elementType>& input)
    {
        /* partialProduct is a vector containing per thread accumulators */
        std::vector<alignmentType> partialProduct(numThreads_,0);
        std::cout << "\nThe output structure is aligned to: " << sizeof(alignmentType(0)) << " bytes" << std::endl;

        /* managing and starting threads */
        std::vector<std::thread> threads;
        int batchSize = input.size()/numThreads_ + 1;
        for (auto i = 0ul; i < numThreads_; ++i) {
            std::size_t start = i * batchSize;
            std::size_t stop = ( i + 1 ) * batchSize;
            threads.emplace_back(&FalseSharingAccumulator::partialSum, this, std::cref(input), start, stop, std::ref(partialProduct[i].val));
        }
        /* join all threads */
        for (auto i = 0ul; i < numThreads_; ++i) threads[i].join();

        /* the final accumulate is sequential, no point in making it parallel, it is too fast */
        return std::accumulate(partialProduct.begin(), partialProduct.end(), 0,
            [](const auto sum, const auto& next){
                return sum + next.val;
            });
    }
};

int main(){
    /* get total execution time */
    //auto timer1(TimeMe("Total execution time"));

    /* initialize container used for input data storage */
    const int numElements = 100000007;
    const int initialValue = 3;
    std::vector<int> vect(numElements,initialValue);

    /* initialize aligned accumulator, run it and time it */
    {
        FalseSharingAccumulator<AlignedType<int>, int> accumulator;
        auto timer(TimeMe("Multithreaded accumulate"));
        if ( accumulator.elementwiseSum(vect) != initialValue * numElements ) std::cout << "Smth went wrong";
    }

    /* initialize unaligned accumulator, run it and time it */
    {
        FalseSharingAccumulator<UnalignedType<int>, int> accumulator;
        auto timer(TimeMe("Multi-threaded accumulate"));
        if ( accumulator.elementwiseSum(vect) != initialValue * numElements ) std::cout << "Smth went wrong";
    }

    {
		auto timer(TimeMe("\nSingle-threaded custom accumulate"));
		if (accumulate(vect) != initialValue * numElements ) std::cout << "Smth went wrong";
	}
    {
		auto timer(TimeMe("Single-threaded std::accumulate"));
		if (std::accumulate(vect.begin(), vect.end(), 0) != initialValue * numElements ) std::cout << "Smth went wrong";
	}

    return 0;
}
