// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

#include "RoflanEngine.h"
#include "Components/WidgetInteractionComponent.h"
#include "SensorTouchComponent.generated.h"

class USensorWidgetComponent;

/**
 * @brief A component for physical interaction with sensor 3D widgets.
 * 
 * We use a collision component to trigger overlap events with 3D widget,
 * then we can call touch events and do pointer press key on widget interaction.
 * 
 * The widget interaction MUST point in touch direction, so that the collision
 * should be attached to interaction component to keep sync with its orientation.
 * 
 * TODO: improve interaction orientation handling
 * 
 * @see USensorWidgetComponent
 */
UCLASS(ClassGroup = "UserInterface", meta = (BlueprintSpawnableComponent))
class ROFLANENGINE_API USensorTouchComponent : public UWidgetInteractionComponent
{
	GENERATED_BODY()

public:
	//~Begin UActorComponent interface
	virtual void BeginPlay() override;
	//~End UActorComponent interface

	/**
	 * Press a defined pointer key
	 */
	UFUNCTION(BlueprintCallable, Category = "Sensor Touch")
	void TouchPress();

	/**
	 * Release a defined pointer key
	 */
	UFUNCTION(BlueprintCallable, Category = "Sensor Touch")
	void TouchRelease();

private:
	/**
	 * Called when a collision overlaps another primitive component.
	 * If the component is a sensor widget - call a press touch event.
	 */
	UFUNCTION()
	void OnCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex
		, bool bFromSweep
		, const FHitResult& SweepResult);

	/**
	 * Called when a collision ends overlap another primitive component.
	 * If the component is a sensor widget - call a release touch event.
	 */
	UFUNCTION()
	void OnCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex);

	/**
	 * A name of the component to trigger interactions with world objects.
	 * When the collision overlaps 3D widget we call a press touch event.
	 */
	UPROPERTY(EditAnywhere, Category = "Sensor Touch")
	FString CollisionComponentName;
};
