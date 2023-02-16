/* who owns the mutex?
 * the code below is an example od undefined behaviour
 * do not use in production */

#include <iostream>
#include <mutex>

std::mutex mtx;

int main()
{
    std::unique_lock<std::mutex> lock1(mtx);
    std::cout << "lock1 is owned? " << lock1.owns_lock() << std::endl;

    std::unique_lock<std::mutex> lock2(mtx, std::try_to_lock); //no no no, undefined behaviour!
                                                               //even though pthreads may allow it, c++ standard does not
    std::cout << "lock2 is owned? " << lock2.owns_lock() << std::endl;
    
    if (lock1.owns_lock()) lock1.unlock(); //unlock regardles of who exaclty owns the lock
    if (lock2.owns_lock()) lock2.unlock(); //unlock regardles of who exaclty owns the lock

    std::unique_lock<std::mutex> lock3(mtx, std::adopt_lock); //no no no, undefined behaviour!
                                                              //assuming ownership by adopting unlocked mutex is a no-no
                                                              //it will call unlock on a mutex it does not own!
    std::cout << "lock2 is owned? " << lock3.owns_lock() << std::endl;

    std::cout << "finished \n";
    return 0;
}
