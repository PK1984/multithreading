#include <iostream>
#include <future>
#include <thread>

int main()
{
    std::promise<int> p;
    auto f = p.get_future();
    
    auto sum = [&f](int a, int b) { 
        int c = f.get();
        std::cout << "result: " << a + b + c << std::endl;
    };
    
    auto t1 = std::thread(sum, 1, 2);
    auto t2 = std::thread(sum, 3, 4);
    
    p.set_value(5);
    
    t1.join(); t2.join();
    
    return 0;
}
