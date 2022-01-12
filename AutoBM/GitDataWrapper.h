/*
	Winter 2021
	Automate your work assignment
	Written by SeungGeon Kim

	Filename : GitDataWrapper.h
	Created : 2022-01-11
*/

#pragma once

#define GIT_DW GitDataWrapper::Instance()

#include "GlobalDefines.h"



class GitDataWrapper
{

public:

	// Singleton pattern, placed deleted functions as public for better error messages
	static GitDataWrapper& Instance() noexcept;
	GitDataWrapper(const GitDataWrapper&) = delete;
	void operator=(const GitDataWrapper&) = delete;

	void DiffCheckTableRepo(KR_STR) noexcept;

private:

	// Making it private to prevent multiple copies of this appearing
	GitDataWrapper() = default;

};