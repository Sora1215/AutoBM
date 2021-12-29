/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim, special thanks to Ricky.Cha

    << This project was started because the author was pissed off thanks to his company that threw him and his colleage some crazy workload >>

    ~ External libraries used : LibXL (Paid version)
    ~ Build instructions : Just hit build, then make sure to have the required dlls together with the final executable. 
        This is currently set as a post-build event.
    ~ Any inquiries about the project may be forwarded either to keithrek@hanmail.net, or straight to the author, who should be working at 2F, JM building.

    Windows SDK 10
    Visual Studio 2022
    ISO C++17

    $(SolutionDir)external\libxl\include_cpp
    $(SolutionDir)external\libxl\lib64
    libxl.lib

    copy "$(SolutionDir)external\libxl\bin64" "$(OutDir)"
    libxl.dll

    Filename : AutoBM.cpp
    Edited : 2021-12-28
*/

#include "ConsoleManager.h"
#include "XLDataWrapper.h"



int main()
{
    CONSOLE.SetupLocal();

    XL_DW.RemoveZeroWidthSpace(L"Table/", L".xlsm");
    XL_DW.RemoveZeroWidthSpace(L"Table/", L".xlsx");

    //XL_DW.CheckForFormula(L"ItemTable.xlsx");
    //XL_DW.CheckForZeroWidthSpace(L"ItemTable.xlsx");
    //XL_DW.CheckForItemLocal(L"ItemTable.xlsx");



    return 0;
}