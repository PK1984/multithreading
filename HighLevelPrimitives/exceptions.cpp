#include <thread>
#include <iostream>
#include <future>
#include <chrono>

int main()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();
 
    std::thread t([&p](){
        try
        {
            throw std::runtime_error("error");
        }
        catch(...)
        {
            p.set_exception(std::current_exception());
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    try
    {
        std::cout << f.get();
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception from the thread: " << e.what() << '\n';
    }
    t.join();
}
