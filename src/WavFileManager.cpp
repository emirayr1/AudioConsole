#include "WavFileManager.h"
#include <fstream>
#include <chrono>

bool WavFileManager::loadWav(std::string filePath)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file(filePath, std::ios::binary);
    
    if(file.is_open()){
        file.seekg(0);
        file.read(chunkID, 4);
        file.read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));
        file.read(format, 4);
        file.read(subChunk1ID, 4);
        file.read(reinterpret_cast<char*>(&subChunk1Size), sizeof(subChunk1Size));
        file.read(reinterpret_cast<char*>(&audioFormat), 2);
        file.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
        file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
        file.read(reinterpret_cast<char*>(&byteRate), sizeof(byteRate));
        file.read(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
        file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));

        char searchBuffer[4] = {0};
        while(file.read(searchBuffer, 4))
        {   
            if(searchBuffer[0] == 'd' && searchBuffer[1] == 'a' &&
                searchBuffer[2] == 't' && searchBuffer[3] == 'a')
                {
                    subChunk2ID[0] = 'd'; subChunk2ID[1] = 'a'; 
                    subChunk2ID[2] = 't'; subChunk2ID[3] = 'a';
                    break; // Döngüden çık
                }

                // 4 ileri geldiğimiz için 3 geri gittik ve 1 byte ilerledik
                file.seekg(-3, std::ios::cur);
        }

        if(file.eof() || !file) { return false; }

        file.read(reinterpret_cast<char*>(&subChunk2Size), sizeof(subChunk2Size));
        
        // RESET BUFFER
        audioBuffer.clear();

        if(bitsPerSample == 8)
        {
            size_t sampleCount = subChunk2Size / sizeof(int8_t);

            std::vector<uint8_t> tempBuffer(sampleCount);

            file.read(reinterpret_cast<char*>(&tempBuffer[0]), subChunk2Size);

            audioBuffer.resize(sampleCount);

            for(size_t i = 0; i < sampleCount; ++i)
            {
                audioBuffer[i] = (static_cast<float>(tempBuffer[i]) - 128.0f) / 128.0f;
            }
        }
        else if(bitsPerSample == 16)
        {
            size_t sampleCount = subChunk2Size / sizeof(int16_t);

            std::vector<int16_t> tempBuffer(sampleCount);

            file.read(reinterpret_cast<char*>(&tempBuffer[0]), subChunk2Size);

            audioBuffer.resize(sampleCount);

            for(size_t i = 0; i < sampleCount; ++i)
            {
                audioBuffer[i] = static_cast<float>(tempBuffer[i]) / 32768.0f;
            }
        }else if(bitsPerSample == 24){
            
            size_t sampleCount = subChunk2Size / 3;
            std::vector<uint8_t> rawBytes(subChunk2Size);
            
            file.read(reinterpret_cast<char*>(&rawBytes[0]), subChunk2Size);

            audioBuffer.resize(sampleCount);

            size_t bufferIndex = 0;

            for(size_t i = 0; i < subChunk2Size; i += 3)
            {
                uint8_t b0 = rawBytes[i];
                uint8_t b1 = rawBytes[i + 1];
                uint8_t b2 = rawBytes[i + 2];
    
                int32_t sample32 = (b0) | (b1 << 8) | (b2 << 16);
    
                if(sample32 & 0x00800000)
                {
                    sample32 = sample32 | 0xFF000000;
                }

                audioBuffer[bufferIndex] = static_cast<float>(sample32) / 8388608.0f;
                bufferIndex++;
            }
        }else if(bitsPerSample == 32){
            size_t sampleCount = subChunk2Size / sizeof(float);
            audioBuffer.resize(sampleCount);

            file.read(reinterpret_cast<char*>(&audioBuffer[0]), subChunk2Size);
        }
        
        file.seekg(0, std::ios::end);
        fileSize = static_cast<uint64_t>(file.tellg());
        
        if(numChannels > 0 && bitsPerSample > 0 && sampleRate > 0)
        {
            durationInSeconds = static_cast<double>(subChunk2Size) / byteRate;
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Loading Time: " << duration.count() << " ms" << std::endl;

        return true;
    }else{
        return false;
    }
}

