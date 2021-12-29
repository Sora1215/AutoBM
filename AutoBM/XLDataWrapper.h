/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

	Filename : XLDataWrapper.h
	Edited : 2021-11-28
*/

#pragma once

#define XL_DW XLDataWrapper::Instance()

#include "GlobalDefines.h"

#include <unordered_map>



class XLDataWrapper
{

public:

	// Singleton pattern, placed deleted functions as public for better error messages
	static XLDataWrapper& Instance() noexcept;
	XLDataWrapper(const XLDataWrapper&) = delete;
	void operator=(const XLDataWrapper&) = delete;

	// Deal with any formluas
	void CheckForFormula(KR_STR) noexcept;

	// Deal with any zero width spaces
	void CheckForZeroWidthSpace(KR_STR) noexcept;
	void CheckForZeroWidthSpace_Recursive(KR_STR) noexcept;
	void RemoveZeroWidthSpace(KR_STR, KR_STR) noexcept;
	void RemoveZeroWidthSpace_Recursive(KR_STR, KR_STR) noexcept;

	// Ensures that all the item keys has corresponding local keys
	void CheckForItemLocal(KR_STR) noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	XLDataWrapper() = default;

	template <class T>
	[[nodiscard]] T CreateXLSXBook() noexcept;

	template <class T>
	void RepeatLambdaForAllFilesByExtension(KR_STR, KR_STR, T) noexcept;

	template <class T>
	void RepeatLambdaForAllFilesByExtension_Recursive(KR_STR, KR_STR, T) noexcept;

	// Maps used by MatchItemLocal function
	std::unordered_map<int, int> mItemTable_ItemIndex_Row;
	std::unordered_map<int, KR_STR> mLocalTable_ItemIndex_Name;
	std::unordered_map<int, KR_STR> mLocalTable_ItemIndex_Desc;
	std::unordered_map<int, int> mLocalTable_ItemIndex_Count;

};