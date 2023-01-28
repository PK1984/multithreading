#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
 
/* access to cout in print() is protected by a mutex */
std::mutex coutMtx;
void print(const std::string& message){
    std::lock_guard<std::mutex> lock(coutMtx);
    std::cout << message << std::endl;
}
 
int main()
{
    auto detachMe = [](){
        print("Going to sleep");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        print("I am awake now");
    };
     
    /* detachMe runs on a separate thread */  
    std::thread th(detachMe);
    th.detach();
 
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Finished execution";
    return 0;
}
