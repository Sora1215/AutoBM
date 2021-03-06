/*
	Winter 2021
	Automate your work assignment
	Written by SeungGeon Kim

	Filename : ConsoleManager.h
	Created : 2021-11-28
*/

#pragma once

#define CONSOLE ConsoleManager::Instance()

#define P_STRING ConsoleManager::Instance().PrintString
#define P_DOUBLE ConsoleManager::Instance().PrintDouble

#define C_PRINT ConsoleManager::Color::Green // For general prompting
#define C_PRINT_PARAMETER ConsoleManager::Color::Yellow // For general parameters
#define C_PROCEDURE ConsoleManager::Color::Blue // For procedure-related prompting
#define C_PROCEDURE_PARAMETER ConsoleManager::Color::Skyblue // For procedure-related parameters
#define C_ERROR ConsoleManager::Color::Red // For error messages
#define C_MEMORY ConsoleManager::Color::Purple // For memory-related prompting

#include "GlobalDefines.h"
#include <string>



class ConsoleManager
{

public:

	// Singleton pattern, placed deleted functions as public for better error messages
	static ConsoleManager& Instance() noexcept;
	ConsoleManager(const ConsoleManager&) = delete;
	void operator=(const ConsoleManager&) = delete;

	void SetupLocal() noexcept;

	enum class Color
	{
		Green,
		Blue,
		Skyblue,
		Yellow,
		Red,
		Purple
	};

	// A safe version that works even for special characters without destroying the windows console
	void PrintString(std::wstring, Color, bool = true) noexcept;
	void PrintString(std::string, Color, bool = true) noexcept;
	void PrintString(KR_STR, Color, bool = true) noexcept;
	void PrintString(EN_STR, Color, bool = true) noexcept;

	void PrintDouble(double, Color, bool = true) noexcept;
	void PrintDouble(int, Color, bool = true) noexcept;
	void PrintDouble(size_t, Color, bool = true) noexcept;

	[[nodiscard]] std::string ConvertWstringToString(std::wstring) const noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	ConsoleManager() = default;

	template <class T>
	void PrintHelper(T&&, Color, bool) noexcept;



	[[nodiscard]] int ReturnColor(Color) noexcept;

	void ClearConsole() noexcept;

	void CheckTimer() noexcept;



	int mClearTimer = 0;

};