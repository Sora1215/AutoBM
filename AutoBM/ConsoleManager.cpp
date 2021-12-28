/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : ConsoleManager.cpp
	Edited : 2021-11-27
*/

#include "ConsoleManager.h"

#include <windows.h>
#include <iostream>



ConsoleManager& ConsoleManager::Instance() noexcept
{
    static ConsoleManager instance;
    return instance;
}

void ConsoleManager::SetupLocal() const noexcept
{
    std::locale mylocale("");
    std::wcout.imbue(mylocale);
}

void ConsoleManager::PrintPosition(int row, int col, Color paramColor, bool paramIsNewLine) noexcept
{
    auto lambda = [=]()
    {
        std::wcout << "(" << row << ", " << col << ") -> ";
    };

    PrintHelper(lambda, paramColor, paramIsNewLine);
}

void ConsoleManager::PrintString(KR_STR paramText, Color paramColor, bool paramIsNewLine) noexcept
{
    auto lambda = [=]()
    {
        std::wcout << paramText;
    };

    PrintHelper(lambda, paramColor, paramIsNewLine);
}

void ConsoleManager::PrintString(std::string paramText, Color paramColor, bool paramIsNewLine) noexcept
{
    auto lambda = [=]()
    {
        std::wcout << paramText.c_str();
    };

    PrintHelper(lambda, paramColor, paramIsNewLine);
}

void ConsoleManager::PrintDouble(double paramDouble, Color paramColor, bool paramIsNewLine) noexcept
{
    auto lambda = [=]()
    {
        std::wcout << paramDouble;
    };

    PrintHelper(lambda, paramColor, paramIsNewLine);
}

void ConsoleManager::PrintDouble(int paramInt, Color paramColor, bool paramIsNewLine) noexcept
{
    auto lambda = [=]()
    {
        std::wcout << paramInt;
    };

    PrintHelper(lambda, paramColor, paramIsNewLine);
}

template <class T>
void ConsoleManager::PrintHelper(T&& lambda, ConsoleManager::Color paramColor, bool paramIsNewLine) noexcept
{
    ConsoleManager::CheckTimer();

    HANDLE hConsole;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, static_cast<WORD>(ConsoleManager::ReturnColor(paramColor)));

    lambda();

    if (paramIsNewLine == true)
    {
        std::wcout << std::endl;
    }
}

int ConsoleManager::ReturnColor(Color paramColor) const noexcept
{
    switch (paramColor)
    {
    case Color::Green:
        return 0xA;
    case Color::Blue:
        return 0x3;
    case Color::Skyblue:
        return 0xB;
    case Color::Parameter:
        return 0xE;
    case Color::Error:
        return 0xC;
    case Color::Special:
        return 0xD;
    }

    return -1;
}

void ConsoleManager::CheckTimer() noexcept
{
    mClearTimer += 1;

    if (mClearTimer >= REFRESH_RATE)
    {
        system("CLS");
        mClearTimer = 0;
    }
}