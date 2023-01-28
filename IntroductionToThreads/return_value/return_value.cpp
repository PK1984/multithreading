#include <iostream>
#include <thread>
 
int main()
{
    int lambdaCaptureVal{0}; //reference in lambda capture, writes will visible after join()
    int byReferenceVal{0}; // argument passed by reference, writes will be visible after join()
    auto function = [&lambdaCaptureVal](const int i, const int j, int& output){ lambdaCaptureVal= j*j; output = i*i; return i*i; };
    std::thread th(function, 2, 3, std::ref(byReferenceVal));
    std::cout << "i*i = " << byReferenceVal<< "; j*j = " << lambdaCaptureVal << std::endl;
    th.join();
    std::cout << "i*i = " << byReferenceVal<< "; j*j = " << lambdaCaptureVal << std::endl;
    return 0;
}
