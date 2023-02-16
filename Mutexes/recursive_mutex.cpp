#include <iostream>
#include <mutex>

class BankAccount {
public:
    BankAccount(int initialBalance = 0) : balance_(initialBalance) {}
    int getBalance() const   {
		std::lock_guard<std::recursive_mutex> lock(mtx_);
		return balance_;
	}
    void deposit(int amount) {
		std::lock_guard<std::recursive_mutex> lock(mtx_);
		balance_ += amount;
	}
    bool withdraw(int amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        if (balance_ >= amount) {
            balance_ -= amount;
            return true;
        } else {
            return false;
        }
    }
    bool transfer(BankAccount& other, int amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx_);
        if (balance_ >= amount) {
            balance_ -= amount;
            other.deposit(amount);
            return true;
        } else {
            return false;
        }
    }
private:
    int balance_;
    mutable std::recursive_mutex mtx_;
};

int main() {
    BankAccount a(100), b(50);

    std::cout << "Initial balances: a=" << a.getBalance() << ", b=" << b.getBalance() << std::endl;

    // Deposit 50 into account a
    a.deposit(50);
    std::cout << "After depositing 50 into a: a=" << a.getBalance() << ", b=" << b.getBalance() << std::endl;

    // Withdraw 30 from account a
    if (a.withdraw(30)) {
        std::cout << "After withdrawing 30 from a: a=" << a.getBalance() << ", b=" << b.getBalance() << std::endl;
    } else {
        std::cout << "Failed to withdraw 30 from a (not enough balance)" << std::endl;
    }

    // Transfer 20 from account a to account b
    if (a.transfer(b, 20)) {
        std::cout << "After transferring 20 from a to b: a=" << a.getBalance() << ", b=" << b.getBalance() << std::endl;
    } else {
        std::cout << "Failed to transfer 20 from a to b (not enough balance in a)" << std::endl;
    }

    return 0;
}
