/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : XLDataWrapper.cpp
	Edited : 2021-11-29
*/

#include "XLDataWrapper.h"

#include "ConsoleManager.h"
#include "PromptDefines.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include "libxl.h"

using namespace libxl;



XLDataWrapper& XLDataWrapper::Instance() noexcept
{
	static XLDataWrapper instance;
	return instance;
}

void XLDataWrapper::RemoveZeroWidthSpace(KR_STR baseDirectory, KR_STR paramFileExtension) noexcept
{
    RepeatLambdaForAllFilesByExtension(baseDirectory, paramFileExtension, [&](KR_STR fileName)
        {
            const std::wstring fullPath = std::wstring(baseDirectory) + std::wstring(fileName);
            RepeatLambdaForAllCellsByTable(fullPath.c_str(), true, [&](Sheet* XLSXsheet, int row, int col)
                {
                    CellType cellType = XLSXsheet->cellType(row, col);

                    if (cellType != CELLTYPE_STRING)
                    {
                        return;
                    }

                    std::wstring tempStringBuffer = XLSXsheet->readStr(row, col);

                    std::size_t firstZeroWidthSpace = tempStringBuffer.find(L'\u200b');

                    if (firstZeroWidthSpace == std::wstring::npos)
                    {
                        return;
                    }

                    P_POSITION(row, col, C_PROCEDURE, false);
                    P_STRING(tempStringBuffer, C_ERROR, false);

                    while (firstZeroWidthSpace != std::wstring::npos)
                    {
                        tempStringBuffer.erase(firstZeroWidthSpace);
                        firstZeroWidthSpace = tempStringBuffer.find(L'\u200b');
                    }

                    XLSXsheet->writeStr(row, col, tempStringBuffer.c_str());
                    mEditFlag = true;

                    P_STRING(" was fixed as : ", C_PROCEDURE, false);
                    P_STRING(tempStringBuffer, C_ERROR);
                });
        });
}

template <class T>
T XLDataWrapper::CreateXLSXBook() noexcept
{
    Book* XLSX = xlCreateXMLBook(); // xlCreateXMLBook() is a method of LibXL library for loading a file of XLSX (XLSM) format (there's another for XLS)
    XLSX->setKey(L"SeungGeon Kim", L"windows-2f24290302cbeb016bbd6363a0wdlft8"); // Product Key, prefix is there in order to match argument type (wchar)

    return XLSX;
}

template<class T>
void XLDataWrapper::RepeatLambdaForAllCellsByTable(KR_STR paramFileName, bool isEdit, T lambda) noexcept
{
    PRINT_PROCEDURE;



    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(paramFileName) == true)
    {
        PRINT_ONFILELOAD;
    }
    else
    {
        ERROR_FILENOTFOUND;
        return;
    }



    const int totalSheetCount = XLSX->sheetCount();

    for (int inputSheetIndex = 0; inputSheetIndex < totalSheetCount; inputSheetIndex++)
    {
        Sheet* XLSXsheet = XLSX->getSheet(inputSheetIndex);

        if (XLSXsheet != nullptr)
        {
            P_STRING("Current sheet : ", C_PRINT, false);
            P_STRING(XLSX->getSheetName(inputSheetIndex), C_PRINT_PARAMETER);
            PRINT_SCANNING;
        }
        else
        {
            ERROR_SHEETNOTFOUND;
            return;
        }



        // XLSXsheet -> trueLastColIndex
        LOGIC_FINDLASTCOL;

        // Go through each cell of the entire sheet
        for (int row = XLSXsheet->firstRow(); row < XLSXsheet->lastRow(); row++)
        {
            for (int col = XLSXsheet->firstCol(); col < trueLastColIndex; col++)
            {
                lambda(XLSXsheet, row, col);
            }
        }



        PRINT_SCANCOMPLETE;
    }



    if (isEdit == true && mEditFlag == true)
    {
        PRINT_SAVING;
        XLSX->save(paramFileName);
        mEditFlag = false;
        PRINT_SAVECOMPLETE;
    }



    XLSX->release();
    PRINT_ONFILEUNLOAD;
}