bool WavFileManager::saveWav(std::string filePath)
{
    std::ofstream file(filePath, std::ios::binary);
    
    if(!(audioBuffer.size() > 0)) { return false; }

    if(file.is_open())
    {
        file.write(chunkID, 4);
        file.write(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));
        file.write(format, 4);
        file.write(subChunk1ID, 4);
        file.write(reinterpret_cast<char*>(&subChunk1Size), sizeof(subChunk1Size));
        file.write(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
        file.write(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
        file.write(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
        file.write(reinterpret_cast<char*>(&byteRate), sizeof(byteRate));
        file.write(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
        file.write(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));
        file.write(subChunk2ID, 4);
        file.write(reinterpret_cast<char*>(&subChunk2Size), sizeof(subChunk2Size));

        if(bitsPerSample == 8)
        {
            for(size_t i = 0; i < audioBuffer.size(); i++)
            {
                float fSample = audioBuffer[i];

                if(fSample > 1.0f) fSample = 1.0f;
                if(fSample < -1.0f) fSample = -1.0f;

                int32_t targetSample = static_cast<int32_t>(std::round(fSample * 127.0f)) + 128;
                
                if(targetSample > 255) targetSample = 255;
                if(targetSample < 0)   targetSample = 0;

                uint8_t sample8 = static_cast<uint8_t>(targetSample);
                file.write(reinterpret_cast<char*>(&sample8), sizeof(uint8_t));
            }
        }
        else if(bitsPerSample == 16)
        {
            for(size_t i = 0; i < audioBuffer.size(); i++)
            {
                float fSample = audioBuffer[i];

                if(fSample > 1.0f) fSample = 1.0f;
                if(fSample < -1.0f) fSample = -1.0f;

                int32_t calculatedSample = static_cast<int32_t>(std::round(fSample * 32767.0f));

                if(calculatedSample > 32767)  calculatedSample = 32767;
                if(calculatedSample < -32768) calculatedSample = -32768;

                int16_t sample16 = static_cast<int16_t>(calculatedSample);
                file.write(reinterpret_cast<char*>(&sample16), sizeof(int16_t));
            }
        }
        else if(bitsPerSample == 24)
        {
            for(size_t i = 0; i < audioBuffer.size(); i++)
            {
                float fSample = audioBuffer[i];

                if(fSample > 1.0f) fSample = 1.0f;
                if(fSample < -1.0f) fSample = -1.0f;

                int32_t sample32 = static_cast<int32_t>(std::round(fSample * 8388608.0f));

                if (sample32 > 8388607)  sample32 = 8388607;
                if (sample32 < -8388608) sample32 = -8388608;

                uint8_t b0 = static_cast<uint8_t>(sample32 & 0xFF);
                uint8_t b1 = static_cast<uint8_t>((sample32 >> 8) & 0xFF);
                uint8_t b2 = static_cast<uint8_t>((sample32 >> 16) & 0xFF);

                file.write(reinterpret_cast<char*>(&b0), 1);
                file.write(reinterpret_cast<char*>(&b1), 1);
                file.write(reinterpret_cast<char*>(&b2), 1);
            }
        }
        else if(bitsPerSample == 32)
        {
            for(size_t i = 0; i < audioBuffer.size(); i++)
            {
                float fSample = audioBuffer[i];
    
                if(fSample > 1.0f) fSample = 1.0f;
                if(fSample < -1.0f) fSample = -1.0f;

                file.write(reinterpret_cast<char*>(&fSample), sizeof(float));
            }
        }
            
        std::cout << "Audio Data size is: " << audioBuffer.size() << std::endl;
        file.close();
        return true;
    }else{
        return false;
    }
}


void WavFileManager::printInfo()
{
    if(audioBuffer.size() > 0)
    {
        std::cout << "File Size is: " << fileSize << std::endl;
        std::cout << "SampleRate: " << sampleRate << std::endl;
        std::cout << "NumChannels: " << numChannels << std::endl;
        std::cout << "Bits Per Sample: " << bitsPerSample << std::endl;
        std::cout << "File is: " << durationInSeconds << " second long.." << std::endl;
        std::cout << "SubChunk2 Size: " << subChunk2Size << std::endl;
    }
}