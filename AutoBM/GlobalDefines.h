/*
    Winter 2021
    Automate your work assignment
    Written by SeungGeon Kim

    Filename : GlobalDefines.h
	Edited : 2021-11-27
*/

#pragma once

#define KR_STR const wchar_t*
#define EN_STR const char*

#define REFRESH_RATE INT_MAX

#define EXPORT_FILENAME L"Log_Export.xlsx"
#define IMPORT_FILENAME L"Input.xlsx"

#define JSON_REFERENCECODE "SpriteName"
#define JSON_ORIGINALCODE "OrigName"
#define JSON_NULLCODE "NULL"
#define JSON_ICONHEADER "Icon/Item/"

#define UNICODECHARACTER_ZEROWIDTHSPACE L'\u200b'

//#define R1ASSET_PATH L"r1_asset/98. AssetBundle/ExportProject/Assets/Resources.Export/NguiAtlas/EditorJsonData/"
#define R1ASSET_PATH L"EditorJsonData/"
#define ICON_FILEHEADER L"NguiAtlas.Icon.Item."

// ItemTable
#define COL_ITEMTABLE_ITEMCODE 1
#define COL_ITEMTABLE_ITEMNAME 2
#define COL_ITEMTABLE_ITEMDESC 3
#define ROW_ITEMTABLE_FIRST 2

// LocalTable
#define COL_LOCALTABLE_ITEMCODE 0
#define COL_LOCALTABLE_LOCALTEXT 1
#define ROW_LOCALTABLE_FIRST 1
#define PREFIX_ITEMNAME L"ITEM_NAME_"
#define PREFIX_ITEMDESC L"ITEM_DESC_"
#define PREFIX_ITEMDESC2 L"ITEM_DESC2_"