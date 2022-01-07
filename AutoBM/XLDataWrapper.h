/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

	Filename : XLDataWrapper.h
	Edited : 2021-11-28
*/

#pragma once

#define XL_DW XLDataWrapper::Instance()

#include <initializer_list>
#include "GlobalDefines.h"
#include <unordered_map>



class XLDataWrapper
{

public:

	// Singleton pattern, placed deleted functions as public for better error messages
	static XLDataWrapper& Instance() noexcept;
	XLDataWrapper(const XLDataWrapper&) = delete;
	void operator=(const XLDataWrapper&) = delete;

	// Deal with any zero-width spaces
	void RemoveZeroWidthSpace(KR_STR, std::initializer_list<KR_STR>) noexcept;

	[[nodiscard]] std::unordered_map<std::string, std::vector<std::string>> MapItemCodeByXLSX(KR_STR) noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	XLDataWrapper() = default;

	template <class T>
	void RepeatLambdaForAllCellsByXLSX(KR_STR, bool, T&&) noexcept;

	template <class T>
	void RepeatLambdaForAllFilesByExtension(KR_STR, KR_STR, T&&) noexcept;

	// Folders inside folders inside folders...
	template <class T>
	void RepeatLambdaForAllFilesByExtension_Recursive(KR_STR, KR_STR, T&&) noexcept;



	// This guy cannot be treated as a const function due to internal library structure which I cannot reach...
	template <class T>
	[[nodiscard]] T CreateXLSXBook() noexcept;

	template <class T>
	void PrintCellType(T) noexcept;

	template <class T>
	[[nodiscard]] int PromptSheets(T) noexcept;



	bool mEditFlag = false;
	int mEditCount = 0;

};