#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <numeric>

void accumulate(
    std::vector<int>::iterator first,
    std::vector<int>::iterator last,
    std::promise<int>& accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);
}

int main()
{
    std::vector<int> numbers(11, 2);
    std::promise<int> accumulatePromise;
    std::future<int> accumulateFuture = accumulatePromise.get_future();

    std::thread t(accumulate, numbers.begin(), numbers.end(), std::ref(accumulatePromise));

    auto validityCheck = [&accumulateFuture]() {
        auto state = accumulateFuture.valid() ? "valid" : "invalid";
        std::cout << "future object is " << state << std::endl;
    };

    validityCheck();
    std::cout << "result: " << accumulateFuture.get() << std::endl;
    validityCheck();
    
    t.join();
    return 0;
}
