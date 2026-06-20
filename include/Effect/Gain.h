#include "AudioEffect.h"

class Gain : public AudioEffect
{
public:
    Gain(float value)
    :  m_value(value) {}

    void process(std::vector<float>& buffer) override;

    float m_value;
};