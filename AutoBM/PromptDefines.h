/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : PromptDefines.h
    Edited : 2021-12-28
*/

#pragma once
             
// None
#define WAITFORINPUT                    system("PAUSE");

// None
#define NEWLINE                         P_STRING("", C_PRINT);



// ---



// None
#define PRINT_PROCEDURE                 P_STRING("Initiating ", C_PROCEDURE, false);\
                                        P_STRING(__func__, C_PROCEDURE_PARAMETER, false);\
                                        P_STRING(" procedure...", C_PROCEDURE);

// None                                 
#define PRINT_SCANNING                  P_STRING("Scanning...", C_PROCEDURE_PARAMETER);

// None                                 
#define PRINT_SCANCOMPLETE              P_STRING("Scan complete.", C_PROCEDURE_PARAMETER);

// None                                 
#define PRINT_SAVING                    P_STRING("Saving...", C_PROCEDURE_PARAMETER);

// None                                 
#define PRINT_SAVECOMPLETE              P_STRING("Save complete.", C_PROCEDURE_PARAMETER);



// ---



// [int, int]                           
#define PRINT_CELLPOSITION(row, col)    P_STRING(">> Row : ", C_PROCEDURE, false);\
                                        P_DOUBLE(row, C_PROCEDURE_PARAMETER, false);\
                                        P_STRING(", Col : ", C_PROCEDURE, false);\
                                        P_DOUBLE(col, C_PROCEDURE_PARAMETER, false);\
                                        P_STRING(" <<", C_PROCEDURE);

// [KR_STR] 
#define PRINT_ONFILELOAD(fileName)      P_STRING("File with the name of : ", C_PRINT, false);\
                                        P_STRING(fileName, C_PRINT_PARAMETER, false);\
                                        P_STRING(" was successfully loaded.", C_PRINT);\
                                        P_STRING("Resource allocated.", C_MEMORY);

// [KR_STR] 
#define PRINT_ONFILEUNLOAD(fileName)    P_STRING("File with the name of : ", C_PRINT, false);\
                                        P_STRING(fileName, C_PRINT_PARAMETER, false);\
                                        P_STRING(" was successfully unloaded.", C_PRINT);\
                                        P_STRING("Resource deallocated.", C_MEMORY);

// [KR_STR] 
#define PRINT_PATHFOUND(path)           P_STRING("Path with the name of : ", C_PROCEDURE, false);\
                                        P_STRING(path, C_PROCEDURE_PARAMETER, false);\
                                        P_STRING(" was successfully located.", C_PROCEDURE);

// [KR_STR]
#define PRINT_JSONPARSESUCCESS(path)    P_STRING("Successfully parsed json at : ", C_PRINT, false);\
                                        P_STRING(path, C_PRINT_PARAMETER);

// [KR_STR]
#define PRINT_JSONFINDSUCCESS           P_STRING("Successfully located required keys inside the file.", C_PRINT);



// ---



 // [KR_STR]
#define ERROR_FILENOTFOUND(fileName)    P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                                        P_STRING("File with the name of : ", C_PRINT, false);\
                                        P_STRING(fileName, C_PRINT_PARAMETER, false);\
                                        P_STRING(" was not found.", C_PRINT);

// [KR_STR] 
#define ERROR_PATHNOTFOUND(path)        P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                                        P_STRING("Path with the name of : ", C_PRINT, false);\
                                        P_STRING(path, C_PRINT_PARAMETER, false);\
                                        P_STRING(" was not found.", C_PRINT);

// [int] 
#define ERROR_SHEETNOTFOUND(index)      P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                                        P_STRING("Sheet with the index of : ", C_PRINT, false);\
                                        P_DOUBLE(index, C_PRINT_PARAMETER, false);\
                                        P_STRING(" was not found.", C_PRINT);

// [std::wstring] 
#define ERROR_JSONPARSEFAIL(path)       P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                                        P_STRING("Failed to parse json at : ", C_PRINT, false);\
                                        P_STRING(path, C_PRINT_PARAMETER);

#define ERROR_JSONFINDFAIL              P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                                        P_STRING("Failed to locate required keys inside the file.", C_PRINT);

// [int, int] 
#define ERROR_OUTOFRANGE(begin, end)    P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                                        P_STRING("Please type in a number between : ", C_PRINT, false);\
                                        P_DOUBLE(begin, C_PRINT_PARAMETER, false);\
                                        P_STRING(", and ", C_PRINT, false);\
                                        P_DOUBLE(end, C_PRINT_PARAMETER, false);\
                                        P_STRING(" - ", C_PRINT, false);