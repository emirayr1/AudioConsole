#include "AudioProcessor.h"
#include "../include/Effect/AudioEffect.h"
#include "../include/Effect/Distortion.h"
#include "../include/Effect/Gain.h"
#include <memory>

AudioProcessor::AudioProcessor() = default;
AudioProcessor::~AudioProcessor() = default;

void AudioProcessor::updateEffectChain(EffectType effectType, int subType)
{
    std::unique_ptr<AudioEffect> effect = nullptr;
    auto distType = Distortion::DistortionType::SoftClip;
    auto gainType = Gain::GainType::Linear;
    switch (effectType)
    {
    case EffectType::Distortion:
        distType = static_cast<Distortion::DistortionType>(subType);

        effect = std::make_unique<Distortion>(distType, 5.0f, 0.7f);
        break;
    case EffectType::Reverb:
        break;
    case EffectType::Gain:
        gainType = static_cast<Gain::GainType>(subType); 
        effect = std::make_unique<Gain>(0.1f, gainType);
    default:
        break;
    }

    if (!effect) std::cerr <<"No Effect Found" << std::endl;

    effectChain.push_back(std::move(effect));
}

void AudioProcessor::applyEffect(std::vector<float>& buffer)
{
    for(auto& eff : effectChain)
    {
        std::cout << eff->getName() << " is processing right now" << std::endl;
        eff->process(buffer);
    }
}
