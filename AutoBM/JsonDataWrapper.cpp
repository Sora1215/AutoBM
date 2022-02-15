/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : JsonDataWrapper.cpp
    Created : 2021-11-28
*/

#include "JsonDataWrapper.h"

#include "ConsoleManager.h"
#include "XLDataWrapper.h"
#include "PromptDefines.h"
#include <filesystem>
#include "json.hpp"
#include <fstream>

using json = nlohmann::ordered_json;



JsonDataWrapper& JsonDataWrapper::Instance() noexcept
{
    static JsonDataWrapper instance;
    return instance;
}

void JsonDataWrapper::ConnectItemIcon() noexcept
{
    // --- Print which function are we currently at

    PRINT_PROCEDURE;

    // --- Load map, end procedure if empty

    std::unordered_map<std::string, std::vector<std::string>> itemCodeMap = std::move(XL_DW.MapItemCodeByXLSX(IMPORT_FILENAME));

    if (itemCodeMap.empty() == true)
    {
        NEWLINE;
        P_STRING("An empty container was returned. Aborting procedure.", C_PROCEDURE);
        NEWLINE;
        PRINT_NOCHANGESMADE;
        return;
    }

    // --- Check if path exists 

    if (std::filesystem::exists(R1ASSET_PATH))
    {
        PRINT_PATHFOUND(R1ASSET_PATH);
    }
    else
    {
        ERROR_PATHNOTFOUND(R1ASSET_PATH);
        PRINT_NOCHANGESMADE;
        return;
    }

    // --- Temporary variables for flagging

    int totalEditCount = 0;
    int cycleEditCount = 0;
    std::vector<std::string> locatedRefKeys;

    // --- OK, we have the parameter and know that the directory is intact, now iterate through icon directory

    for (const auto& directoryIterator : std::filesystem::directory_iterator(R1ASSET_PATH))
    {
        const std::wstring fileName = std::wstring(directoryIterator.path().filename());
        const std::wstring fileExtension = std::wstring(directoryIterator.path().extension());

        // --- "return" if file header does not match or extension is not json

        if (fileName.find(ICON_FILEHEADER) == std::wstring::npos || fileExtension != L".json")
        {
            continue;
        }

        // --- Try opening the file and handle exceptions

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

        // --- Try parsing the file and handle exceptions

        json editorJson;

        try
        {
            editorJson = json::parse(fileInput);
        }
        catch (const std::exception& msg)
        {
            P_STRING(msg.what(), C_ERROR);
            ERROR_JSONPARSEFAIL(fullPath);
            continue;
        }

        PRINT_JSONPARSESUCCESS(fullPath);

        fileInput.close();
        PRINT_ONFILEUNLOAD(fullPath);

        // --- The file is what we want, and it is parsable

        for (const auto& referenceCode : itemCodeMap)
        {
            // --- Check if key was already located

            if (std::find(locatedRefKeys.begin(), locatedRefKeys.end(), referenceCode.first) != locatedRefKeys.end())
            {
                continue;
            }

            try
            {
                /* I am doing this because this specific function's return type is limited to references */
                const json::array_t& connections = editorJson.at("Atlas").at("ReferenceSpriteList");

                for (const auto& jsonObject : connections)
                {
                    std::string tempStringBuffer = jsonObject.value(JSON_REFERENCECODE, JSON_NULLCODE);

                    const std::string originalCodeWithHeader = jsonObject.value(JSON_ORIGINALCODE, JSON_NULLCODE);

                    // --- Check if original code is null

                    if (originalCodeWithHeader == JSON_NULLCODE)
                    {
                        continue;
                    }

                    // --- Check if reference code is null

                    if (tempStringBuffer == JSON_NULLCODE)
                    {
                        continue;
                    }

                    // --- Check if string header is in the right format. If so, remove the header.

                    if (tempStringBuffer.find(JSON_ICONHEADER) != std::string::npos)
                    {
                        tempStringBuffer.erase(0, std::string(JSON_ICONHEADER).length());
                    }
                    else
                    {
                        continue;
                    }

                    //P_STRING(tempStringBuffer, C_PRINT_PARAMETER); ~ Uncomment this line for future debug ~

                    // --- Check if the reference code does not match

                    if (tempStringBuffer == referenceCode.first)
                    {
                        PRINT_JSONREFKEYFOUND(referenceCode.first);
                    }
                    else
                    {
                        // --- Check if the original code does not match

                        if (originalCodeWithHeader == referenceCode.first)
                        {
                            PRINT_JSONORIGINKEYFOUND(referenceCode.first);
                        }
                        else 
                        {
                            continue;
                        }
                    }

                    // --- Status all green, we are ready to go

                    for (const auto& newCode : itemCodeMap[referenceCode.first])
                    {
                        const std::string newCodeWithHeader = std::string(JSON_ICONHEADER) + newCode;

                        const json jsonReferenceCode = { JSON_REFERENCECODE, newCodeWithHeader };
                        const json jsonNewCode = { JSON_ORIGINALCODE, originalCodeWithHeader };
                        const json jsonExportObject = { jsonReferenceCode, jsonNewCode };

                        P_STRING("Added : ", C_PRINT_PARAMETER, false); 
                        P_STRING(jsonExportObject.dump(), C_PROCEDURE);

                        editorJson.at("Atlas").at("ReferenceSpriteList").emplace_back(jsonExportObject);

                        totalEditCount++;
                        cycleEditCount++;
                    }

                    // --- Save the processed reference key for future checking

                    locatedRefKeys.emplace_back(referenceCode.first);
                    break;
                }
            }
            catch (const std::exception& msg)
            {
                P_STRING(msg.what(), C_ERROR);
                ERROR_JSONREFLISTNOTFOUND;
                break;
            }
        }

        // --- If not edited, skip the file. Else, save it

        if (cycleEditCount == 0)
        {
            continue;
        }

        std::ofstream fileOutput(fullPath, std::ios_base::binary);

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
        fileOutput << std::setw(4) << std::setfill(' ') << editorJson;
        PRINT_SAVECOMPLETE;

        fileOutput.close();
        PRINT_ONFILEUNLOAD(fullPath);

        cycleEditCount = 0;
    }

    // --- Print result

    NEWLINE;

    for (const auto& itemCodePair : itemCodeMap)
    {
        if (std::find(locatedRefKeys.begin(), locatedRefKeys.end(), itemCodePair.first) == locatedRefKeys.end())
        {
            P_STRING("Reference key of : ", C_ERROR, false);
            P_STRING(itemCodePair.first, C_PRINT_PARAMETER, false);
            P_STRING(" was not found.", C_ERROR);
        }
        else 
        {
            P_STRING("Reference key of : ", C_PROCEDURE, false);
            P_STRING(itemCodePair.first.c_str(), C_PROCEDURE_PARAMETER, false);
            P_STRING(" was found.", C_PROCEDURE);
        }
    }

    NEWLINE;

    if (totalEditCount > 0)
    {
        P_STRING("A total of ", C_PROCEDURE, false);
        P_DOUBLE(totalEditCount, C_PROCEDURE_PARAMETER, false);
        P_STRING(" reference pairs were added.", C_PROCEDURE);
    }

    NEWLINE;
}