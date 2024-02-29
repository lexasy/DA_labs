#pragma once

#include <iostream>
#include <stdexcept>

const int DEFAULT = 1;

namespace NVector 
{
    template <class T>
    class TVector 
    {
    private:
        T *buffer;
        std::size_t size;
        std::size_t capacity;

        void Expand() 
        {
            T *new_buffer = new T[capacity * 2];
            for (std::size_t i = 0; i < size; ++i) 
            {
                new_buffer[i] = buffer[i];
            }
            delete[] buffer;
            buffer = new_buffer;
            capacity *= 2;
        }

    public:
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
            buffer[size++] = value;
        }

        std::size_t Size()
        {
            return size;
        }

        T& operator[](std::size_t idx) 
        {
            if (idx >= size) 
            {
                throw std::logic_error("Out of bound exception");
            }
            return buffer[idx];
        }

        ~TVector()
        {
            delete[] buffer;
        }
    };
}
