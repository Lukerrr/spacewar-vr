// Copyright (C) 2022 GB ltd. All rights reserved.

#include "World/Widget/Components/SensorWidgetComponent.h"

void USensorWidgetComponent::LockSensor(float Duration)
{
	if (Duration < 0.f)
	{
		// Invalid duration
		UE_LOG(LogSensorWidget, Error, TEXT("[Screen] %s: cannot set lock with invalid duration (%.3f sec)"),
			*GetName(), Duration);
		return;
	}

	UE_LOG(LogSensorWidget, Log, TEXT("[Screen] %s: set lock for %.3f sec"),
		*GetName(), Duration);

	if (Duration == 0.f)
	{
		return;
	}

	auto&& TimerManager = GetWorld()->GetTimerManager();
	const float LockRemain = TimerManager.GetTimerRemaining(LockTimer);

	if (LockRemain < Duration)
	{
		TimerManager.SetTimer(LockTimer, Duration, false);
	}
	else
	{
		// Duration is already satisfied
		UE_LOG(LogSensorWidget, Log, TEXT("[Screen] %s: lock is already satisfied (%.3f remaining)"),
			*GetName(), LockRemain);
	}
}

bool USensorWidgetComponent::IsLocked() const
{
	auto&& TimerManager = GetWorld()->GetTimerManager();
	return TimerManager.GetTimerRemaining(LockTimer) > 0.f;
}

bool USensorWidgetComponent::OnTouchPress(USensorTouchComponent* TouchComponent)
{
	if (!IsLocked())
	{
		LockSensor(TouchPressLockDuration);
		return true;
	}

	return false;
}
