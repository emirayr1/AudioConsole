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

    
    const std::vector<std::unique_ptr<AudioEffect>>& getEffectChain() const { return effectChain; }
    
    void updateEffectChain(EffectType mainType, int subType = 0);
    void applyEffect(std::vector<float>& buffer);

    
    std::vector<std::unique_ptr<AudioEffect>> effectChain;
};