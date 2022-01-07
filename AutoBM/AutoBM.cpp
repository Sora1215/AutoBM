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

#include "JsonDataWrapper.h"
#include "ConsoleManager.h"
#include "XLDataWrapper.h"



int main()
{
    CONSOLE.SetupLocal();

    P_STRING("ItemIconConnector v1.0 by Sora.Kim", C_PRINT_PARAMETER);
    P_STRING("", C_PRINT_PARAMETER);
    P_STRING("Make sure that this executable is placed just inside the r1_asset repo folder.", C_PROCEDURE_PARAMETER);
    P_STRING("", C_PRINT_PARAMETER);

    JSON_DW.ConnectItemIcon();

    //P_STRING("ExcelZeroWidthSpaceRemover v1.2 by Sora.Kim", C_PRINT_PARAMETER);
    //P_STRING("", C_PRINT_PARAMETER);
    //P_STRING("Make sure that this executable is placed just outside the Table repo folder.", C_PROCEDURE_PARAMETER);
    //P_STRING("", C_PRINT_PARAMETER);

    //XL_DW.RemoveZeroWidthSpace(L"Table/", { L".xlsx", L".xlsm" });

    return 0;
}