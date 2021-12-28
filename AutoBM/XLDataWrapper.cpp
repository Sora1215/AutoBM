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
#include <iostream>
#include "libxl.h"

using namespace libxl;



XLDataWrapper& XLDataWrapper::Instance() noexcept
{
	static XLDataWrapper instance;
	return instance;
}

void XLDataWrapper::CheckForFormula(KR_STR paramFileName)
{
    PROMPT_FUNCNAME;

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(paramFileName) == true)
    {
        PROMPT_ONFILELOAD;

        // XLSX -> totalSheetCount, inputSheetIndex
        LOGIC_PROMPTSHEETS;

        PROMPT_SCANREADY;

        Sheet* sheet = XLSX->getSheet(inputSheetIndex);

        if (sheet != nullptr)
        {
            P_STRING("Checking for any formulas in the sheet", C_PROMPT);

            // sheet -> trueLastColIndex
            LOGIC_FINDLASTCOL;

            // Go through each cell of the entire sheet
            for (int row = sheet->firstRow(); row < sheet->lastRow(); row++)
            {
                for (int col = sheet->firstCol(); col < trueLastColIndex; col++)
                {
                    if (sheet->isFormula(row, col))
                    {
                        P_POSITION(row, col, C_PROCEDURE, false);
                        P_STRING(sheet->readFormula(row, col), C_ERROR);
                    }
                }
            }
        }
        else
        {
            ERROR_SHEETNOTFOUND;
        }

        PROMPT_SCANCOMPLETE;
        PROMPT_MSG("Above are all formulas found.");
    }
    else
    {
        ERROR_FILENOTFOUND;
    }

    XLSX->release();
    PROMPT_ONFILEUNLOAD;
}

void XLDataWrapper::CheckForZeroWidthSpace(KR_STR paramFileName)
{
    PROMPT_FUNCNAME;

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(paramFileName) == true)
    {
        PROMPT_ONFILELOAD;

        // XLSX -> totalSheetCount, inputSheetIndex
        LOGIC_PROMPTSHEETS;

        PROMPT_SCANREADY;

        Sheet* sheet = XLSX->getSheet(inputSheetIndex);

        if (sheet != nullptr)
        {
            P_STRING("Checking for any Zero Width Spaces in the sheet", C_PROMPT);

            // sheet -> trueLastColIndex
            LOGIC_FINDLASTCOL;

            // Go through each cell of the entire sheet
            for (int row = sheet->firstRow(); row < sheet->lastRow(); row++)
            {
                for (int col = sheet->firstCol(); col < trueLastColIndex; col++)
                {
                    if (sheet->isFormula(row, col))
                    {
                        P_POSITION(row, col, C_PROCEDURE, false);
                        P_STRING(sheet->readFormula(row, col), C_ERROR);
                    }
                }
            }
        }
        else
        {
            ERROR_SHEETNOTFOUND;
        }

        PROMPT_SCANCOMPLETE;
        PROMPT_MSG("Above are all formulas found.");
    }
    else
    {
        ERROR_FILENOTFOUND;
    }

    XLSX->release();
    PROMPT_ONFILEUNLOAD;
}

void XLDataWrapper::CheckForItemLocal(KR_STR paramFileName)
{
    PROMPT_FUNCNAME;

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(paramFileName) == true)
    {
        PROMPT_ONFILELOAD;
        PROMPT_SCANREADY;

        // Load the itemtable first, and here I am 'assuming' those sheets' indices to be correct
        Sheet* ItemTable = XLSX->getSheet(0);
        P_STRING("~ ItemTable sheet 0 ~", C_PROMPT);

        if (ItemTable != nullptr)
        {
            P_STRING("Making sure that all cell contains a number...", C_PROMPT);

            // Go through each row of the entire sheet
            for (int row = ROW_ITEMTABLE_FIRST; row < ItemTable->lastRow(); row++)
            {
                // Make sure that the cell is a number
                CellType cellType = ItemTable->cellType(row, COL_ITEMTABLE_ITEMINDEX);

                if (cellType != CELLTYPE_NUMBER)
                {
                    P_POSITION(row, COL_ITEMTABLE_ITEMINDEX, C_PROCEDURE, false);
                    LOGIC_PRINTCELLTYPE;

                    PROMPT_RETRY;
                    return;
                }
                else
                {
                    mItemTable_ItemIndex_Row[static_cast<int>(ItemTable->readNum(row, COL_ITEMTABLE_ITEMINDEX))] = row;
                }
            }
        }

        PROMPT_SCANCOMPLETE;
        PROMPT_MSG("All cell contains a number.");
        PROMPT_SCANREADY;

        // Now it's time for the LocalTable
        Sheet* LocalTable = XLSX->getSheet(1);
        P_STRING("~ ItemTable sheet 1 ~", C_PROMPT);

        if (LocalTable != nullptr)
        {
            P_STRING("Checking for any non-string data in the sheet...", C_PROMPT);

            // Go through each row of the entire sheet
            for (int row = ROW_ITEMTABLE_FIRST; row < LocalTable->lastRow(); row++)
            {
                // Make sure that the cell is a string
                CellType cellType = LocalTable->cellType(row, COL_LOCALTABLE_ITEMINDEX);

                if (cellType != CELLTYPE_STRING)
                {
                    P_POSITION(row, COL_LOCALTABLE_ITEMINDEX, C_PROCEDURE, false);
                    LOGIC_PRINTCELLTYPE;

                    PROMPT_RETRY;
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

        PROMPT_SCANCOMPLETE;
        PROMPT_MSG("Above are all non-string data found.");

        // Now, wrangle with the data
        PROMPT_MSG("HERE COMES THE REAL DEAL");

        for (auto elm : mItemTable_ItemIndex_Row)
        {
            if (mLocalTable_ItemIndex_Count.find(elm.first) != mLocalTable_ItemIndex_Count.end())
            {
                if (mLocalTable_ItemIndex_Count[elm.first] < 2)
                {
                    P_DOUBLE(elm.first, C_PROMPT_PARAMETER, false);
                    P_STRING(") <-> ", C_PROMPT, false);
                    P_DOUBLE(mLocalTable_ItemIndex_Count[elm.first], C_PROMPT_PARAMETER);
                    P_STRING("Corresponding text count is smaller than 2.", C_ERROR);
                }
                else if (mLocalTable_ItemIndex_Count[elm.first] > 2)
                {
                    P_DOUBLE(elm.first, C_PROMPT_PARAMETER, false);
                    P_STRING(") <-> ", C_PROMPT, false);
                    P_DOUBLE(mLocalTable_ItemIndex_Count[elm.first], C_PROMPT_PARAMETER);
                    P_STRING("Corresponding text count is bigger than 2.", C_ERROR);
                }
            }
            else
            {
                P_DOUBLE(elm.first, C_PROMPT_PARAMETER, false);
                P_STRING(" This ItemIndex does not have any corresponding texts...", C_ERROR);
            }
        }

        PROMPT_SCANCOMPLETE;
    }
    else
    {
        ERROR_FILENOTFOUND;
    }

    XLSX->release();
    PROMPT_ONFILEUNLOAD;
}

template <class T>
T XLDataWrapper::CreateXLSXBook()
{
    Book* XLSX = xlCreateXMLBook(); // xlCreateXMLBook() is a method of LibXL library for loading a file of XLSX format (there's another for XLS)
    XLSX->setKey(L"SeungGeon Kim", L"windows-2f24290302cbeb016bbd6363a0wdlft8"); // Product Key, prefix is there in order to match argument type

    return XLSX;
}