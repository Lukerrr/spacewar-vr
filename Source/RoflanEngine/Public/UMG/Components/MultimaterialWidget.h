// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UObject/ObjectMacros.h"
#include "MultimaterialWidget.generated.h"

class UMaterialInterface;

/**
 * A widget component with customizable base materials
 */
UCLASS(ClassGroup = "UserInterface", meta = (BlueprintSpawnableComponent))
class ROFLANENGINE_API UMultimaterialWidget : public UWidgetComponent
{
	GENERATED_BODY()

public:
	//~Begin USceneComponent interface
	virtual UMaterialInterface* GetMaterial(int32 MaterialIndex) const override;
	//~End USceneComponent interface

private:
	/** The material instance for translucent widget components */
	UPROPERTY(EditAnywhere, Category = "Material Overrides")
	TObjectPtr<UMaterialInterface> TranslucentMaterial_Override;

	/** The material instance for translucent, one-sided widget components */
	UPROPERTY(EditAnywhere, Category = "Material Overrides")
	TObjectPtr<UMaterialInterface> TranslucentMaterial_OneSided_Override;

	/** The material instance for opaque widget components */
	UPROPERTY(EditAnywhere, Category = "Material Overrides")
	TObjectPtr<UMaterialInterface> OpaqueMaterial_Override;

	/** The material instance for opaque, one-sided widget components */
	UPROPERTY(EditAnywhere, Category = "Material Overrides")
	TObjectPtr<UMaterialInterface> OpaqueMaterial_OneSided_Override;

	/** The material instance for masked widget components. */
	UPROPERTY(EditAnywhere, Category = "Material Overrides")
	TObjectPtr<UMaterialInterface> MaskedMaterial_Override;

	/** The material instance for masked, one-sided widget components. */
	UPROPERTY(EditAnywhere, Category = "Material Overrides")
	TObjectPtr<UMaterialInterface> MaskedMaterial_OneSided_Override;
};
