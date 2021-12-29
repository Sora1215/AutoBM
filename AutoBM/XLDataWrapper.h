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

	// Checks for any formlua inside the XLSXsheet.
	void CheckForFormula(KR_STR) noexcept;

	// Ensures that all the item keys has corresponding local keys
	void CheckForZeroWidthSpace(KR_STR) noexcept;

	// Ensures that all the item keys has corresponding local keys
	void CheckForItemLocal(KR_STR) noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	XLDataWrapper() = default;

	template <class T>
	T CreateXLSXBook() noexcept;

	// Maps used by MatchItemLocal function
	std::unordered_map<int, int> mItemTable_ItemIndex_Row;
	std::unordered_map<int, KR_STR> mLocalTable_ItemIndex_Name;
	std::unordered_map<int, KR_STR> mLocalTable_ItemIndex_Desc;
	std::unordered_map<int, int> mLocalTable_ItemIndex_Count;

};