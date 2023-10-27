// Copyright (C) 2022 GB ltd. All rights reserved.

#include "Game/Interaction/Components/SensorTouchComponent.h"
#include "World/Widget/Components/SensorWidgetComponent.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Launch/Resources/Version.h"

void USensorTouchComponent::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* CollisionComponent = NULL;

	for (auto PrimComp : GetOwner()->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		if (PrimComp->GetName() == CollisionComponentName)
		{
			CollisionComponent = Cast<UPrimitiveComponent>(PrimComp);
			break;
		}
	}

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &USensorTouchComponent::OnCollisionBeginOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &USensorTouchComponent::OnCollisionEndOverlap);

		UE_LOG(LogSensorWidget, Log, TEXT("[Touch] %s: setup with collision component %s"),
			*GetName(), *CollisionComponent->GetName());
	}
	else
	{
		UE_LOG(LogSensorWidget, Error, TEXT("[Touch] %s: can't setup - no collision component with name %s"),
			*GetName(), *CollisionComponentName);
	}
}

void USensorTouchComponent::TouchPress()
{
	UE_LOG(LogSensorWidget, Log, TEXT("[Touch] %s: touch press"), *GetName());
	PressPointerKey(EKeys::LeftMouseButton);

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	// FIXME: W/a for UE-166377
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(this, 0));
#endif
}

void USensorTouchComponent::TouchRelease()
{
	UE_LOG(LogSensorWidget, Log, TEXT("[Touch] %s: touch release"), *GetName());
	ReleasePointerKey(EKeys::LeftMouseButton);

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	// FIXME: W/a for UE-166377
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(this, 0));
#endif
}

void USensorTouchComponent::OnCollisionBeginOverlap(
	UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	auto SensorWidget = Cast<USensorWidgetComponent>(OtherComp);
	if (SensorWidget)
	{
		UE_LOG(LogSensorWidget, Log, TEXT("[Touch] %s: begin overlap with %s (%s)"),
			*GetName(), *SensorWidget->GetName(), *OtherActor->GetName());

		if (SensorWidget->OnTouchPress(this))
		{
			UE_LOG(LogSensorWidget, Log, TEXT("[Touch] %s: pressing sensor %s (%s)"),
				*GetName(), *SensorWidget->GetName(), *OtherActor->GetName());

			TouchPress();
		}
		else
		{
			UE_LOG(LogSensorWidget, Warning, TEXT("[Touch] %s: pressing sensor %s (%s) is not allowed!"),
				*GetName(), *SensorWidget->GetName(), *OtherActor->GetName());
		}
	}
}

void USensorTouchComponent::OnCollisionEndOverlap(
	UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex)
{
	auto SensorWidget = Cast<USensorWidgetComponent>(OtherComp);
	if (SensorWidget)
	{
		UE_LOG(LogSensorWidget, Log, TEXT("[Touch] %s: releasing sensor %s (%s)"),
			*GetName(), *SensorWidget->GetName(), *OtherActor->GetName());

		TouchRelease();
	}
}
