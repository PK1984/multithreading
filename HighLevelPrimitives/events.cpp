#include <thread>
#include <iostream>
#include <future>
#include <chrono>

int main()
{
    std::promise<void> p;
    std::future<void> f = p.get_future();
 
    std::thread t([&p]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        p.set_value();
    });

    std::cout << "waiting..." << std::endl;
    f.wait();
    std::cout << "operation finished" << std::endl;

    t.join();
}