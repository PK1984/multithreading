#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include <chrono>

int accumulate(
    std::vector<int>::iterator first,
    std::vector<int>::iterator last)
{
    std::cout << __func__ << "-thread id: " << std::this_thread::get_id() << std::endl;
    return std::accumulate(first, last, 0);
}

int main()
{
    std::vector<int> numbers(11, 2);

    auto accumulateFuture =
        //std::async(std::launch::async, &accumulate, numbers.begin(), numbers.end());
        std::async(std::launch::deferred, &accumulate, numbers.begin(), numbers.end());

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout <<  __func__ << "-thread id: " << std::this_thread::get_id() << std::endl;
    std::cout << accumulateFuture.get();
    
    return 0;
}