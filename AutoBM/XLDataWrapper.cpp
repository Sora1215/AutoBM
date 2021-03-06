/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : XLDataWrapper.cpp
	Created : 2021-11-28
*/

#include "XLDataWrapper.h"

#include "ConsoleManager.h"
#include "PromptDefines.h"
#include <unordered_map>
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

void XLDataWrapper::RemoveZeroWidthSpace(KR_STR baseDirectory, std::initializer_list<KR_STR> paramFileExtension) noexcept
{
    PRINT_PROCEDURE;

    // --- Temporary variables used for logging

    std::unordered_map<std::wstring, int> EditedCellPerTable;

    int editCount = 0;

    // --- Temporary containers used for log export

    Book* exportXLSX = CreateXLSXBook<Book*>();

    Sheet* exportXLSXSheet = exportXLSX->addSheet(EXPORT_FILENAME);

    Format* column = exportXLSX->addFormat();
    column->setPatternBackgroundColor(COLOR_WHITE);
    column->setPatternForegroundColor(COLOR_LIGHTORANGE);
    column->setFillPattern(FILLPATTERN_SOLID);
    column->setBorder(BORDERSTYLE_THIN);

    exportXLSXSheet->writeStr(0, 0, L"Before", column);
    exportXLSXSheet->writeStr(0, 1, L"After", column);
    exportXLSXSheet->writeStr(0, 2, L"Table", column);
    exportXLSXSheet->writeStr(0, 3, L"Sheet", column);
    exportXLSXSheet->writeStr(0, 4, L"Row (0-based)", column);
    exportXLSXSheet->writeStr(0, 5, L"Column (0-based)", column);
    exportXLSXSheet->setCol(0, 5, 20);

    Format* before = exportXLSX->addFormat();
    before->setPatternBackgroundColor(COLOR_WHITE);
    before->setPatternForegroundColor(COLOR_ORANGE);
    before->setFillPattern(FILLPATTERN_THINDIAGSTRIPE);

    Format* after = exportXLSX->addFormat();
    after->setPatternBackgroundColor(COLOR_WHITE);
    after->setPatternForegroundColor(COLOR_LIGHTGREEN);
    after->setFillPattern(FILLPATTERN_THINDIAGSTRIPE);

    for (const auto& extension : paramFileExtension)
    {
        RepeatLambdaForAllFilesByExtension(baseDirectory, extension, [&](KR_STR fileName)
            {
                const std::wstring fullPath = std::wstring(baseDirectory) + std::wstring(fileName);
                RepeatLambdaForAllCellsByXLSX(fullPath.c_str(), true, [&](Sheet* XLSXsheet, int row, int col)
                    {
                        const CellType cellType = XLSXsheet->cellType(row, col);

                        if (cellType != CELLTYPE_STRING)
                        {
                            return;
                        }

                        std::wstring tempStringBuffer = XLSXsheet->readStr(row, col);

                        std::size_t firstZeroWidthSpace = tempStringBuffer.find(UNICODECHARACTER_ZEROWIDTHSPACE);

                        if (firstZeroWidthSpace == std::wstring::npos)
                        {
                            return;
                        }

                        PRINT_CELLPOSITION(row, col);
                        P_STRING(tempStringBuffer, C_ERROR, false);

                        exportXLSXSheet->writeStr(editCount + 1, 0, tempStringBuffer.c_str(), before);  // Logging
                        exportXLSXSheet->writeStr(editCount + 1, 2, fileName);                          // Logging
                        exportXLSXSheet->writeStr(editCount + 1, 3, XLSXsheet->name());                 // Logging
                        exportXLSXSheet->writeStr(editCount + 1, 4, std::to_wstring(row).c_str());      // Logging
                        exportXLSXSheet->writeStr(editCount + 1, 5, std::to_wstring(col).c_str());      // Logging

                        while (firstZeroWidthSpace != std::wstring::npos)
                        {
                            tempStringBuffer.erase(firstZeroWidthSpace, 1);
                            firstZeroWidthSpace = tempStringBuffer.find(UNICODECHARACTER_ZEROWIDTHSPACE);
                        }

                        XLSXsheet->writeStr(row, col, tempStringBuffer.c_str());
                        EditedCellPerTable[fullPath] += 1;
                        mEditFlag = true;
                        editCount++;

                        P_STRING(" was fixed as : ", C_PROCEDURE, false);
                        P_STRING(tempStringBuffer, C_ERROR);

                        exportXLSXSheet->writeStr(editCount, 1, tempStringBuffer.c_str(), after);       // Logging
                    });
            });
    }

    if (editCount > 0)
    {
        NEWLINE;

        for (const auto& editLog : EditedCellPerTable)
        {
            P_STRING(editLog.first, C_PROCEDURE_PARAMETER, false);
            P_STRING(" : ", C_PROCEDURE, false);
            P_DOUBLE(editLog.second, C_PROCEDURE_PARAMETER, false);
            P_STRING(" cells were edited.", C_PROCEDURE);
        }

        NEWLINE;
        P_STRING("A total of ", C_PROCEDURE, false);
        P_DOUBLE(editCount, C_PROCEDURE_PARAMETER, false);
        P_STRING(" cells in ", C_PROCEDURE, false);
        P_DOUBLE(EditedCellPerTable.size(), C_PROCEDURE_PARAMETER, false);
        P_STRING(" tables were edited.", C_PROCEDURE);
        NEWLINE;

        exportXLSX->save(EXPORT_FILENAME);

        P_STRING("Diff was exported as : ", C_PRINT, false);
        P_STRING(EXPORT_FILENAME, C_PRINT_PARAMETER);

        NEWLINE;
    }
    else
    {
        PRINT_NOCHANGESMADE;
    }

    exportXLSX->release();
}

