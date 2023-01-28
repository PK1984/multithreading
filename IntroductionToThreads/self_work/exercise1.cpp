#include <iostream>
#include <thread>
#include <vector>
 
/* simple implementation of partial accumulate function with container size checking */
void accumulate(std::vector<int> container, std::size_t start, std::size_t stop, int& accumulator){
    if (container.size() < stop) stop = container.size();
    for (auto i = start; i < stop; ++i) accumulator += container[i];
}
 
int main()
{
    /* get the number of supported threads */
    auto numThreads = std::thread::hardware_concurrency();
     
    /* create input data for algorithm to work on */
    std::vector<int> inputVec(11000,1);
     
    /* create output vector for storing partial sums */
    std::vector<int> outputVec(numThreads);
     
    /* create a vector of threads for easier management */
    std::vector<std::thread> thVec;
     
    /* calculate workload for each thread */
    auto workloadSize = inputVec.size()/numThreads;
     
    for (auto i = 0ul; i < numThreads; ++i) {
        /* calculate boundaries for partial accumulates */
        int start = i * workloadSize;
        int stop = start + workloadSize;
     
        /* start threads */
        thVec.emplace_back(accumulate, inputVec, start, stop, std::ref(outputVec[i]));
    }
     
    /* waiting for all threads to finish execution */
    for (auto i = 0ul; i < numThreads; ++i) thVec[i].join();
   
    /* sequential accumulation of partial sums */
    auto sum = 0;
    accumulate(outputVec, 0, outputVec.size(), sum);
    std::cout << "The sum of all elements in a vector is: " << sum << std::endl;
    return 0;
}
