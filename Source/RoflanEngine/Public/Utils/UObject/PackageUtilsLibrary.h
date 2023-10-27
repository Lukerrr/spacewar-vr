// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PackageUtilsLibrary.generated.h"

/**
 * The library that exposes some useful in-engine functions to blueprints
 */
UCLASS()
class ROFLANENGINE_API UPackageUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * THIS IS SLOW. MAY BLOCK ASYNC LOADING.
	 * Returns the async load percentage for a package in flight with the passed in name or -1 if there isn't one.
	 * 
	 * @param PackageName - Name of package to query load percentage for
	 * @return Async load percentage if package is currently being loaded, -1 otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Package Utils")
	static float Package_GetAsyncLoadPercentage(const FName& PackageName);

	/**
	 * THIS IS SLOW. MAY BLOCK ASYNC LOADING.
	 * Returns the async load percentage for a package in flight with the passed in name or -1 if there isn't one.
	 *
	 * @param PackageName - Name of package to query load percentage for
	 * @return Async load percentage if package is currently being loaded, -1 otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Package Utils")
	static float Package_GetLevelAsyncLoadPercentage(const TSoftObjectPtr<UWorld> Level);
};
