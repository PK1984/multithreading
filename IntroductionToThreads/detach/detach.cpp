#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
 
/* access to cout in print() is protected by a mutex */
std::mutex coutMtx;
void print(const std::string& message){
    std::lock_guard<std::mutex> lock(coutMtx);
    std::cout << message;
}
 
int main()
{
    auto detachMe = [](){
        print("1. Going to sleep. ");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        print("2. I am awake now. ");
    };
     
    /* detachMe runs on a separate thread */  
    std::thread th(detachMe);
    th.detach();
 
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    print("3. Finished exsecution. ");
    return 0;
}
