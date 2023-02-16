#include <mutex>
#include <thread>
#include <iostream>
 
struct Account
{
    explicit Account(int amount) : balance{amount} {}
    int balance{0};
    std::mutex m;
};
 
void transfer_unique_lock(Account& from, Account& to, int amount)
{
    std::unique_lock<std::mutex> lock1{from.m, std::defer_lock};
    std::unique_lock             lock2{to.m, std::defer_lock};
    std::lock(lock1, lock2);
    from.balance-= amount;
    to.balance+= amount;
}

void transfer_scoped_lock(Account& from, Account& to, int amount)
{
    std::scoped_lock lock{from.m, to.m};
    from.balance-= amount;
    to.balance+= amount;
}

void transfer_lock_guard(Account& from, Account& to, int amount)
{
    std::lock_guard lock1{from.m};
    std::lock_guard lock2{to.m};
    from.balance-= amount;
    to.balance+= amount;
}

int main()
{
    Account acc1{1000}; Account acc2{1000};

	for (auto i = 0u; i < 500; ++i) {
       std::thread t1{transfer_unique_lock, std::ref(acc1), std::ref(acc2), 2};
       std::thread t2{transfer_unique_lock, std::ref(acc2), std::ref(acc1), 1};
       t1.join();
       t2.join();
    }
    std::cout << "unique_lock -- acc1: " << acc1.balance << " acc2: " << acc2.balance << '\n';

 	for (auto i = 0u; i < 500; ++i) {
       std::thread t1{transfer_scoped_lock, std::ref(acc1), std::ref(acc2), 1};
       std::thread t2{transfer_scoped_lock, std::ref(acc2), std::ref(acc1), 2};
       t1.join();
       t2.join();
    }
    std::cout << "scoped_lock -- acc1: " << acc1.balance << " acc2: " << acc2.balance << '\n';

 	for (auto i = 0u; i < 500; ++i) {
       std::thread t1{transfer_lock_guard, std::ref(acc1), std::ref(acc2), 1};
       std::thread t2{transfer_lock_guard, std::ref(acc2), std::ref(acc1), 2};
       t1.join();
       t2.join();
    }
    std::cout << "lock_guard  -- acc1: " << acc1.balance << " acc2: " << acc2.balance << '\n'; 
    return 0;
}
