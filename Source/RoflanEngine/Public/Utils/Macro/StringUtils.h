// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

// Identifier to static char string
#define STRINGIFY(x) #x

// UEnum value to FString
#define ENUM_TO_STRING(EnumType, EnumValue) \
	FindObject<UEnum>(ANY_PACKAGE, TEXT(STRINGIFY(EnumType)), true)->GetNameStringByIndex(static_cast<uint8>(EnumValue))

// FString to UEnum value
#define STRING_TO_ENUM(EnumType, EnumValueStr) \
	EnumType(static_cast<uint8>(FindObject<UEnum>(ANY_PACKAGE, TEXT(STRINGIFY(EnumType)), true)->GetIndexByName(FName(EnumValueStr))))

// Boolean value to FString
#define BOOL_TO_STRING(b) ((b == 0) ? FString("true") : FString("false"))
