#include "../../include/Effect/Gain.h"
#include <iostream>
#include <cmath>

void Gain::process(std::vector<float>& buffer)
{
    if(m_gainType == Gain::GainType::Linear)
    {
        for(size_t i = 0; i < buffer.size(); i++)
        {
            buffer[i] *= m_value;
        }
    }
    else if(m_gainType == Gain::GainType::Decibel)
    {
        float linearValue = powf(10.0f, (m_value / 20.0f));

        std::cout << "Decibel: " << m_value <<" Linear Value is: " << linearValue << std::endl;

        for(size_t i = 0; i < buffer.size(); i++)
        {
            buffer[i] *= linearValue;
        }
    }
}