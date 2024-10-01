#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

int main() {
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    std::string text;
    getline(std::cin, text);
    std::string pattern;
    int patterns_counter = 0;
    while (getline(std::cin, pattern))
    {
        std::vector<int> result;
        patterns_counter++;
        auto it = std::search(text.begin(), text.end(), pattern.begin(), pattern.end());
        while (it != text.end())
        {
            result.push_back(int(std::distance(text.begin(), it)));
            it = std::search(it + 1, text.end(), pattern.begin(), pattern.end());
        }
        if (result.size() != 0)
        {
            std::cout << patterns_counter << ": ";
            std::sort(result.begin(), result.end());
            std::cout << result[0] + 1;
            for (size_t i = 1; i < result.size(); ++i)
            {
                std::cout << ", " << result[i] + 1;
            }
            std::cout << "\n";
        }
    }
    auto end_ts = std::chrono::system_clock::now();
    uint64_t sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "time: " << sort_ts << DURATION_PREFIX << "\n";
}
