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
#define PROMPT_ONFILELOAD   P_STRING("File with the name of : ", C_PROMPT, false);\
                            P_STRING(paramFileName, C_PROMPT_PARAMETER, false);\
                            P_STRING(" was successfully loaded.", C_PROMPT);\
                            P_STRING("Resource allocated.", C_MEMORY);\
                            //system("PAUSE");

// [KR_STR] paramFileName
#define PROMPT_ONFILEUNLOAD P_STRING("File with the name of : ", C_PROMPT, false);\
                            P_STRING(paramFileName, C_PROMPT_PARAMETER, false);\
                            P_STRING(" was successfully unloaded.", C_PROMPT);\
                            P_STRING("Resource deallocated.", C_MEMORY);\
                            //system("PAUSE");

// [KR_STR] baseDirectory
#define PROMPT_PATHFOUND    P_STRING("Path with the name of : ", C_PROMPT, false);\
                            P_STRING(baseDirectory, C_PROMPT_PARAMETER, false);\
                            P_STRING(" was successfully located.", C_PROMPT);\
                            //system("PAUSE");

// [KR_STR] baseDirectory
#define PROMPT_PATHNOTFOUND P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                            P_STRING("Path with the name of : ", C_PROMPT, false);\
                            P_STRING(baseDirectory, C_PROMPT_PARAMETER, false);\
                            P_STRING(" was not found.", C_PROMPT);\
                            //system("PAUSE");

// None
#define PROMPT_RETRY        P_STRING("Please retry after fixing all issues. ", C_ERROR);\
                            //system("PAUSE");

// None
#define PROMPT_SCANREADY    P_STRING("Scan is now ready.", C_PROCEDURE_PARAMETER);\
                            //system("PAUSE");

// None
#define PROMPT_SCANCOMPLETE P_STRING("Scan complete.", C_PROCEDURE_PARAMETER);\
                            //system("PAUSE");

// None
#define PROMPT_EDITREADY    P_STRING("Edit is now ready.", C_PROCEDURE_PARAMETER);\
                            //system("PAUSE");

// None
#define PROMPT_EDITCOMPLETE P_STRING("Edit complete.", C_PROCEDURE_PARAMETER);\
                            //system("PAUSE");

// None
#define PROMPT_MSG(msg)     P_STRING(msg, C_PROCEDURE);\
                            //system("PAUSE");

 // [KR_STR] paramFileName
#define ERROR_FILENOTFOUND  P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                            P_STRING("File with the name of : ", C_PROMPT, false);\
                            P_STRING(paramFileName, C_PROMPT_PARAMETER, false);\
                            P_STRING(" was not found.", C_PROMPT);

// [int] inputSheetIndex
#define ERROR_SHEETNOTFOUND P_STRING("$$ ERROR!!! $$ ", C_ERROR, false);\
                            P_STRING("Sheet with the index of : ", C_PROMPT, false);\
                            P_DOUBLE(inputSheetIndex, C_PROMPT_PARAMETER, false);\
                            P_STRING(" was not found.", C_PROMPT);

// [CellType] cellType
#define LOGIC_PRINTCELLTYPE switch (cellType)\
                            {\
                            case CELLTYPE_EMPTY:\
                                P_STRING("EMPTY!", C_ERROR);\
                                break;\
                            case CELLTYPE_NUMBER:\
                                P_STRING("NUMBER!", C_ERROR);\
                                break;\
                            case CELLTYPE_STRING:\
                                P_STRING("STRING!", C_ERROR);\
                                break;\
                            case CELLTYPE_BOOLEAN:\
                                P_STRING("BOOLEAN!", C_ERROR);\
                                break;\
                            case CELLTYPE_BLANK:\
                                P_STRING("BLANK!", C_ERROR);\
                                break;\
                            case CELLTYPE_ERROR:\
                                P_STRING("ERROR!", C_ERROR);\
                                break;\
                            }

// [Sheet*] XLSXsheet -> trueLastColIndex
#define LOGIC_FINDLASTCOL   int trueLastColIndex = 0;\
                            for (int i = 0; i < XLSXsheet->lastCol(); i++)\
                            {\
                                CellType cellType = XLSXsheet->cellType(0, i);\
                                if (cellType == CELLTYPE_EMPTY)\
                                {\
                                    break;\
                                }\
                                else\
                                {\
                                    trueLastColIndex = i;\
                                }\
                            }

// [Book*] XLSX -> totalSheetCount, inputSheetIndex
#define LOGIC_PROMPTSHEETS  int totalSheetCount = XLSX->sheetCount();\
                            P_STRING("File has a total of : ", C_PROMPT, false);\
                            P_DOUBLE(totalSheetCount, C_PROMPT_PARAMETER, false);\
                            P_STRING(" sheets, each of them having indices and names of...", C_PROMPT);\
                            for (int i = 0; i < totalSheetCount; i++)\
                            {\
                                P_STRING("", C_PROMPT);\
                                P_DOUBLE(i, C_PROMPT_PARAMETER, false);\
                                P_STRING(") <-> ", C_PROMPT, false);\
                                P_STRING(XLSX->getSheetName(i), C_PROMPT_PARAMETER);\
                            }\
                            P_STRING("", C_PROMPT);\
                            P_STRING("Please type in the index of the sheet to be scanned - ", C_PROMPT, false);\
                            int inputSheetIndex = 0;\
                            while (true)\
                            {\
                                std::wstring tempStringBuffer = L"";\
                                std::wcin >> tempStringBuffer;\
                                try\
                                {\
                                    inputSheetIndex = std::stoi(tempStringBuffer);\
                                }\
                                catch (std::invalid_argument msg)\
                                {\
                                    P_STRING(msg.what(), C_ERROR);\
                                    P_STRING("ERROR!!! ", C_ERROR, false);\
                                    P_STRING("Please type in a number between : ", C_PROMPT, false);\
                                    P_DOUBLE(0, C_PROMPT_PARAMETER, false);\
                                    P_STRING(", and ", C_PROMPT, false);\
                                    P_DOUBLE(totalSheetCount - 1, C_PROMPT_PARAMETER, false);\
                                    P_STRING(" - ", C_PROMPT, false);\
                                    continue;\
                                }\
                                if (inputSheetIndex >= 0 && inputSheetIndex < totalSheetCount)\
                                {\
                                    break;\
                                }\
                                else\
                                {\
                                    P_STRING("ERROR!!! ", C_ERROR, false);\
                                    P_STRING("Please type in a number between : ", C_PROMPT, false);\
                                    P_DOUBLE(0, C_PROMPT_PARAMETER, false);\
                                    P_STRING(", and ", C_PROMPT, false);\
                                    P_DOUBLE(totalSheetCount - 1, C_PROMPT_PARAMETER, false);\
                                    P_STRING(" - ", C_PROMPT, false);\
                                }\
                            }