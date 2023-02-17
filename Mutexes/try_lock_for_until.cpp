#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

std::timed_mutex g_mutex;

void work(int id){ // Do some work while holding the lock
    std::cout << "Worker " << id << " is working \n";
    std::this_thread::sleep_for(std::chrono::seconds(10));	
}
void workerInfinity(){
	g_mutex.lock();
	std::cout << "WorkerInfinity acquired lock\n";
	std::this_thread::sleep_for(std::chrono::seconds(100000));
	g_mutex.unlock();
}

void worker1(int id, std::chrono::milliseconds timeout) {
    std::cout << "Worker " << id << " will try to acquire lock \n";
	if (g_mutex.try_lock_for(timeout)) {
		std::cout << "Worker " << id << " acquired lock\n";
		work(id);
		g_mutex.unlock();
		std::cout << "Worker " << id << " released lock \n";
	} else {
		std::cout << "Worker " << id << " failed to acquire lock \n";
    }

}
template<class clock>
void worker2(int id, std::chrono::time_point<clock> timePoint) {
    std::cout << "Worker " << id << " will try to acquire lock \n";
	if (g_mutex.try_lock_until(timePoint)) 
	{
		std::cout << "Worker " << id << " acquired lock \n";
		work(id); // what if work throws? -> use lock_guard/scoped_lock
		g_mutex.unlock(); // release the lock
		std::cout << "Worker " << id << " released lock \n";
	} else {
		std::cout << "Worker " << id << " failed to acquire lock\n";
	}
}

int main() {
    std::chrono::milliseconds timeout(1000);
    //std::thread t1(worker1, 1, timeout);
    std::thread t0(workerInfinity);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); //not a proper sync method

    auto end_time = std::chrono::system_clock::now() + timeout*20;
    std::thread t2(worker2<std::chrono::system_clock>, 2, end_time);
    
    //std::thread t1(worker1, 1, timeout*10); 

	t0.join();
    t2.join();
    //t1.join();
    return 0;
}
