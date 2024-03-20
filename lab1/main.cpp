#include <iostream>
#include <string>
#include <cstring>
#include <memory>

const size_t MAX_LENGTH_OF_NUMBER = 16;
const size_t MAX_LENGTH_OG_VALUE = 64;
const size_t DEFAULT = 1000000;

class TPhoneNumber 
{
public:
    uint8_t null_counter;
    char full_number[MAX_LENGTH_OF_NUMBER];
    size_t idx;
};

namespace NVector 
{
    template <class T>
    class TVector 
    {
    private:
        std::size_t size;
        std::size_t capacity;

        void Expand() 
        {
            T *new_buffer = new T[capacity * 2];
            capacity *= 2;
            for (std::size_t i = 0; i < size; ++i) 
            {
                new_buffer[i] = std::move(buffer[i]);
            }
            delete[] buffer;
            buffer = new_buffer;
        }

    public:
        T *buffer;
        TVector() 
        {
            buffer = new T[DEFAULT];
            size = 0;
            capacity = DEFAULT;
        }

        void Push_back(const T& value) 
        {
            if (size == capacity) 
            {
                Expand();
            }
            buffer[size] = value;
            ++size;
        }

        size_t Size()
        {
            return size;
        }

        T& operator[](std::size_t idx) 
        {
            return buffer[idx];
        }

        ~TVector()
        {
            delete[] buffer;
        }
    };
}

void CountingSort(NVector::TVector<TPhoneNumber>& arr, uint8_t& idx, TPhoneNumber *result) 
{
    // Temporary array for digits
    int tmp[10] = {0};
    // Counting of quantity digits in numbers
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        tmp[(idx <= arr[i].null_counter ? '0' : arr[i].full_number[idx - arr[i].null_counter]) - '0']++;
    }
    // Counting of prefix sums
    for (size_t i = 1; i < 10; ++i)
    {
        tmp[i] += tmp[i - 1];
    }
    // Destribution elements in result array
    for (size_t i = arr.Size(); i > 0; --i)
    {
        --tmp[(idx <= arr[i - 1].null_counter ? '0' : arr[i - 1].full_number[idx - arr[i - 1].null_counter]) - '0'];
        result[tmp[(idx <= arr[i - 1].null_counter ? '0' : arr[i - 1].full_number[idx - arr[i - 1].null_counter]) - '0']] = arr[i - 1];
    }
    // Copying of sorted elements in result array
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        arr[i] = std::move(result[i]);
    }
}

void RadixSort(NVector::TVector<TPhoneNumber>& arr, uint8_t& max_len)
{
    // Allocating counting sort result array
    TPhoneNumber *result = new TPhoneNumber[arr.Size()];
    for (uint8_t i = max_len - 1; i > 0; --i)
    {
        // '-' symbols in 4th and 8th idx's
        if ((i == 8) || (i == 4))
        {
            continue;
        }
        CountingSort(arr, i, result);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    TPhoneNumber number;
    char value[MAX_LENGTH_OG_VALUE];

    NVector::TVector<TPhoneNumber> arr;
    NVector::TVector<std::shared_ptr<std::string>> valarr;
    uint8_t max_len = 0;
    // Reading elements
    while (scanf("%s", number.full_number) != EOF)
    {
        scanf("%s", value);
        // Counting of max length of number
        max_len = (strlen(number.full_number) > max_len ? strlen(number.full_number) : max_len);
        // Null digits counter forward number
        number.null_counter = MAX_LENGTH_OF_NUMBER - strlen(number.full_number);
        // Idx for value array
        number.idx = arr.Size();
        // Filling of value array
        valarr.Push_back(std::make_shared<std::string>(std::string(value)));
        // Filling of key array
        arr.Push_back(number);
    }
    // Sorting elements
    RadixSort(arr, max_len);
    // Printing elements
    for (size_t i = 0; i < arr.Size(); ++i)
    {
        printf("%s\t%s\n", arr[i].full_number, valarr[arr[i].idx]->c_str());
    }
    return 0;
}