#pragma once

#include <iostream>
#include "number.hpp"

template <class K, class V>
class TPairKV 
{
public:
    K key;
    V value;
};

inline std::ostream& operator<<(std::ostream& os, const TPairKV<TPhoneNumber, std::string>& kv) 
{
    os << kv.key << "\t" << kv.value;
    return os;
}

inline std::istream& operator>>(std::istream& is, TPairKV<TPhoneNumber, std::string>& kv) 
{
    is >> kv.key >> kv.value;
    return is;
}