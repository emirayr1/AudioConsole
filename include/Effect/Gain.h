#include "AudioEffect.h"
#include <string>

class Gain : public AudioEffect
{
public:

    enum class GainType
    {
        Linear,
        Decibel
    };

    Gain(float value, GainType gainType)
    :  m_value(value), m_gainType(gainType) {}

    void process(std::vector<float>& buffer) override;

    std::string getName() const override {return "Gain";}
    float m_value;
    GainType m_gainType;
};