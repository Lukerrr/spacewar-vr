// Copyright (C) 2022 GB ltd. All rights reserved.

#include "World/Triggerable/SlidingDoorActor.h"

#include "Utils/Macro/StringUtils.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#define DOORS_SMALL_DISTANCE 0.1f

ASlidingDoorActor::ASlidingDoorActor()
	: Super()
{
	DoorsRoot = CreateDefaultSubobject<USceneComponent>("DoorsRoot");

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>("LeftDoor");
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>("RightDoor");

	OpenTriggerSound = CreateDefaultSubobject<UAudioComponent>("OpenTriggerSound");
	OpenStartSound = CreateDefaultSubobject<UAudioComponent>("OpenStartSound");
	OpenEndSound = CreateDefaultSubobject<UAudioComponent>("OpenEndSound");
	CloseTriggerSound = CreateDefaultSubobject<UAudioComponent>("CloseTriggerSound");
	CloseStartSound = CreateDefaultSubobject<UAudioComponent>("CloseStartSound");
	CloseEndSound = CreateDefaultSubobject<UAudioComponent>("CloseEndSound");

	DoorsRoot->SetupAttachment(GetActorRoot());

	LeftDoor->SetupAttachment(DoorsRoot);
	RightDoor->SetupAttachment(DoorsRoot);

	OpenTriggerSound->SetupAttachment(DoorsRoot);
	OpenStartSound->SetupAttachment(DoorsRoot);
	OpenEndSound->SetupAttachment(DoorsRoot);
	CloseTriggerSound->SetupAttachment(DoorsRoot);
	CloseStartSound->SetupAttachment(DoorsRoot);
	CloseEndSound->SetupAttachment(DoorsRoot);

	OpenTriggerSound->SetAutoActivate(false);
	OpenStartSound->SetAutoActivate(false);
	OpenEndSound->SetAutoActivate(false);
	CloseTriggerSound->SetAutoActivate(false);
	CloseStartSound->SetAutoActivate(false);
	CloseEndSound->SetAutoActivate(false);

	PrimaryActorTick.bCanEverTick = true;
}

void ASlidingDoorActor::BeginPlay()
{
	Super::BeginPlay();

	// Save closed positions
	LeftDoorPosClosed = LeftDoor->GetRelativeLocation();
	RightDoorPosClosed = RightDoor->GetRelativeLocation();
}

void ASlidingDoorActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Invalidate timers
	GetWorld()->GetTimerManager().ClearTimer(DelayedOpenTimer);
	GetWorld()->GetTimerManager().ClearTimer(DelayedCloseTimer);
}

void ASlidingDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CurState = GetState();
	if (CurState == ESlidingDoorState::DOOR_Opening || CurState == ESlidingDoorState::DOOR_Closing)
	{
		ProcessDoorMovement(CurState == ESlidingDoorState::DOOR_Opening);
	}
}

void ASlidingDoorActor::OnTriggered(AActor* OtherActor)
{
	if (GetState() == ESlidingDoorState::DOOR_Closed)
	{
		TriggerOpenDoor();
	}
}

void ASlidingDoorActor::OnUntriggered(AActor* OtherActor)
{
	if (GetState() == ESlidingDoorState::DOOR_Opened)
	{
		TriggerCloseDoor();
	}
}

ESlidingDoorState ASlidingDoorActor::GetState() const
{
	return State;
}

void ASlidingDoorActor::OpenDoor()
{
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': opening door..."), *GetName());

	if (CloseStartSound->IsPlaying())
	{
		CloseStartSound->FadeOut(0.2f, 0.f, EAudioFaderCurve::Linear);
	}
	OpenStartSound->Play();

	SetState(ESlidingDoorState::DOOR_Opening);
}

void ASlidingDoorActor::CloseDoor()
{
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': closing door..."), *GetName());

	if (OpenStartSound->IsPlaying())
	{
		OpenStartSound->FadeOut(0.2f, 0.f, EAudioFaderCurve::Linear);
	}
	CloseStartSound->Play();

	SetState(ESlidingDoorState::DOOR_Closing);
}

void ASlidingDoorActor::TriggerOpenDoor()
{
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': trigger open door"), *GetName());

	SetState(ESlidingDoorState::DOOR_TriggeredOpen);

	OpenTriggerSound->Play();

	auto&& TimerManager = GetWorld()->GetTimerManager();


	if (OpenDelay > 0.f)
	{
		UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': set open door after %f sec"),
			*GetName(), OpenDelay);
		TimerManager.SetTimer(DelayedOpenTimer, [this] { OpenDoor(); }, OpenDelay, false);
	}
	else
	{
		OpenDoor();
	}

	if (TimerManager.IsTimerActive(DelayedCloseTimer))
	{
		TimerManager.ClearTimer(DelayedCloseTimer);
		UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': delayed close timer is cleared"), *GetName());
	}
}

void ASlidingDoorActor::TriggerCloseDoor()
{
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': trigger close door"), *GetName());

	SetState(ESlidingDoorState::DOOR_TriggeredClose);

	CloseTriggerSound->Play();

	auto&& TimerManager = GetWorld()->GetTimerManager();

	if (CloseDelay > 0.f)
	{
		UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': set close door after %f sec"),
			*GetName(), OpenDelay);
		TimerManager.SetTimer(DelayedCloseTimer, [this] { CloseDoor(); }, CloseDelay, false);
	}
	else
	{
		CloseDoor();
	}

	if (TimerManager.IsTimerActive(DelayedOpenTimer))
	{
		TimerManager.ClearTimer(DelayedOpenTimer);
		UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': delayed open timer is cleared"), *GetName());
	}
}

