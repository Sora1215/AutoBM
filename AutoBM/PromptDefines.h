/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : PromptDefines.h
    Edited : 2021-12-28
*/

#pragma once

// None
#define WAITFORINPUT        system("PAUSE");

// None
#define PRINT_PROCEDURE     P_STRING("Initiating ", C_PROCEDURE, false);\
                            P_STRING(__func__, C_PROCEDURE_PARAMETER, false);\
                            P_STRING(" procedure...", C_PROCEDURE);

// [KR_STR] paramFileName
#define PRINT_ONFILELOAD   P_STRING("File with the name of : ", C_PRINT, false);\
                            P_STRING(paramFileName, C_PRINT_PARAMETER, false);\
                            P_STRING(" was successfully loaded.", C_PRINT);\
                            P_STRING("Resource allocated.", C_MEMORY);

// [KR_STR] paramFileName
#define PRINT_ONFILEUNLOAD P_STRING("File with the name of : ", C_PRINT, false);\
                            P_STRING(paramFileName, C_PRINT_PARAMETER, false);\
                            P_STRING(" was successfully unloaded.", C_PRINT);\
                            P_STRING("Resource deallocated.", C_MEMORY);

// [KR_STR] baseDirectory
#define PRINT_PATHFOUND    P_STRING("Path with the name of : ", C_PRINT, false);\
                            P_STRING(baseDirectory, C_PRINT_PARAMETER, false);\
                            P_STRING(" was successfully located.", C_PRINT);

// [KR_STR] baseDirectory
#define PRINT_PATHNOTFOUND P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                            P_STRING("Path with the name of : ", C_PRINT, false);\
                            P_STRING(baseDirectory, C_PRINT_PARAMETER, false);\
                            P_STRING(" was not found.", C_PRINT);

// None
#define PRINT_RETRY         P_STRING("Please retry after fixing all issues. ", C_ERROR);

// None
#define PRINT_SCANNING      P_STRING("Scanning...", C_PROCEDURE_PARAMETER);

// None
#define PRINT_SCANCOMPLETE  P_STRING("Scan complete.", C_PROCEDURE_PARAMETER);

// None
#define PRINT_SAVING        P_STRING("Saving modifications...", C_PROCEDURE_PARAMETER);

// None
#define PRINT_SAVECOMPLETE  P_STRING("Save complete.", C_PROCEDURE_PARAMETER);

 // [KR_STR] paramFileName
#define ERROR_FILENOTFOUND  P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                            P_STRING("File with the name of : ", C_PRINT, false);\
                            P_STRING(paramFileName, C_PRINT_PARAMETER, false);\
                            P_STRING(" was not found.", C_PRINT);

// [int] inputSheetIndex
#define ERROR_SHEETNOTFOUND P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                            P_STRING("Sheet with the index of : ", C_PRINT, false);\
                            P_DOUBLE(inputSheetIndex, C_PRINT_PARAMETER, false);\
                            P_STRING(" was not found.", C_PRINT);