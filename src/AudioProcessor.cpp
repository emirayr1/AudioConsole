#include "AudioProcessor.h"
#include "../include/Effect/AudioEffect.h"
#include "../include/Effect/Distortion.h"
#include "../include/Effect/Gain.h"
#include <memory>

AudioProcessor::AudioProcessor() = default;
AudioProcessor::~AudioProcessor() = default;


void AudioProcessor::applyEffect(std::vector<float>& buffer)
{
    for(auto& eff : effectChain)
    {
        std::cout << eff->getName() << " is processing right now" << std::endl;
        eff->process(buffer);
    }
}