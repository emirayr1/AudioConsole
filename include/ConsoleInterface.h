#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "WavFileManager.h"
#include "AudioProcessor.h"


struct MenuItem{
    char key;
    std::string text;
    std::function<void()> action;
};

enum class MenuOption
{
    LoadWav = '1',
    BasicInfo = '2',
    ApplyEffect = '3',
    CropFile = '4',
    SaveWavFile = '5',
    Exit = '6',
    Invalid
};

enum class EffectMenuOption
{
    Distortion = '1',
    Reverb = '2',
    Gain = '3',
    Exit = '4'
};

class ConsoleInterface
{
public:
    void showMenu(const std::string& title, const std::vector<MenuItem>& items);
    void displayEffectMenu();
    void displayMainMenu();
    
    
    void displayDistortionOptions();
    void displayReverbOptions();
    void displayGainOptions();

    bool c_loadWav();
    bool c_saveWav();

    int run();

    WavFileManager FileManager;
    AudioProcessor Processor;

    std::string filePath;
};