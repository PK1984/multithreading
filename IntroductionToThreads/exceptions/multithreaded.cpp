#include <iostream>
#include <thread>
 
void jobThatOcasionallyThrows() {throw std::invalid_argument( "Argument jest inwalidą" );}
void someLargeJob(){int x=0; for ( int i = 0; i < 1000000; ++i) {x+=i;}}
 
void performJobsConcurrently(){
    std::thread th1(someLargeJob);
    std::thread th2(jobThatOcasionallyThrows);
    th1.join(); th2.join();
}
 
int main()
{
    try{
        performJobsConcurrently();
    } catch (std::invalid_argument& arg) {
        std::cout << "I have handled exception" << std::endl;
    }
    std::cout << "I have reached the end." << std::endl;
    return 0;
}
