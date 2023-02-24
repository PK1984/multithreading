#include <iostream>
#include <future>
#include <vector>
#include <numeric>
#include <chrono>

int main()
{
    std::vector<int> numbers(11, 2);

    auto accumulateFuture =
        std::async(&std::accumulate<std::vector<int>::const_iterator, int>, numbers.begin(), numbers.end(), 0);

    while (std::future_status::ready != accumulateFuture.wait_for(std::chrono::milliseconds(1000)))
    {
        std::cout << "waiting..." << std::endl;
    }

    std::cout << accumulateFuture.get();
    
    return 0;
}