#pragma once

#include <iostream>
#include <vector>

class AudioEffect;
class Distortion;
class Gain;

class AudioProcessor
{
public:
    enum class EffectType{
        None,
        Distortion,
        Reverb,
        Gain
    };

    AudioProcessor();
    ~AudioProcessor();

    std::vector<std::unique_ptr<AudioEffect>> effectChain;


    void updateEffectChain(EffectType mainType, int subType = 0);
    void applyEffect(std::vector<float>& buffer);
};