#include <iostream>
#include <mutex>

std::mutex mtx;

int main()
{
    std::unique_lock<std::mutex> lock1(mtx);
    std::cout << "lock1 is owned? " << lock1.owns_lock() << std::endl;
    std::unique_lock<std::mutex> lock2(mtx, std::try_to_lock);
    std::cout << "lock2 is owned? " << lock2.owns_lock() << std::endl;
    std::cout << "finished \n";
    return 0;
}

