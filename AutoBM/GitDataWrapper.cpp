/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : GitDataWrapper.cpp
    Created : 2022-01-11
*/

#include "GitDataWrapper.h"



GitDataWrapper& GitDataWrapper::Instance() noexcept
{
    static GitDataWrapper instance;
    return instance;
}