template<class T>
void XLDataWrapper::RepeatLambdaForAllFilesByExtension(KR_STR baseDirectory, KR_STR paramFileExtension, T lambda) noexcept
{
    if (std::filesystem::exists(baseDirectory))
    {
        PRINT_PATHFOUND;
    }
    else
    {
        PRINT_PATHNOTFOUND;
        return;
    }

    for (const auto& directoryIterator : std::filesystem::directory_iterator(baseDirectory))
    {
        const std::wstring fileName = std::wstring(directoryIterator.path().filename());
        const std::wstring fileExtension = std::wstring(directoryIterator.path().extension());

        if (fileExtension == paramFileExtension)
        {
            lambda(fileName.c_str());
        }
    }

    WAITFORINPUT;
}

template<class T>
void XLDataWrapper::RepeatLambdaForAllFilesByExtension_Recursive(KR_STR baseDirectory, KR_STR paramFileExtension, T lambda) noexcept
{
    if (std::filesystem::exists(baseDirectory))
    {
        PRINT_PATHFOUND;
    }
    else
    {
        PRINT_PATHNOTFOUND;
        return;
    }

    for (const auto& directoryIterator : std::filesystem::recursive_directory_iterator(baseDirectory))
    {
        const std::wstring fileName = std::wstring(directoryIterator.path().filename());
        const std::wstring fileExtension = std::wstring(directoryIterator.path().extension());

        if (fileExtension == paramFileExtension)
        {
            lambda(fileName.c_str());
        }
    }

    WAITFORINPUT;
}

// --- Below are messy codes those are to be refactored ---

