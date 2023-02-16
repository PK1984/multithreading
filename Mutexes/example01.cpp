#include <iostream>
#include <thread>
#include <memory>
#include <mutex>

class Singleton {
  public:
    static std::shared_ptr<Singleton> getInstance() {
        if (!instance_) instance_ = std::shared_ptr<Singleton>(new Singleton);
        return instance_;
    }

  private:
    Singleton() {};
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static std::shared_ptr<Singleton> instance_;
};

std::shared_ptr<Singleton> Singleton::instance_ = nullptr;

std::mutex mtx; // only for protecting std::cout
void workerFunction() {
    std::shared_ptr<Singleton> singleton = Singleton::getInstance();
    std::lock_guard lock(mtx);
    std::cout << "Thread ID: " << std::this_thread::get_id() << " Instance: " << singleton.get() << std::endl;
}

int main() {
    std::thread t1(workerFunction);
    std::thread t2(workerFunction);
    std::thread t3(workerFunction);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}  
