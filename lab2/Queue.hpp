#pragma once

template <class T>
class TQueue
{
public:
    T *buffer;
    size_t size;
    size_t capacity;
    size_t head;
    TQueue()
    {
        buffer = new T[1];
        size = 0;
        capacity = 1;
        head = 0;
    }
    void Expand() 
    {
        T *new_buffer = new T[capacity * 2];
        capacity *= 2;
        for (std::size_t i = 0; i < size; ++i) 
        {
            new_buffer[i] = buffer[i];
        }
        delete[] buffer;
        buffer = new_buffer;
    }
    void PushBack(T& value)
    {
        if (size == capacity) 
        {
            Expand();
        }
        buffer[size] = value;
        ++size;
    }
    T PopFront()
    {
        T res = buffer[head];
        size--;
        head++;
        return res;
    }
    T& operator[](std::size_t idx) 
    {
        return buffer[idx];
    }
    void Clear()
    {
        delete[] buffer;
        size = 0;
        head = 0;
        capacity = 1;
        buffer = new T[capacity];
    }
    ~TQueue()
    {
        if (size > 0)
        {
            delete[] buffer;
        }
    }
};