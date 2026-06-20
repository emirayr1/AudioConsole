#include <iostream>
#include <fstream>
#include <string>
#include "ConsoleInterface.h"
#include "../include/Effect/AudioEffect.h"

void ConsoleInterface::showMenu(const std::string& title, const std::vector<MenuItem>& items)
{
    while (true) // Geçersiz girdilerde menünün kaybolmaması için döngü
    {
        std::cout << "\n=== " << title << " ===\n";
        for (const auto& item : items) {
            std::cout << item.key << " - " << item.text << "\n";
        }
        std::cout << "Your Choice: ";

        std::string line;
        std::getline(std::cin, line);

        if (line.empty() || line.length() > 1) {
            std::cout << "Invalid Option, try again!\n";
            continue;
        }

        char choice = line[0];

        // Kullanıcının bastığı tuşu listede arıyoruz
        for (const auto& item : items) {
            if (item.key == choice) {
                item.action(); // Eşleşen fonksiyonu tetikle ve çık!
                return;
            }
        }
        std::cout << "Invalid Option, try again!\n";
    }
}

void ConsoleInterface::displayMainMenu()
{
    std::vector<MenuItem> mainM = {
        {'1', "Load Wav File",     [this]() { c_loadWav(); displayMainMenu(); }},
        {'2', "Basic Info",        [this]() { FileManager.printInfo(); displayMainMenu(); }},
        {'3', "Apply Effect",      [this]() { displayEffectMenu(); }},
        {'4', "Crop File",         [this]() { /* Crop fonksiyonun */ displayMainMenu(); }},
        {'5', "Save Wav File",     [this]() { c_saveWav(); displayMainMenu(); }},
        {'6', "Exit",              [this]() { std::cout << "Bye Bye!\n"; }}
    };

    showMenu("WELCOME THE AUDIO CONSOLE", mainM);
}

void ConsoleInterface::displayEffectMenu()
{
    // 1. Önce aktif efekt zincirini güzelce bir başlık olarak hazırlayalım
    std::string menuTitle = "Effect Menu\n[Current Chain: ";
    
    const auto& chain = Processor.getEffectChain();
    if (chain.empty()) {
        menuTitle += "Empty]";
    } else {
        for (size_t i = 0; i < chain.size(); ++i) {
            menuTitle += chain[i]->getName();
            if (i < chain.size() - 1) menuTitle += " -> "; // Efektlerin arasına ok işareti koyar
        }
        menuTitle += "]";
    }

    std::vector<MenuItem> effectM = {
        {'1', "Distortion", [this]() { displayDistortionOptions(); }},
        {'2', "Reverb",     [this]() { /* Reverb çağrısı */ }},
        {'3', "Gain",       [this]() { displayGainOptions(); }},
        {'4', "Geri Dön",   [this]() { /* Boş bırakıyoruz, sadece showMenu'den çıkacak */ }}
    };
    
    showMenu(menuTitle, effectM);
}

void ConsoleInterface::displayDistortionOptions()
{
    // Buradaki lambdalar doğrudan senin AudioProcessor sınıfına sinyal gönderir!
    std::vector<MenuItem> distM = {
        {'1', "Hardclip", [this]() { Processor.updateEffectChain(AudioProcessor::EffectType::Distortion, 1); displayMainMenu(); }},
        {'2', "Softclip", [this]() { Processor.updateEffectChain(AudioProcessor::EffectType::Distortion, 2); displayMainMenu(); }},
        {'3', "Fuzz",     [this]() { Processor.updateEffectChain(AudioProcessor::EffectType::Distortion, 3); displayMainMenu(); }},
        {'4', "Back", [this]() { displayEffectMenu(); }}
    };

    showMenu("Distortion Type", distM);
}

void ConsoleInterface::displayGainOptions()
{
    std::vector<MenuItem> distM = {
        {'1', "Linear (0 - 1)", [this](){ Processor.updateEffectChain(AudioProcessor::EffectType::Gain, 0); displayMainMenu(); }},
        {'2', "Decibel (-48dbFS - 0dbFS)", [this](){ Processor.updateEffectChain(AudioProcessor::EffectType::Gain, 1); displayMainMenu(); }},
        {'3', "Back", [this](){ displayMainMenu(); }}
    };

    showMenu("Gain Options", distM);
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

int ConsoleInterface::run()
{
    displayMainMenu();
    
    return 0;
}