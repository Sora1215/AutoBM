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

#include <iostream> // TEST

using json = nlohmann::ordered_json;



JsonDataWrapper& JsonDataWrapper::Instance() noexcept
{
    static JsonDataWrapper instance;
    return instance;
}

void JsonDataWrapper::ConnectItemIcon() noexcept
{
    PRINT_PROCEDURE;

    // ---

    std::unordered_map<std::string, std::list<std::string>> itemCodeMap = std::move(XL_DW.MapItemCodeByXLSX(IMPORT_FILENAME));

    if (itemCodeMap.empty() == true)
    {
        NEWLINE;
        P_STRING("An empty container was returned. Aborting procedure.", C_PROCEDURE);
        NEWLINE;

        WAITFORINPUT;
        return;
    }

    // ---

    if (std::filesystem::exists(R1ASSET_PATH))
    {
        PRINT_PATHFOUND(R1ASSET_PATH);
    }
    else
    {
        ERROR_PATHNOTFOUND(R1ASSET_PATH);

        WAITFORINPUT;
        return;
    }

    // ---

    for (const auto& directoryIterator : std::filesystem::directory_iterator(R1ASSET_PATH))
    {
        const std::wstring fileName = std::wstring(directoryIterator.path().filename());
        const std::wstring fileExtension = std::wstring(directoryIterator.path().extension());

        if (fileName.find(ICON_FILEHEADER) == std::wstring::npos || fileExtension != L".json")
        {
            // File extension is not .json
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

        PRINT_JSONPARSESUCCESS(fullPath);

        fileInput.close();
        PRINT_ONFILEUNLOAD(fullPath);

        // ---

        for (const auto& referenceCode : itemCodeMap)
        {
            try
            {
                // I am doing this because this specific function's return type is limited to references
                const json::array_t& connections = editorJson.at("Atlas").at("ReferenceSpriteList");

                for (const auto& jsonObject : connections)
                {
                    std::string tempStringBuffer = jsonObject.value(JSON_REFERENCECODE, JSON_NULLCODE);

                    if (tempStringBuffer == JSON_NULLCODE)
                    {
                        // Reference code is null
                        continue;
                    }

                    if (tempStringBuffer.find(JSON_ICONHEADER) != std::string::npos)
                    {
                        tempStringBuffer.erase(0, std::string(JSON_ICONHEADER).length());
                    }
                    else
                    {
                        // String header is not in format of item/icon
                        continue;
                    }

                    // A version of the reference code having its header removed
                    //P_STRING(tempStringBuffer, C_PRINT_PARAMETER); ~ Uncomment this line for future debug ~

                    if (tempStringBuffer == referenceCode.first)
                    {
                        PRINT_JSONREFKEYFOUND(referenceCode.first);
                    }
                    else
                    {
                        // Reference code mismatch
                        continue;
                    }

                    const std::string originalCodeWithHeader = jsonObject.value(JSON_ORIGINALCODE, JSON_NULLCODE);

                    if (originalCodeWithHeader == JSON_NULLCODE)
                    {
                        // Original code is null
                        continue;
                    }

                    for (const auto& newCode : itemCodeMap[referenceCode.first])
                    {
                        const std::string newCodeWithHeader = std::string(JSON_ICONHEADER) + newCode;

                        const json jsonReferenceCode = { JSON_REFERENCECODE, newCodeWithHeader };
                        const json jsonNewCode = { JSON_ORIGINALCODE, originalCodeWithHeader };
                        const json jsonExportObject = { jsonReferenceCode, jsonNewCode };

                        P_STRING("Added : ", C_PRINT_PARAMETER, false); 
                        P_STRING(jsonExportObject.dump(), C_PROCEDURE);

                        editorJson.at("Atlas").at("ReferenceSpriteList").emplace_back(jsonExportObject);
                    }
                }
            }
            catch (const json::out_of_range& msg)
            {
                P_STRING(msg.what(), C_ERROR);
                ERROR_JSONREFLISTNOTFOUND;
                break;
            }
        }

        // ---

        std::ofstream fileOutput(fullPath);

        if (fileOutput.fail() != true)
        {
            PRINT_ONFILELOAD(fullPath);
        }
        else
        {
            ERROR_FILENOTFOUND(fullPath);
            continue;
        }

        PRINT_SAVING;
        fileOutput << std::setw(1) << std::setfill('\t') << editorJson;
        PRINT_SAVECOMPLETE;

        fileOutput.close();
        PRINT_ONFILEUNLOAD(fullPath);

    }

    // ---

    WAITFORINPUT;
}