#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

class TPair
{
public:
    std::string number;
    std::string value;
    bool operator<(const TPair& other) const;
};

bool TPair::operator<(const TPair& other) const {
    if (number.size() == other.number.size())
    {
        return number < other.number;
    } else if (number.size() < other.number.size())
    {
        return true;
    } else {
        return false;
    }
}

int main()
{
    std::vector<TPair> arr;
    TPair pair;
    while (std::cin >> pair.number >> pair.value)
    {
        arr.push_back(pair);
    }
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    std::stable_sort(std::begin(arr), std::end(arr));
    auto end_ts = std::chrono::system_clock::now();
    uint64_t sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    for (size_t i = 0; i < arr.size(); ++i)
    {
        std::cout << arr[i].number << "\t" << arr[i].value << "\n";
    }
    std::cout << "time: " << sort_ts << DURATION_PREFIX << "\n";
}