#include "AudioEffect.h"

class Fade : public AudioEffect
{
public:
    enum class FadeType{
        Linear,
        Exponential,
        SCurve,
        None
    };

    Fade() = default;
    ~Fade() = default;

    void process(std::vector<float>& buffer) override;
    std::string getName() const override { return "Fade"; }

    void setType(FadeType type) { m_type = type; }
    void setFadeLenghtInSeconds(float length) { fadeLengthInSeconds = length; }
    void setIsFadeIn(bool condition) { isFadeIn = condition; }
    void setIsFadeOut(bool condition) { isFadeOut = condition; }

private:
    FadeType m_type = FadeType::None;
    bool isFadeIn = false;
    bool isFadeOut = false;
    float fadeLengthInSeconds = 0.0f;
};