#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <random>

class TimeMe {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::string label_;
    static std::mutex mtx_;
public:
    TimeMe(const std::string& label) : label_(label), start_(std::chrono::high_resolution_clock::now()) {}
    ~TimeMe(){
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_);
        std::unique_lock<std::mutex> lock(TimeMe::mtx_);
        std::cout << label_ << " = " << duration.count() << " us" << std::endl;
    }  
};
std::mutex TimeMe::mtx_;

std::vector<int> getRandomData(const int size){
    std::vector<int> output(size);
    std::generate(output.begin(), output.end(), std::rand);
    return output;
}

void sorter(std::vector<int>& vect, const int start, const int stop, const int id)
{
    auto timer = TimeMe("Time spent in worker " + std::to_string(id));
    std::sort(vect.begin() + start, vect.begin() + stop);
}

std::vector<int> sequentialSorter(std::vector<int> vect){
    auto timer = TimeMe("Time spent on sequential sorting");
    std::sort(vect.begin(), vect.end());
    return vect;
}



int main(){
    constexpr int size = 1000;
    constexpr int threadNum = 4;
    auto randomDataVec = getRandomData(size);
    sequentialSorter(randomDataVec);
    auto timer = TimeMe("Total time spent in multithreaded sorter");
    std::array<std::thread,threadNum> threads;
    for (int i = 0; i < threadNum; ++i)
        threads[i] = std::thread(sorter, std::ref(randomDataVec), i * size / threadNum, ( i + 1 ) *  size / threadNum, i);

    for (int i = 0; i < threadNum; ++i) threads[i].join();

    auto merge = [&randomDataVec](int left, int middle, int end){
        std::inplace_merge(randomDataVec.begin() + left, randomDataVec.begin() + middle, randomDataVec.begin() + end);};
    auto th1 = std::thread(merge, 0, size / threadNum, 2 * size / threadNum); 
    auto th2 = std::thread(merge, 2 * size / threadNum, 3 * size / threadNum, 4 * size / threadNum);
    th1.join();
    th2.join();
    merge(0 * size / threadNum, 2 * size / threadNum, 4 * size / threadNum); 
    //if (std::is_sorted(randomDataVec.begin(), randomDataVec.end())) std::cout << "sorted!" << std::endl;

    {
        auto timer = TimeMe("creating and joining and destroying std::thread object");
        std::thread th;
        {
            auto timer = TimeMe("initialize std::thread object");
            th = std::thread([](){});
        }
        {
            auto timer = TimeMe("join std::thread object");
            th.join();
        }
    }
    return 0;
}