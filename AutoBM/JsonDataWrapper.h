/*
	Winter 2021
	Automate your work assignment
	Written by SeungGeon Kim

	Filename : JsonDataWrapper.h
	Edited : 2021-11-28
*/

#pragma once

#define JSON_DW JsonDataWrapper::Instance()

#include "GlobalDefines.h"



class JsonDataWrapper
{

public:

	// Singleton pattern, placed deleted functions as public for better error messages
	static JsonDataWrapper& Instance() noexcept;
	JsonDataWrapper(const JsonDataWrapper&) = delete;
	void operator=(const JsonDataWrapper&) = delete;

	// Connect item icon through json directly
	void ConnectItemIcon() noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	JsonDataWrapper() = default;

};