void ASlidingDoorActor::OnOpened()
{
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': door is opened"), *GetName());

	if (OpenStartSound->IsPlaying())
	{
		OpenStartSound->FadeOut(0.2f, 0.f, EAudioFaderCurve::Linear);
	}
	OpenEndSound->Play();

	if (!IsTriggerable() || !IsTriggered())
	{
		TriggerCloseDoor();
	}
}

void ASlidingDoorActor::OnClosed()
{
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': door is closed"), *GetName());

	if (CloseStartSound->IsPlaying())
	{
		CloseStartSound->FadeOut(0.2f, 0.f, EAudioFaderCurve::Linear);
	}
	CloseEndSound->Play();

	if (IsTriggerable() && IsTriggered())
	{
		TriggerOpenDoor();
	}
}

void ASlidingDoorActor::OnOpeningFinished()
{
	if (MinOpenedDelay > 0.f)
	{
		UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': opening is finished, waiting for %f to completely open"),
			*GetName(), MinOpenedDelay);

		auto&& TimerManager = GetWorld()->GetTimerManager();
		FTimerHandle OpenedLockTimer;
		TimerManager.SetTimer(OpenedLockTimer, [this]
			{
				SetState(ESlidingDoorState::DOOR_Opened);
			}, MinOpenedDelay, false);
	}
	else
	{
		SetState(ESlidingDoorState::DOOR_Opened);
	}
}

void ASlidingDoorActor::OnClosingFinished()
{
	if (MinClosedDelay > 0.f)
	{
		UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': opening is finished, waiting for %f to completely open"),
			*GetName(), MinClosedDelay);

		auto&& TimerManager = GetWorld()->GetTimerManager();
		FTimerHandle ClosedLockTimer;
		TimerManager.SetTimer(ClosedLockTimer, [this]
			{
				SetState(ESlidingDoorState::DOOR_Closed);
			}, MinClosedDelay, false);
	}
	else
	{
		SetState(ESlidingDoorState::DOOR_Closed);
	}
}

void ASlidingDoorActor::ProcessDoorMovement(bool bOpening)
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	const FVector LeftDoorPos = LeftDoor->GetRelativeLocation();
	const FVector RightDoorPos = RightDoor->GetRelativeLocation();

	const FVector LeftDoorPosOpened = LeftDoorPosClosed + LeftDoorSlideVector.GetSafeNormal() * (OpenDistance / 2.f);
	const FVector RightDoorPosOpened = RightDoorPosClosed + RightDoorSlideVector.GetSafeNormal() * (OpenDistance / 2.f);

	auto IsAtLocation = [](USceneComponent* Comp, const FVector& Loc)
	{
		return FVector::PointsAreNear(Comp->GetRelativeLocation(), Loc, DOORS_SMALL_DISTANCE);
	};

	const FVector& LeftDoorTargetPos = bOpening ? LeftDoorPosOpened : LeftDoorPosClosed;
	const FVector& RightDoorTargetPos = bOpening ? RightDoorPosOpened : RightDoorPosClosed;

	bool bLeftDoorAtLocation = IsAtLocation(LeftDoor, LeftDoorTargetPos);
	bool bRightDoorAtLocation = IsAtLocation(RightDoor, RightDoorTargetPos);

	// Tick movement animation if necessary
	if (!bLeftDoorAtLocation || !bRightDoorAtLocation)
	{
		if (!bLeftDoorAtLocation)
		{
			const FVector LeftDoorNewPos = FMath::VInterpConstantTo(LeftDoorPos, LeftDoorTargetPos, DeltaTime, MovementSpeed);
			LeftDoor->SetRelativeLocation(LeftDoorNewPos);
			bLeftDoorAtLocation = IsAtLocation(LeftDoor, LeftDoorTargetPos);
		}

		if (!bRightDoorAtLocation)
		{
			const FVector RightDoorNewPos = FMath::VInterpConstantTo(RightDoorPos, RightDoorTargetPos, DeltaTime, MovementSpeed);
			RightDoor->SetRelativeLocation(RightDoorNewPos);
			bRightDoorAtLocation = IsAtLocation(RightDoor, RightDoorTargetPos);
		}

		// Movement completed
		if (bLeftDoorAtLocation && bRightDoorAtLocation)
		{
			SetState(bOpening ? ESlidingDoorState::DOOR_OpeningEnd : ESlidingDoorState::DOOR_ClosingEnd);
		}
	}
}

void ASlidingDoorActor::SetState(ESlidingDoorState NewState)
{
	State = NewState;
	UE_LOG(LogTriggerableActor, Log, TEXT("[ASlidingDoorActor] '%s': state is set to %s"), *GetName(), *ENUM_TO_STRING(ESlidingDoorState, State));

	switch (State)
	{
	case ESlidingDoorState::DOOR_Opened:
		OnOpened();
		break;
	case ESlidingDoorState::DOOR_Closed:
		OnClosed();
		break;
	case ESlidingDoorState::DOOR_OpeningEnd:
		OnOpeningFinished();
		break;
	case ESlidingDoorState::DOOR_ClosingEnd:
		OnClosingFinished();
		break;
	default:
		break;
	}

}
