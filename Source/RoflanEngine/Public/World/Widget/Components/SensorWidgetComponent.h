// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

#include "RoflanEngine.h"
#include "Components/WidgetComponent.h"
#include "SensorWidgetComponent.generated.h"

/**
 * A 3D widget that can be touched with a collision.
 * 
 * @see SensorTouchComponent
 */
UCLASS(Blueprintable, ClassGroup = "UserInterface", hidecategories = (Object, Activation, "Components|Activation", Sockets, Base, Lighting, LOD, Mesh), editinlinenew, meta = (BlueprintSpawnableComponent))
class ROFLANENGINE_API USensorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	friend class USensorTouchComponent;

public:
	/**
	 * Lock sensor touch for a given duration.
	 * If the widget has an active lock:
	 *   - update lock with new duration if the old one is less
	 *   - ignore if the old duration is greater
	 * 
	 * @param Duration - lock duration in seconds
	 */
	UFUNCTION(BlueprintCallable, Category = "Sensor Widget")
	void LockSensor(float Duration);

	/**
	 * @return True if the sensor is locked and cannot be touched
	 */
	UFUNCTION(BlueprintCallable, Category = "Sensor Widget")
	bool IsLocked() const;

private:
	/**
	 * Validates a touch press, dispatching any internal settings (e.g. touch delays)
	 * 
	 * @param TouchComponent - a component that presses the sensor
	 * @return True if the press is allowed
	 */
	bool OnTouchPress(USensorTouchComponent* TouchComponent);

	// A diration in seconds for a sensor lock after it is touched 
	UPROPERTY(EditAnywhere, Category = "Sensor Widget")
	float TouchPressLockDuration = 0.0f;

	/**
	 * A sensor lock timer.
	 * All touch presses are ignored while the lock is active.
	 */
	FTimerHandle LockTimer;
};
