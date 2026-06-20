#pragma once

#include <iostream>
#include "WavFileManager.h"

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

class ConsoleInterface
{
public:
    void displayMainMenu();
    void handleMenuSelection(MenuOption option);

    bool c_loadWav();
    bool c_saveWav();

    int run();

    MenuOption getMenuSelection();
    WavFileManager FileManager;


    std::string filePath;
};