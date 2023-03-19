// exhaust_entropy.cpp
#include <openssl/rand.h>
#include <iostream>

int main() {
    if (RAND_status() == 0){
		std::cout << "random number generator has not been suffieciently seeded" << std::endl;
		return 1;
	}

    unsigned char random_data[1024*1024];
    // Generate random numbers until the entropy pool is exhausted
    while (true) {
		auto errCode = RAND_bytes(random_data, sizeof(random_data));
        if (errCode != 1) {
           std::cout << "The source of entropy has been exhausted" << std::endl;
           return 1;
        }
	}
 
    return 0;
}
