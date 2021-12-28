/*
	Winter 2021
	Automate your work assignment
	Written by SeungGeon Kim

	Filename : ConsoleManager.h
	Edited : 2021-11-27
*/

#pragma once

#define CONSOLE ConsoleManager::Instance()

#define P_POSITION ConsoleManager::Instance().PrintPosition

#define P_STRING ConsoleManager::Instance().PrintString
#define P_DOUBLE ConsoleManager::Instance().PrintDouble

#define C_GREEN ConsoleManager::Color::Green // For general prompting
#define C_BLUE ConsoleManager::Color::Blue // For procedure-related prompting
#define C_SKYBLUE ConsoleManager::Color::Skyblue // For parameters inside the procedure-related prompting
#define C_PARAMETER ConsoleManager::Color::Parameter // For general parameters
#define C_ERROR ConsoleManager::Color::Error // For error messages
#define C_SPECIAL ConsoleManager::Color::Special // For memory-related prompting

#include "GlobalDefines.h"

#include <string>



class ConsoleManager
{

public:

	// Singleton pattern, placed deleted functions as public for better error messages
	static ConsoleManager& Instance() noexcept;
	ConsoleManager(const ConsoleManager&) = delete;
	void operator=(const ConsoleManager&) = delete;

	void SetupLocal() const noexcept;

	enum class Color
	{
		Green,
		Blue,
		Skyblue,
		Parameter,
		Error,
		Special
	};

	void PrintPosition(int, int, Color, bool = true) noexcept;

	void PrintString(KR_STR, Color, bool = true) noexcept;
	void PrintString(std::string, Color, bool = true) noexcept;

	void PrintDouble(double, Color, bool = true) noexcept;
	void PrintDouble(int, Color, bool = true) noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	ConsoleManager() = default;

	template <class T>
	void PrintHelper(T&&, Color, bool) noexcept;

	int ReturnColor(Color) const noexcept;

	void CheckTimer() noexcept;

	int mClearTimer;

};