#include <iostream>
#include <future>
#include <vector>
#include <numeric>

int main()
{
    std::vector<int> numbers(11, 2);

    auto accumulateFuture =
        std::async(std::launch::async, &std::accumulate<std::vector<int>::const_iterator, int>, numbers.begin(), numbers.end(), 0);

    std::cout << accumulateFuture.get();
    
    return 0;
}