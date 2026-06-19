#pragma once

#include <iostream>
#include "WavFileManager.h"

enum class MenuOption
{
    LoadWav = '1',
    BasicInfo = '2',
    ApplyEffect = '3',
    CropFile = '4',
    ChangeWavFile = '5',
    SaveWavFile = '6',
    Exit = '7',
    Invalid
};

class ConsoleInterface
{
public:
    void displayMainMenu();
    void handleMenuSelection(MenuOption option);

    bool c_loadWav();
    

    int run();

    MenuOption getMenuSelection();

    std::string filePath;
};