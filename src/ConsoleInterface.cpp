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

    if(FileManager.saveWav(filePath)){ return true; }
    else{
        std::cerr << "Failed to save Wave file.\n";
        return false;
    }
}

int ConsoleInterface::run()
{
    displayMainMenu();
    
    return 0;
}