std::unordered_map<std::string, std::vector<std::string>> XLDataWrapper::MapItemCodeByXLSX(KR_STR fullPath) noexcept
{
    // --- Initialize map

    std::unordered_map<std::string, std::vector<std::string>> tempItemCodeMap;

    // --- Load XLSX

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(fullPath) == true)
    {
        PRINT_ONFILELOAD(fullPath);
    }
    else
    {
        ERROR_FILENOTFOUND(fullPath);

        P_STRING("A new one will be created, please try again after filling it.", C_PROCEDURE); 

        Book* exportXLSX = CreateXLSXBook<Book*>();

        Sheet* exportXLSXSheet = exportXLSX->addSheet(IMPORT_FILENAME);

        Format* column = exportXLSX->addFormat();
        column->setPatternBackgroundColor(COLOR_WHITE);
        column->setPatternForegroundColor(COLOR_LIGHTORANGE);
        column->setFillPattern(FILLPATTERN_SOLID);
        column->setBorder(BORDERSTYLE_THIN);

        exportXLSXSheet->writeStr(0, 0, L"Reference ItemCode", column);
        exportXLSXSheet->writeStr(0, 1, L"New ItemCode", column);
        exportXLSXSheet->setCol(0, 5, 20);

        exportXLSX->save(IMPORT_FILENAME);

        exportXLSX->release();

        return tempItemCodeMap;
    }

    // --- Load sheet

    Sheet* XLSXsheet = XLSX->getSheet(0);

    if (XLSXsheet != nullptr)
    {
        P_STRING("Current sheet : ", C_PRINT, false);
        P_STRING(XLSX->getSheetName(0), C_PRINT_PARAMETER);
        PRINT_SCANNING;
    }
    else
    {
        ERROR_SHEETNOTFOUND(0);
        return tempItemCodeMap;
    }

    // --- Read sheet

    const int lastRow = XLSXsheet->lastFilledRow();

    for (int row = 1; row < lastRow; row++)
    {
        bool skipLineFlag = false;

        for (int col = 0; col < 2; col++)
        {
            const CellType cellType = XLSXsheet->cellType(row, col);

            if (cellType != CELLTYPE_STRING)
            {
                PRINT_CELLPOSITION(row, col);
                ERROR_BADCELLTYPE;
                skipLineFlag = true;
            }
        }

        if (skipLineFlag == true)
        {
            continue;
        }
        
        const std::string referenceCode = CONSOLE.ConvertWstringToString(std::wstring(XLSXsheet->readStr(row, 0)));
        const std::string newCode = CONSOLE.ConvertWstringToString(std::wstring(XLSXsheet->readStr(row, 1)));

        tempItemCodeMap[referenceCode].emplace_back(newCode);
    }

    PRINT_SCANCOMPLETE;

    // --- Unload XLSX

    XLSX->release();
    PRINT_ONFILEUNLOAD(fullPath);

    // --- Print reference pairs

    for (const auto& referenceString : tempItemCodeMap)
    {
        for (const auto& newString : tempItemCodeMap[referenceString.first])
        {
            P_STRING(referenceString.first, C_ERROR, false);
            P_STRING(" will be referenced by : ", C_PROCEDURE, false);
            P_STRING(newString, C_ERROR);
        }
    }

    // --- Return result

    return tempItemCodeMap;
}

