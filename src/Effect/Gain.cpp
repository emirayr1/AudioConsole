#include "../../include/Effect/Gain.h"
#include <algorithm>
#include <iostream>

void Gain::process(std::vector<float>& buffer)
{
    if(m_value < 1 && m_value > 0)
    {
        std::transform(buffer.begin(), buffer.end(), buffer.begin(), 
                            [this](float x){return x * m_value; });
    }else{
        std::cout << "Value must between 0-1" << std::endl;
    }
}