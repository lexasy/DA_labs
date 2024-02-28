#include "pair.hpp"
#include "vector.hpp"
#include <string.h>

void Counting_sort(NVector::TVector<TPairKV<TPhoneNumber, std::string>>& arr, size_t idx, int max_digit, std::string segment) 
{
    int tmp[max_digit + 1];
    memset(tmp, 0, sizeof(int) * (max_digit + 1));
    if (segment == "number")
    {
        for (size_t i = 0; i < arr.Size(); ++i)
        {
            if (segment == "number")
            {
                tmp[arr[i].key.number[idx] - '0']++;
            }
            else if (segment == "region")
            {
                tmp[arr[i].key.region_code[idx] - '0']++;
            }
            else
            {
                tmp[arr[i].key.country_code[idx] - '0']++;
            }
        }
        for (size_t i = 1; i < max_digit + 1; ++i)
        {
            tmp[i] += tmp[i - 1];
        }
        TPairKV<TPhoneNumber, std::string> result[arr.Size()];
        for (size_t i = arr.Size(); i > 0; --i)
        {
            if (segment == "number")
            {
                result[tmp[arr[i - 1].key.number[idx] - '0'] - 1] = arr[i - 1];
                tmp[arr[i - 1].key.number[idx] - '0']--;
            }
            else if (segment == "region")
            {
                result[tmp[arr[i - 1].key.region_code[idx] - '0'] - 1] = arr[i - 1];
                tmp[arr[i - 1].key.region_code[idx] - '0']--;
            }
            else
            {
                result[tmp[arr[i - 1].key.country_code[idx] - '0'] - 1] = arr[i - 1];
                tmp[arr[i - 1].key.country_code[idx] - '0']--;
            }
        }
        for (size_t i = 0; i < arr.Size(); ++i)
        {
            arr[i] = result[i];
        }
    }
}

void Radix_sort(NVector::TVector<TPairKV<TPhoneNumber, std::string>>& arr, size_t max_country_len, size_t max_region_len, size_t max_number_len)
{
    for (size_t i = max_number_len - 1; i > 0; --i)
    {
        int max_digit = 0;
        for (size_t j = 0; j < arr.Size(); ++j)
        {
            max_digit = (arr[j].key.number[i] - '0' > max_digit ? arr[j].key.number[i] - '0' : max_digit);
        }
        Counting_sort(arr, i, max_digit, "number");
    }
    for (size_t i = max_region_len - 1; i > 0; --i)
    {
        int max_digit = 0;
        for (size_t j = 0; j < arr.Size(); ++j)
        {
            max_digit = (arr[j].key.region_code[i] - '0' > max_digit ? arr[j].key.region_code[i] - '0' : max_digit);
        }
        Counting_sort(arr, i, max_digit, "region");
    }
    for (size_t i = max_country_len - 1; i > 0; --i)
    {
        int max_digit = 0;
        for (size_t j = 0; j < arr.Size(); ++j)
        {
            max_digit = (arr[j].key.country_code[i] - '0' > max_digit ? arr[j].key.country_code[i] - '0' : max_digit);
        }
        Counting_sort(arr, i, max_digit, "country");
    }
}

int main()
{
    TPairKV<TPhoneNumber, std::string> kv;
    NVector::TVector<TPairKV<TPhoneNumber, std::string>> arr;
    while (std::cin >> kv)
    {
        arr.Push_back(kv);
    }
    size_t max_country_len = 0;
    size_t max_region_len = 0;
    size_t max_number_len = 0;
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        max_country_len = (arr[i].key.country_code.size() > max_country_len ? arr[i].key.country_code.size() : max_country_len);
        max_region_len = (arr[i].key.region_code.size() > max_region_len ? arr[i].key.region_code.size() : max_region_len);
        max_number_len = (arr[i].key.number.size() > max_number_len ? arr[i].key.number.size() : max_number_len);
    }
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        arr[i].key.country_code = std::string(max_country_len - arr[i].key.country_code.size(), '0') + arr[i].key.country_code;
        // arr[i].key.country_null_counter = max_country_len - arr[i].key.country_code.size();
        arr[i].key.region_code = std::string(max_region_len - arr[i].key.region_code.size(), '0') + arr[i].key.region_code;
        // arr[i].key.region_null_counter = max_region_len - arr[i].key.region_code.size();
        arr[i].key.number = std::string(max_number_len - arr[i].key.number.size(), '0') + arr[i].key.number;
        // arr[i].key.number_null_counter = max_number_len - arr[i].key.number.size();
    }
    Radix_sort(arr, max_country_len, max_region_len, max_number_len);
    std::cout << std::endl;
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        std::cout << arr[i] << std::endl;
    }
    return 0;
}