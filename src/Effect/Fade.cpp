#include "../../include/Effect/Fade.h"

void Fade::process(std::vector<float>& audioBuffer)
{
    if(isFadeIn){

    }
}

std::vector<float> Fade::generateFadeArray()
{
    std::vector<float> fadeArray(lengthInSamples);
    
    if (fadeArray.size() <= 1) {
        if (!fadeArray.empty()) fadeArray[0] = 1.0f;
        return fadeArray;
    }

    float maxIndex = static_cast<float>(fadeArray.size() - 1);

    for(size_t i = 0; i < fadeArray.size(); i++)
    {
        fadeArray[i] = static_cast<float>(i) / maxIndex;
    }

    return fadeArray;
}