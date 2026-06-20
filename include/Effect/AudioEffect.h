#pragma once
#include <vector>

class AudioEffect
{
public:
    virtual ~AudioEffect() = default;
    virtual void process(std::vector<float>& buffer) = 0;
};