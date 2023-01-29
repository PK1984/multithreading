#include <iostream>
#include <thread>

int main()
{
    int lambdaCaptureVal{0}; //reference in lambda capture, writes will visible after join()
    int byReferenceVal{0}; // argument passed by reference, writes will be visible after join()
    auto function = [&lambdaCaptureVal](const int i, const int j, int& output)
					{
						lambdaCaptureVal= j*j; //value captured by reference will be accessible - synchronization needed
						output = i*i; //output passed by reference will be accessible - synchronization needed
						return i*i; //returned value discarded
					};

	/* invoke the thread */
    std::thread th(function, 2, 3, std::ref(byReferenceVal));

    /* the thread may not have finished yet - data race! */
    std::cout << "i*i = " << byReferenceVal << "; j*j = " << lambdaCaptureVal << "; ";

    /* synchronize with the thread */
    th.join();

    /* the main is already synchonized, the expected output is i * i = 4; j * j = 9 */
    std::cout << "i*i = " << byReferenceVal<< "; j*j = " << lambdaCaptureVal << std::endl;
    return 0;
}
