#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;

void work()
{
    std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);
    std::cout << "Thread " << std::this_thread::get_id() << " started working" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << std::this_thread::get_id() << " finished working" << std::endl;
}

int main()
{
    mtx.lock();
    std::thread t1(work);
    std::thread t2(work);
    mtx.unlock();

    t1.join();
    t2.join();

    return 0;
}
