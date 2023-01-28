#include <iostream>
#include <thread>
 
void jobThatOcasionallyThrows() {throw std::invalid_argument( "Argument jest inwalidą" );}
void someLargeJob(){int x=0; for ( int i = 0; i < 1000000; ++i) {x+=i;}}
 
/* both someLargeJob() and jobThatOcasionallyThrows() look like prime target for parallelization */
void performJobsSequentially(){ someLargeJob(); jobThatOcasionallyThrows();}
 
int main()
{
    try{
        performJobsSequentially();
    } catch (std::invalid_argument& arg) {
        std::cout << "I have handled the exception (by ignoring it)." << std::endl;
    }
    std::cout << "I have reached the end." << std::endl;
    return 0;
}
