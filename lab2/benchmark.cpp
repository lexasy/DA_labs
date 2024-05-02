#include <map>
#include <iostream>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

int main()
{
    std::map<std::string, uint64_t> rb;
    std::string input;
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    while (std::cin >> input)
    {
        if (input == "+")
        {
            uint64_t value;
            std::string key;
            std::cin >> key >> value;
            rb.insert({key, value});
            std::cout << "OK\n";
        }
        else
        {
            std::string key;
            rb.find(key);
            std::cout << "OK\n";
        }
    }
    
    auto end_ts = std::chrono::system_clock::now();
    uint64_t ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "time: " << ts << DURATION_PREFIX << "\n";
}