template<class T>
void XLDataWrapper::RepeatLambdaForAllCellsByXLSX(KR_STR fullPath, bool isEdit, T&& lambda) noexcept
{
    // --- load XLSX

    Book* XLSX = CreateXLSXBook<Book*>();

    if (XLSX->load(fullPath) == true)
    {
        PRINT_ONFILELOAD(fullPath);
    }
    else
    {
        ERROR_FILENOTFOUND(fullPath);
        return;
    }

    // --- Load sheets

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
            ERROR_SHEETNOTFOUND(inputSheetIndex);
            return;
        }

        const int lastRow = XLSXsheet->lastFilledRow();
        const int lastCol = XLSXsheet->lastFilledCol();

        for (int row = XLSXsheet->firstRow(); row < lastRow; row++)
        {
            for (int col = XLSXsheet->firstCol(); col < lastCol; col++)
            {
                lambda(XLSXsheet, row, col);
            }
        }

        PRINT_SCANCOMPLETE;
    }

    // --- Save on edit

    if (isEdit == true && mEditFlag == true)
    {
        PRINT_SAVING;
        XLSX->save(fullPath);
        mEditFlag = false;
        PRINT_SAVECOMPLETE;
    }

    // --- Unload XLSX

    XLSX->release();
    PRINT_ONFILEUNLOAD(fullPath);
}

template<class T>
void XLDataWrapper::RepeatLambdaForAllFilesByExtension(KR_STR baseDirectory, KR_STR paramFileExtension, T&& lambda) noexcept
{
    if (std::filesystem::exists(baseDirectory))
    {
        PRINT_PATHFOUND(baseDirectory);
    }
    else
    {
        ERROR_PATHNOTFOUND(baseDirectory);
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
}

template<class T>
void XLDataWrapper::RepeatLambdaForAllFilesByExtension_Recursive(KR_STR baseDirectory, KR_STR paramFileExtension, T&& lambda) noexcept
{
    if (std::filesystem::exists(baseDirectory))
    {
        PRINT_PATHFOUND(baseDirectory);
    }
    else
    {
        ERROR_PATHNOTFOUND(baseDirectory);
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
}

template <class T>
T XLDataWrapper::CreateXLSXBook() noexcept
{
    Book* XLSX = xlCreateXMLBook(); // xlCreateXMLBook() is a method of LibXL library for loading a file of XLSX (XLSM) format (there's another for XLS)
    XLSX->setKey(L"SeungGeon Kim", L"windows-2f24290302cbeb016bbd6363a0wdlft8"); // Product Key, prefix is there in order to match argument type (wchar)

    return XLSX;
}

template<class T>
void XLDataWrapper::PrintCellType(T) noexcept
{
    switch (T)
    {
    case CELLTYPE_EMPTY:
        P_STRING("EMPTY!", C_ERROR); 
        break; 
    case CELLTYPE_NUMBER:
        P_STRING("NUMBER!", C_ERROR); 
        break; 
    case CELLTYPE_STRING:
        P_STRING("STRING!", C_ERROR); 
        break; 
    case CELLTYPE_BOOLEAN:
        P_STRING("BOOLEAN!", C_ERROR); 
        break; 
    case CELLTYPE_BLANK:
        P_STRING("BLANK!", C_ERROR); 
        break; 
    case CELLTYPE_ERROR:
        P_STRING("ERROR!", C_ERROR); 
        break; 
    }
}

template<class T>
int XLDataWrapper::PromptSheets(T XLSX) noexcept
{
    const int totalSheetCount = XLSX->sheetCount();

    P_STRING("File has a total of : ", C_PRINT, false);
    P_DOUBLE(totalSheetCount, C_PRINT_PARAMETER, false);
    P_STRING(" sheets, each of them having indices and names of...", C_PRINT);

    for (int i = 0; i < totalSheetCount; i++)
    {
        NEWLINE;
        P_DOUBLE(i, C_PRINT_PARAMETER, false);
        P_STRING(") -> ", C_PRINT, false);
        P_STRING(XLSX->getSheetName(i), C_PRINT_PARAMETER);
    }

    NEWLINE;
    P_STRING("Please type in the index of the sheet to be scanned - ", C_PRINT, false);

    int inputSheetIndex = 0;

    while (true)
    {
        std::wstring tempStringBuffer = L"";
        std::wcin >> tempStringBuffer;

        try
        {
            inputSheetIndex = std::stoi(tempStringBuffer);
        }
        catch (const std::exception& msg)
        {
            P_STRING(msg.what(), C_ERROR);
            ERROR_OUTOFRANGE(0, totalSheetCount - 1);
            continue;
        }

        if (inputSheetIndex >= 0 && inputSheetIndex < totalSheetCount)
        {
            return inputSheetIndex;
        }
        else
        {
            ERROR_OUTOFRANGE(0, totalSheetCount - 1);
        }
    }
}