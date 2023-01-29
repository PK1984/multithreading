#include <functional>
#include <iostream>
#include <thread>

void print(const std::string& message){
    std::cout << message << std::endl;
}

class SimpleFunctor {
    public:
    void operator()(){print("2. Funktor");}
    void somePublicMethod(){print("3. Metoda klasy");}
};

void funkcja(){print("4. Wolna funkcja");}

void function2(const int i,const std::vector<int>& vect) {print("5. Wolna funkcja z argumentem");}

std::thread t;

int main()
{
    auto lambda = [](){ print("1. Lambda"); };
    std::thread thread1(lambda);
    std::cout << std::boolalpha << thread1.joinable() <<  std::endl;
    std::cout << thread1.get_id() <<  std::endl;
    thread1.join();
    std::cout << std::boolalpha << thread1.joinable() <<  std::endl;
    std::cout << thread1.get_id() <<  std::endl;
    //thread1.join();
	if (thread1.joinable()) thread1.join();


    SimpleFunctor functor;
    std::thread thread2(functor);
    thread2.join();

    std::thread thread3(&SimpleFunctor::somePublicMethod, std::ref(functor));
    thread3.join();

    std::thread thread4(funkcja);
    thread4.join();

    int argument{1};
    std::vector<int> vect{1,2,3};
    std::thread thread5(function2,argument, std::ref(vect));
    //std::thread thread5(std::bind(&function2, 1));
    thread5.join();

    return 0;
}
