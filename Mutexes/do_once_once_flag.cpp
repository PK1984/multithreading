#include <iostream>
#include <thread>
#include <mutex>

std::once_flag flag;

void do_once() { std::cout << "This function will only be called once, even if multiple threads call it." << std::endl; }
void thread_func() { std::call_once(flag, do_once); }

int main()
{
    std::thread t1(thread_func);
    std::thread t2(thread_func);
    std::thread t3(thread_func);

    t1.join(); t2.join(); t3.join();

    return 0;
}
