#include <iostream>
#include <fstream>
#include <string>
#include "ConsoleInterface.h"
#include "../include/Effect/AudioEffect.h"
#include "../include/Effect/Gain.h"

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
        {'4', "Crop File",         [this]() { displayCropOptions(); displayMainMenu(); }},
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
    std::vector<MenuItem> distM = {
        {'1', "Hardclip", [this]() { 
            displayMainMenu(); 
        }},
        {'2', "Softclip", [this]() { displayMainMenu(); }},
        {'3', "Fuzz",     [this]() { displayMainMenu(); }},
        {'4', "Back", [this]() { displayEffectMenu(); }}
    };

    showMenu("Distortion Type", distM);
}

void ConsoleInterface::displayGainOptions()
{
    std::vector<MenuItem> gainM = {
        {'1', "Linear (0 - 1)", [this](){ 
            float val = getFloatInput("\n Please give a value between 0 - 1: ", 0.0f, 1.0f);

            Gain* gain = Processor.updateEffectChain<Gain>();

            gain->setType(Gain::GainType::Linear);
            gain->setValue(val);

            displayMainMenu();
         }},
        {'2', "Decibel (-48dbFS - 0dbFS)", [this](){ 
            float val = getFloatInput("\n Please give a value between -48.0 - 0: ", -48.0f, 0.0f);

            Gain* gain = Processor.updateEffectChain<Gain>();

            gain->setType(Gain::GainType::Decibel);
            gain->setValue(val);

            displayMainMenu();
         }},
        {'3', "Back", [this](){ displayMainMenu(); }}
    };

    showMenu("Gain Options", gainM);
}

void ConsoleInterface::displayCropOptions()
{
    std::vector<MenuItem> cropM = {
        {'1', "Crop by Sample Count", [this]() { 
            std::pair<float, float> values = getRangeValue("Enter Start and Finish Value!", 0.0f, FileManager.getAudioBufferSize() / FileManager.getNumChannels());
        
        FileManager.cropWaveFile(values, 1);
        displayMainMenu(); 

        }},
        {'2', "Crop by Second", [this]() { 
            std::pair<float, float> values = getRangeValue("Enter Start and Finish Value!", 0.0f, (float)FileManager.durationInSeconds);
            FileManager.cropWaveFile(values, 0);
            displayMainMenu(); }},
        {'3', "Back", [this](){ displayMainMenu(); }}
    };

    std::cout << "Your File is: " << FileManager.durationInSeconds << "seconds long" << std::endl;
    showMenu("Crop Options", cropM);
}

std::pair<float, float> ConsoleInterface::getRangeValue(const std::string& prompt, float fileMin, float fileMax)
{
    std::cout << prompt << " (Gecerli aralik: " << fileMin << " - " << fileMax << ")\n";
    float minVal = 0.0f;
    float maxVal = 0.0f;
    
    while (true)
    {
        minVal = getFloatInput("Enter Start Value: ", fileMin, fileMax);
        maxVal = getFloatInput("Enter Finish Value: ", fileMin, fileMax);

        if (minVal <= maxVal)
        {
            return std::make_pair(minVal, maxVal);
        }
        else
        {
            std::cout << "Hata: Baslangic degeri, bitis degerinden buyuk olamaz. Lutfen tekrar girin.\n";
        }
    }
}

float ConsoleInterface::getFloatInput(const std::string& prompt, float minValue, float maxValue)
{
    float value = 0.0f;
    while (true)
    {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        try
        {
            value = std::stof(input); // String'i float'a çevir
            if (value >= minValue && value <= maxValue)
            {
                return value; // Değer başarılı ve sınırlar içindeyse döngüyü kırıp döndür
            }
            else
            {
                std::cout << "Hata: Lutfen " << minValue << " ile " << maxValue << " arasinda bir deger girin.\n";
            }
        }
        catch (const std::invalid_argument&)
        {
            std::cout << "Hata: Gecersiz format! Lutfen bir sayi girin.\n";
        }
        catch (const std::out_of_range&)
        {
            std::cout << "Hata: Girdiginiz sayi sınırların dışında.\n";
        }
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

int ConsoleInterface::run()
{
    displayMainMenu();
    
    return 0;
}