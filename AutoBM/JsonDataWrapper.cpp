/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : JsonDataWrapper.cpp
    Edited : 2021-11-29
*/

#include "JsonDataWrapper.h"

#include "ConsoleManager.h"
#include "XLDataWrapper.h"
#include "PromptDefines.h"
#include <unordered_map>
#include <filesystem>
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;



JsonDataWrapper& JsonDataWrapper::Instance() noexcept
{
    static JsonDataWrapper instance;
    return instance;
}

void JsonDataWrapper::ConnectItemIcon() noexcept
{
    PRINT_PROCEDURE;

    std::unordered_map<std::string, std::list<std::string>> itemCodeMap = std::move(XL_DW.MapItemCodeByXLSX(IMPORT_FILENAME));

    if (itemCodeMap.empty() == true)
    {
        NEWLINE;
        P_STRING("An empty container was returned. Aborting procedure.", C_PROCEDURE);
        NEWLINE;

        return;
    }

    // ---

    if (std::filesystem::exists(R1ASSET_PATH))
    {
        PRINT_PATHFOUND(R1ASSET_PATH);
    }
    else
    {
        PRINT_PATHNOTFOUND(R1ASSET_PATH);
        return;
    }

    // ---

    for (const auto& directoryIterator : std::filesystem::directory_iterator(R1ASSET_PATH))
    {
        const std::wstring fileName = std::wstring(directoryIterator.path().filename());
        const std::wstring fileExtension = std::wstring(directoryIterator.path().extension());

        if (fileName.find(ICON_FILEHEADER) == std::wstring::npos || fileExtension != L".json")
        {
            continue;
        }

        // ---

        const std::wstring fullPath = std::wstring(R1ASSET_PATH) + fileName;

        std::ifstream fileInput(fullPath);

        if (fileInput.fail() != true)
        {
            PRINT_ONFILELOAD(fullPath);
        }
        else
        {
            ERROR_FILENOTFOUND(fullPath);
            continue;
        }

        // ---

        json editorJson;

        try
        {
            editorJson = json::parse(fileInput);
        }
        catch (const json::parse_error& msg)
        {
            P_STRING(msg.what(), C_ERROR);
            ERROR_JSONPARSEFAIL(fullPath);
            continue;
        }

        // Print WIPWIP

        for (const auto& referenceCode : itemCodeMap)
        {
            if (editorJson.contains(referenceCode.first) != true)
            {
                continue;
            }

            for (const auto& newCode : itemCodeMap[referenceCode.first])
            {
                try
                {
                    json::array_t connections = editorJson.at("Atlas").at("ReferenceSpriteList");
                }
                catch (const json::basic_json::out_of_range& msg)
                {
                    P_STRING(msg.what(), C_ERROR);
                    ERROR_JSONFINDFAIL;
                    continue;
                }

                // Found them WIPWIP

            }
        }
    }
}