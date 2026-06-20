#include "AudioEffect.h"

class Distortion : public AudioEffect
{
public:
    enum class DistortionType{
        HardClip,
        SoftClip,
        Fuzz
    };

    Distortion(DistortionType type, float drive, float threshold)
    : m_type(type), m_drive(drive), m_threshold(threshold) {}

    void process(std::vector<float>& buffer) override;

    DistortionType m_type;
    float m_drive;
    float m_threshold;
};