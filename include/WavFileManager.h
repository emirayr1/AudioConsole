#pragma once

#include <iostream>

class WavFileManager
{
public:
    bool loadWav(std::string filePath);
    bool saveWav(std::string filePath);


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
private:
};