#include <iostream>
#include <mutex>
#include <thread>

class BankAccount {
public:
    BankAccount(int initialBalance = 0) : balance(initialBalance) {}
    int getBalance() const {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        return balance;
    }
    void deposit(int amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        balance += amount;
    }
    bool withdraw(int amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        if (balance >= amount) {
            balance -= amount;
            return true;
        } else {
            return false;
        }
    }
 
    bool transfer(BankAccount& other, int amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        if (withdraw(amount)) {
            other.deposit(amount);
            return true;
        } else {
            return false;
        }
    }
 
private:
    int balance;
    mutable std::recursive_mutex mtx;
};

int main() {
    BankAccount a(100), b(50);
    if (a.transfer(b, 20)) {
        std::cout << "After transferring 20 from a to b: a=" << a.getBalance() << ", b=" << b.getBalance() << std::endl;
    } else {
        std::cout << "Failed to transfer 20 from a to b (not enough balance in a)" << std::endl;
    }

    return 0;
}
