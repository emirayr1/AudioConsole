#pragma once
#include <vector>
#include <string>

class AudioEffect
{
public:
    virtual ~AudioEffect() = default;
    virtual void process(std::vector<float>& buffer) = 0;

    virtual std::string getName() const = 0;
    virtual void setParam() = 0;
};