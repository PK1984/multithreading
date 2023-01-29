#include <thread>
#include <stdexcept>
#include <iostream>
 
int main()
{
    {
        std::thread th([](){});
        th.join();
        std::cout << "I have reached the end of the block no 1." << std::endl;
    } // at the end of the block the destructor of th object will be called.
      // It has been neither joined nor detached. The destructor will call std::terminate
 
 
 
    std::thread th([](){});
    {
		//throw std::runtime_error("An error occured");
        th.join();                                 
        std::cout << "I have reached the end of the block no 2." << std::endl;
    }  // after throwing an exception destructor of th object will be called,
       // the following join() will be skipped
    return 0;
}
