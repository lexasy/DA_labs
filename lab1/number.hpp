#pragma once

#include <iostream>
#include <string>

class TPhoneNumber
{
public:
    std::string country_code;
    std::string region_code;
    std::string number;
    std::string full_number;
};

inline std::ostream& operator<<(std::ostream& os, const TPhoneNumber& number)
{
    os << number.full_number;
    return os;
}

inline std::istream& operator>>(std::istream& is, TPhoneNumber& number)
{
    number.country_code = "";
    number.region_code = "";
    number.number = "";
    is >> number.full_number;
    unsigned counter = 0;
    for (std::size_t i = 0; i < number.full_number.length(); ++i)
    {
        switch (number.full_number[i])
        {   
            case '+':
                counter++;
                continue;
            case '-':
                counter++;
                continue;
            default:
                switch (counter)
                {
                    case 1:
                        number.country_code += number.full_number[i];
                        continue;
                    case 2:
                        number.region_code += number.full_number[i];
                        continue;
                    default:
                        number.number += number.full_number[i];
                }
        }
    }
    return is;
}
