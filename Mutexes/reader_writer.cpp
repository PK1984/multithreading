#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <cstdlib>

std::shared_mutex mutex;
int shared_data = 0;

void reader_thread()
{
    while (true) {
        std::shared_lock<std::shared_mutex> lock(mutex);
        std::cout << "Reader thread: Data = " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //lock.unlock();
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void writer_thread()
{
    while (true) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        shared_data = rand() % 100;
        std::cout << "Writer thread: Data = " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::thread reader1(reader_thread);
    std::thread reader2(reader_thread);
    std::thread writer(writer_thread);

    reader1.join();
    reader2.join();
    writer.join();

    return 0;
}
