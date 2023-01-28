#include <functional>
#include <iostream>
#include <thread>
 
void print(const std::string& message){
    std::cout << message << std::endl;
}
 
class SimpleFunctor {
    public:
    void operator()(){print("Ich bin ein functor");}
    void somePublicMethod(){print("Prosta metoda - class member");}
};
 
void funkcja(){print("Prosta funkcja");}
 
void function2(const int i) {print("Prosta funkcja z jednym argumentem");}
 
int main()
{  
    auto lambda = [](){ print("Jestem lambdą"); };
    std::thread thread1(lambda);
    thread1.join();
 
    SimpleFunctor functor;
    std::thread thread2(functor);
    thread2.join();
     
    std::thread thread3(&SimpleFunctor::somePublicMethod, std::ref(functor));
    thread3.join();
     
    std::thread thread4(funkcja);
    thread4.join();
 
    int argument{1};
    std::thread thread5(function2,argument);
    //std::thread thread5(std::bind(&function2, 1));
    thread5.join();
 
    return 0;
}