/*

void XLDataWrapper::CheckForFormula(KR_STR paramFileName) noexcept
{
    PRINT_PROCEDURE;

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(paramFileName) == true)
    {
        PRINT_ONFILELOAD;

        // [Book*] XLSX -> totalSheetCount, inputSheetIndex
        LOGIC_PRINTSHEETS;

        PRINT_SCANNING;

        Sheet* XLSXsheet = XLSX->getSheet(inputSheetIndex);

        if (XLSXsheet != nullptr)
        {
            P_STRING("Checking for any formulas in the specified sheet", C_PRINT);

            // [Sheet*] XLSXsheet -> trueLastColIndex
            LOGIC_FINDLASTCOL;

            // Go through each cell of the entire sheet
            for (int row = XLSXsheet->firstRow(); row < XLSXsheet->lastRow(); row++)
            {
                for (int col = XLSXsheet->firstCol(); col < trueLastColIndex; col++)
                {
                    if (XLSXsheet->isFormula(row, col))
                    {
                        P_POSITION(row, col, C_PROCEDURE, false);
                        P_STRING(XLSXsheet->readFormula(row, col), C_ERROR);
                    }
                }
            }
        }
        else
        {
            ERROR_SHEETNOTFOUND;
        }

        PRINT_SCANCOMPLETE;
        PRINT_MSG("Above are all the formulas found.");
    }
    else
    {
        ERROR_FILENOTFOUND;
    }

    XLSX->release();
    PRINT_ONFILEUNLOAD;
}

void XLDataWrapper::CheckForItemLocal(KR_STR paramFileName) noexcept
{
    PRINT_PROCEDURE;

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(paramFileName) == true)
    {
        PRINT_ONFILELOAD;
        PRINT_SCANNING;

        // Load the itemtable first, and here I am 'assuming' those sheets' indices to be correct
        Sheet* ItemTable = XLSX->getSheet(0);
        P_STRING("~ ItemTable XLSXsheet 0 ~", C_PRINT);

        if (ItemTable != nullptr)
        {
            P_STRING("Making sure that all cell contains a number...", C_PRINT);

            // Go through each row of the entire XLSXsheet
            for (int row = ROW_ITEMTABLE_FIRST; row < ItemTable->lastRow(); row++)
            {
                CellType cellType = ItemTable->cellType(row, COL_ITEMTABLE_ITEMINDEX);

                if (cellType != CELLTYPE_NUMBER)
                {
                    P_POSITION(row, COL_ITEMTABLE_ITEMINDEX, C_PROCEDURE, false);
                    LOGIC_PRINTCELLTYPE;

                    PRINT_RETRY;
                    return;
                }
                else
                {
                    mItemTable_ItemIndex_Row[static_cast<int>(ItemTable->readNum(row, COL_ITEMTABLE_ITEMINDEX))] = row;
                }
            }
        }

        PRINT_SCANCOMPLETE;
        PRINT_MSG("All cell contains a number.");
        PRINT_SCANNING;

        // Now it's time for the LocalTable
        Sheet* LocalTable = XLSX->getSheet(1);
        P_STRING("~ ItemTable XLSXsheet 1 ~", C_PRINT);

        if (LocalTable != nullptr)
        {
            P_STRING("Checking for any non-string data in the XLSXsheet...", C_PRINT);

            // Go through each row of the entire XLSXsheet
            for (int row = ROW_ITEMTABLE_FIRST; row < LocalTable->lastRow(); row++)
            {
                CellType cellType = LocalTable->cellType(row, COL_LOCALTABLE_ITEMINDEX);

                if (cellType != CELLTYPE_STRING)
                {
                    P_POSITION(row, COL_LOCALTABLE_ITEMINDEX, C_PROCEDURE, false);
                    LOGIC_PRINTCELLTYPE;

                    PRINT_RETRY;
                    return;
                }
                else
                {
                    std::wstring tempStringBuffer = LocalTable->readStr(row, COL_LOCALTABLE_ITEMINDEX);

                    int itemIndex = 0;

                    try
                    {
                        if (tempStringBuffer.find(PREFIX_ITEMNAME) != std::wstring::npos)
                        {
                            tempStringBuffer.erase(0, std::wstring(PREFIX_ITEMNAME).length());
                            itemIndex = std::stoi(tempStringBuffer);
                            mLocalTable_ItemIndex_Name[itemIndex] = LocalTable->readStr(row, COL_LOCALTABLE_TEXTINDEX);
                            mLocalTable_ItemIndex_Count[itemIndex] += 1;
                        }
                        else if (tempStringBuffer.find(PREFIX_ITEMDESC) != std::wstring::npos)
                        {
                            tempStringBuffer.erase(0, std::wstring(PREFIX_ITEMDESC).length());
                            itemIndex = std::stoi(tempStringBuffer);
                            mLocalTable_ItemIndex_Desc[itemIndex] = LocalTable->readStr(row, COL_LOCALTABLE_TEXTINDEX);
                            mLocalTable_ItemIndex_Count[itemIndex] += 1;
                        }
                        else if (tempStringBuffer.find(PREFIX_ITEMDESC2) != std::wstring::npos)
                        {
                            tempStringBuffer.erase(0, std::wstring(PREFIX_ITEMDESC2).length());
                            itemIndex = std::stoi(tempStringBuffer);
                            mLocalTable_ItemIndex_Desc[itemIndex] = LocalTable->readStr(row, COL_LOCALTABLE_TEXTINDEX);
                            mLocalTable_ItemIndex_Count[itemIndex] += 1;
                        }
                    }
                    catch (std::invalid_argument msg)
                    {
                        P_STRING(msg.what(), C_ERROR);
                        P_POSITION(row, COL_LOCALTABLE_ITEMINDEX, C_PROCEDURE, false);
                        P_STRING(LocalTable->readStr(row, COL_LOCALTABLE_ITEMINDEX), C_PROCEDURE, true);
                    }
                }
            }
        }

        PRINT_SCANCOMPLETE;
        PRINT_MSG("Above are all non-string data found.");

        // Now, wrangle with the data
        PRINT_MSG("HERE COMES THE REAL DEAL");

        for (auto elm : mItemTable_ItemIndex_Row)
        {
            if (mLocalTable_ItemIndex_Count.find(elm.first) != mLocalTable_ItemIndex_Count.end())
            {
                if (mLocalTable_ItemIndex_Count[elm.first] < 2)
                {
                    P_DOUBLE(elm.first, C_PRINT_PARAMETER, false);
                    P_STRING(") <-> ", C_PRINT, false);
                    P_DOUBLE(mLocalTable_ItemIndex_Count[elm.first], C_PRINT_PARAMETER);
                    P_STRING("Corresponding text count is smaller than 2.", C_ERROR);
                }
                else if (mLocalTable_ItemIndex_Count[elm.first] > 2)
                {
                    P_DOUBLE(elm.first, C_PRINT_PARAMETER, false);
                    P_STRING(") <-> ", C_PRINT, false);
                    P_DOUBLE(mLocalTable_ItemIndex_Count[elm.first], C_PRINT_PARAMETER);
                    P_STRING("Corresponding text count is bigger than 2.", C_ERROR);
                }
            }
            else
            {
                P_DOUBLE(elm.first, C_PRINT_PARAMETER, false);
                P_STRING(" This ItemIndex does not have any corresponding texts...", C_ERROR);
            }
        }

        PRINT_SCANCOMPLETE;
    }
    else
    {
        ERROR_FILENOTFOUND;
    }

    XLSX->release();
    PRINT_ONFILEUNLOAD;
}

*/