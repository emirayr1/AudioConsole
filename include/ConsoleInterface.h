#pragma once

#include <iostream>
#include "WavFileManager.h"
#include "AudioProcessor.h"

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
    void displayMainMenu();
    void displayEffectMenu();
    void displayDistortionOptions();
    void handleMenuSelection(MenuOption option);
    void handleEffectSelection(EffectMenuOption option);
    void handleGainTest();

    bool c_loadWav();
    bool c_saveWav();

    int run();

    MenuOption getMenuSelection();
    EffectMenuOption getEffectMenuSelection();

    WavFileManager FileManager;
    AudioProcessor Processor;


    std::string filePath;
};