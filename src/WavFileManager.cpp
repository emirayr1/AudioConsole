#include "WavFileManager.h"
#include <fstream>

bool WavFileManager::loadWav(std::string filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    
    if(file.is_open()){
        file.seekg(0);
        file.read(chunkID, 4);
        file.read(reinterpret_cast<char*>(&chunkSize), sizeof(chunkSize));
        file.read(format, 4);
        file.read(subChunk1ID, 4);
        file.read(reinterpret_cast<char*>(&subChunk1Size), sizeof(subChunk1Size));
        file.read(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
        file.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
        file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
        file.read(reinterpret_cast<char*>(&byteRate), sizeof(byteRate));
        file.read(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
        file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));
        file.read(subChunk2ID, 4);
        file.read(reinterpret_cast<char*>(&subChunk2Size), sizeof(subChunk2Size));
        
        file.seekg(0, std::ios::end);
        fileSize = static_cast<uint64_t>(file.tellg());
        
        if(numChannels > 0 && bitsPerSample > 0 && sampleRate > 0)
        {
            durationInSeconds = static_cast<double>(subChunk2Size) / byteRate;
        }

        std::cout << "File Size is: " << fileSize << std::endl;
        std::cout << "SampleRate: " << sampleRate << std::endl;
        std::cout << "NumChannels: " << numChannels << std::endl;
        std::cout << "Bits Per Sample: " << bitsPerSample << std::endl;
        std::cout << "File is: " << durationInSeconds << " second long.." << std::endl;
        return true;
    }else{
        return false;
    }
    
    
}

bool WavFileManager::saveWav(std::string filePath)
{
    std::ofstream file(filePath, std::ios::binary);

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
        std::cout << "We're at " << file.tellp() << " .Byte" << std::endl;
            
        file.close();
        return true;
    }else{
        return false;
    }
}