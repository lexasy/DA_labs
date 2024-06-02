#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iterator>

std::vector<size_t> ZFunction(std::vector<uint32_t>& string)
{
    std::vector<size_t> zfunc(string.size());
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 1; i < string.size(); ++i)
    {
        if (i <= right)
        {
            zfunc[i] = std::min(zfunc[i - left], right - i);
        }
        while ((i + zfunc[i] < string.size()) && (string[zfunc[i]] == string[i + zfunc[i]]))
        {
            zfunc[i]++;
        }
        if (i + zfunc[i] > right)
        {
            left = i;
            right = i + zfunc[i];
        }
    }
    return zfunc;
}

std::vector<size_t> StrongSPFunction(std::vector<uint32_t>& string)
{
    std::vector<size_t> spfunc(string.size());
    std::vector<size_t> zfunc = ZFunction(string);
    for (size_t i = string.size() - 1; i > 0; --i)
    {
        spfunc[i + zfunc[i] - 1] = zfunc[i];
    }
    return spfunc;
}

std::vector<size_t> KMPSearch(std::vector<uint32_t>& pattern, std::vector<std::pair<uint32_t, std::pair<size_t, size_t>>>& text)
{
    std::vector<size_t> spfunc = StrongSPFunction(pattern);
    std::vector<size_t> result;
    if (pattern.size() <= text.size())
    {
        size_t text_iterator = 0;
        while (text_iterator < text.size() - pattern.size() + 1)
        {
            size_t pattern_iterator = 0;
            while ((pattern_iterator < pattern.size()) && (pattern[pattern_iterator] == text[text_iterator + pattern_iterator].first))
            {
                pattern_iterator++;
            }
            if (pattern_iterator == pattern.size())
            {
                result.push_back(text_iterator);
            }
            else
            {
                if ((pattern_iterator >= 1) && (pattern_iterator > spfunc[pattern_iterator - 1]))
                {
                    text_iterator += (pattern_iterator - spfunc[pattern_iterator - 1] - 1);
                }
            }
            text_iterator++;
        }
    }
    return result;
}

int main()
{
    std::string input_pattern;
    std::getline(std::cin, input_pattern);
    std::istringstream pattern_stream(input_pattern);
    std::vector<uint32_t> pattern;
    std::copy(std::istream_iterator<uint32_t>(pattern_stream), std::istream_iterator<uint32_t>(), std::back_inserter(pattern));
    std::string input_text;
    std::vector<std::pair<uint32_t, std::pair<size_t, size_t>>> text;
    size_t lc = 1;
    std::vector<size_t> result;
    while (std::getline(std::cin, input_text))
    {
        std::istringstream text_stream(input_text);
        uint32_t x;
        size_t wc = 1;
        while (text_stream >> x)
        {
            text.push_back({x, {wc, lc}});
            wc++;
        }
        lc++;
        if (text.size() > 2 * pattern.size())
        {
            result = KMPSearch(pattern, text);
            for (size_t i = 0; i < result.size(); ++i)
            {
                std::cout << text[result[i]].second.second << ", " << text[result[i]].second.first << "\n";
            }
            std::vector<std::pair<uint32_t, std::pair<size_t, size_t>>> tmp;
            for (size_t i = text.size() - pattern.size() + 1; i < text.size(); ++i)
            {
                tmp.push_back(text[i]);
            }
            text = tmp;
        }
    }
    result = KMPSearch(pattern, text);
    for (size_t i = 0; i < result.size(); ++i)
    {
        std::cout << text[result[i]].second.second << ", " << text[result[i]].second.first << "\n";
    }
}