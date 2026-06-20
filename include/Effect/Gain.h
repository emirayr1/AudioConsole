#include "AudioEffect.h"
#include <string>

class Gain : public AudioEffect
{
public:
    Gain(float value)
    :  m_value(value) {}

    void process(std::vector<float>& buffer) override;

    std::string getName() const override {return "Gain";}
    float m_value;
};