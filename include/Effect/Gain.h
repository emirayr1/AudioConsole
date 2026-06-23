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

    Gain() = default;
    ~Gain() = default;

    void process(std::vector<float>& buffer) override;

    std::string getName() const override {return "Gain";}

    void setValue(float _value) {m_value = _value;}
    void setType(GainType _type) {m_gainType = _type; }


    float m_value;
    GainType m_gainType;
};