#pragma once

#include <iostream>
#include <vector>

class WavFileManager
{
public:
    bool loadWav(std::string filePath);
    bool saveWav(std::string filePath);
    void cropWaveFile(std::pair<float, float> range, int type = 0);

    void printInfo();

    char chunkID[4];
    uint32_t chunkSize = 0;
    char format[4];
    char subChunk1ID[4];
    uint32_t subChunk1Size = 0;
    uint16_t audioFormat = 0;
    uint16_t numChannels = 0;
    uint32_t sampleRate = 0;
    uint32_t byteRate = 0;
    uint16_t blockAlign = 0;
    uint16_t bitsPerSample = 0;
    char subChunk2ID[4];
    uint32_t subChunk2Size = 0;
    
    uint64_t fileSize = 0;
    double durationInSeconds = 0;

    std::vector<float> audioBuffer;

    std::vector<float>& getAudioBuffer() { return audioBuffer; }
    size_t getAudioBufferSize() { return audioBuffer.size(); }
    int getNumChannels() {return (int)numChannels; }
    float getSampleRate() {return (float)sampleRate; }

private:
};