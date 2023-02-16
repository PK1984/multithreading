#include <iostream>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <thread>

template<typename Key, typename Value>
class HashMap {
public:
    void put(const Key &key, const Value &value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        map_[key] = value;
    }

    bool get(const Key &key, Value &value) {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = map_.find(key);
        if (it != map_.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

private:
    std::unordered_map<Key, Value> map_;
    mutable std::shared_mutex mutex_;
};

int main() {
    HashMap<int, std::string> map;

    std::thread t1([&map]() { for (int i =   0; i < 100; i++) map.put(i, "Value_t1_" + std::to_string(i)); });
    std::thread t2([&map]() { for (int i = 100; i < 200; i++) map.put(i, "Value_t2_" + std::to_string(i)); });
    std::thread t3([&map]() {
        for (int i = 0; i < 200; i++) {
            std::string value;
            if (map.get(i, value)) std::cout << "Key: " << i << ", Value: " << value << std::endl;
        }
    });

    t1.join(); t2.join(); t3.join();
    return 0;
}
