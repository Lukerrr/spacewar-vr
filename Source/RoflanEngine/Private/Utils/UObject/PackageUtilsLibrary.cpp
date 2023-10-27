// Copyright (C) 2022 GB ltd. All rights reserved.

#include "Utils/UObject/PackageUtilsLibrary.h"
#include "UObject/UObjectGlobals.h"

float UPackageUtilsLibrary::Package_GetAsyncLoadPercentage(const FName& PackageName)
{
	return GetAsyncLoadPercentage(PackageName);
}

float UPackageUtilsLibrary::Package_GetLevelAsyncLoadPercentage(const TSoftObjectPtr<UWorld> Level)
{
	if (!Level.IsValid())
	{
		return -1.f;
	}

	FString LevelName;
	LevelName = Level->GetPackage()->GetName();

	return Package_GetAsyncLoadPercentage(FName(LevelName));
}
