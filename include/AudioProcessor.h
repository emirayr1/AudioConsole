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
    
    template <typename EffectClass>
    EffectClass* updateEffectChain();

    void applyEffect(std::vector<float>& buffer);

    
    std::vector<std::unique_ptr<AudioEffect>> effectChain;
};

template <typename EffectClass>
EffectClass* AudioProcessor::updateEffectChain()
{
   auto effect = std::make_unique<EffectClass>();

   for (const auto& eff : effectChain) {
        if (eff->getName() == effect->getName()) {
            std::cout << "⚠️ Zincirde zaten bu efekt var!" << std::endl;
            return nullptr; // Koruma: Varsa nullptr dönüyoruz
        }
    }
    
   EffectClass* rawPtr = effect.get();

   effectChain.push_back(std::move(effect));
   return rawPtr;
}