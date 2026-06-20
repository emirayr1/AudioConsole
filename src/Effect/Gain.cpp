#include "../../include/Effect/Gain.h"
#include <algorithm>
#include <iostream>

void Gain::process(std::vector<float>& buffer)
{
    if(m_value < 1 && m_value > 0)
    {
        for(size_t i = 0; i < buffer.size(); i++)
        {
            buffer[i] *= 0.1f;
        }
    }else{
        std::cout << "Value must between 0-1" << std::endl;
    }
}