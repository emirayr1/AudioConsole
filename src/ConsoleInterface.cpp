#include <iostream>
#include <fstream>
#include <string>
#include "ConsoleInterface.h"

MenuOption ConsoleInterface::getMenuSelection()
{
    std::string line;
    std::getline(std::cin, line);

    if(line.empty() || line.length() > 1){
        return MenuOption::Invalid;
    }

    MenuOption selected = static_cast<MenuOption>(line[0]);

    if(selected >= MenuOption::LoadWav && selected <= MenuOption::Exit){
        return selected;
    }

    return MenuOption::Invalid;
}

EffectMenuOption ConsoleInterface::getEffectMenuSelection()
{
    std::string line;
    std::getline(std::cin, line);

    if(line.empty() || line.length() > 1){
        return EffectMenuOption::Exit;
    }

    EffectMenuOption selected = static_cast<EffectMenuOption>(line[0]);

    if(selected >= EffectMenuOption::Distortion && selected <= EffectMenuOption::Exit){
        return selected;
    }

    return EffectMenuOption::Exit;
}

void ConsoleInterface::displayMainMenu()
{
    std::cout << "WELCOME THE AUDIO CONSOLE\n";
    std::cout << "1 - Load Wav File" << std::endl;
    std::cout << "2 - Basic Info" << std::endl;
    std::cout << "3 - Apply Effect" << std::endl;
    std::cout << "4 - Crop File" << std::endl;
    std::cout << "5 - Save Wav File" << std::endl;
    std::cout << "6 - Exit" << std::endl;
    
    MenuOption selectedOption = getMenuSelection();
    handleMenuSelection(selectedOption);
}

void ConsoleInterface::handleMenuSelection(MenuOption option)
{
    switch (option)
    {
    case MenuOption::LoadWav:
        std::cout << "Loading Wav" << std::endl;
        c_loadWav();
        displayMainMenu();
        break;
    case MenuOption::BasicInfo:
        FileManager.printInfo();
        displayMainMenu();
        break;
    case MenuOption::ApplyEffect:
        displayEffectMenu();
        displayMainMenu();
        break;
    case MenuOption::CropFile:
        break;
    case MenuOption::SaveWavFile:
        c_saveWav();
        break;
    case MenuOption::Exit:
        break;
    default:
        break;
    }
}

bool ConsoleInterface::c_loadWav()
{
    std::cout << "Please Enter Valid File Path" << std::endl;
    std::getline(std::cin, filePath);

    if(!filePath.empty() && filePath.front() == '"' && filePath.back() == '"')
    {
        filePath = filePath.substr(1, filePath.size() - 2);
    }

    
    if(FileManager.loadWav(filePath)){ return true; }
    else{
        std::cerr << "Failed to open or parse Wave file.\n";
        return false;
    }
}

bool ConsoleInterface::c_saveWav()
{
    std::cout << "Please Enter Valid File Path" << std::endl;
    std::getline(std::cin, filePath);

    if(!filePath.empty() && filePath.front() == '"' && filePath.back() == '"')
    {
        filePath = filePath.substr(1, filePath.size() - 2);
    }

    Processor.applyEffect(FileManager.getAudioBuffer());

    if(FileManager.saveWav(filePath)){ return true; }
    else{
        std::cerr << "Failed to save Wave file.\n";
        return false;
    }
}

void ConsoleInterface::displayEffectMenu()
{
    std::cout << "Please Choose Effect Type" << std::endl;
    std::cout << "1 - Distortion" << std::endl;
    std::cout << "2 - Reverb" << std::endl;
    std::cout << "3 - Gain" << std::endl;
    std::cout << "4 - Exit" << std::endl;

    EffectMenuOption selectedEffect = getEffectMenuSelection();
    handleEffectSelection(selectedEffect);
}

void ConsoleInterface::handleEffectSelection(EffectMenuOption option)
{
    switch (option)
    {
    case EffectMenuOption::Distortion:
        displayDistortionOptions();
        break;
    case EffectMenuOption::Reverb:
        break;
    case EffectMenuOption::Gain:
        std::cout << "Please Enter Gain Value Between 0 and 1" << std::endl;
        handleGainTest();
        break;
    default:
        break;
    }
}

void ConsoleInterface::handleGainTest()
{
    std::string input;
    std::cin >> input;

    Processor.updateEffectChain(AudioProcessor::EffectType::Gain);
}

void ConsoleInterface::displayDistortionOptions()
{
    std::cout << "Please Choose Distortion Type" << std::endl;
    std::cout << "1 - Hardclip" << std::endl;
    std::cout << "2 - Softclip" << std::endl;
    std::cout << "3 - Fuzz" << std::endl;
    std::cout << "4 - Exit" << std::endl;
}

int ConsoleInterface::run()
{
    displayMainMenu();
    
    return 0;
}