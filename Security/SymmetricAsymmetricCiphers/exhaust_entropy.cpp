#include <openssl/rand.h>
#include <iostream>
#include <chrono>

int main() {
    if (RAND_status() == 0){
		std::cout << "random number generator as not been suffieciently seeded" << std::endl;
		return 1;
	}

    unsigned char random_data[1024*1024];
    // Generate random numbers until the entropy pool is exhausted
    auto failed{false};
    while (!failed) {
		auto start = std::chrono::high_resolution_clock::now();
		for (auto i = 0u; i < 1000; i++){
			failed = RAND_bytes(random_data, sizeof(random_data)) != 1;
		}
		auto stop = std::chrono::high_resolution_clock::now();
		std::cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
	 }
 
    return 0;
}
