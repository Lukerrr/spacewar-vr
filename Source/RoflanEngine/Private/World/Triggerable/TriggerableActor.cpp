// Copyright (C) 2022 GB ltd. All rights reserved.

#include "World/Triggerable/TriggerableActor.h"

#include "Utils/Macro/StringUtils.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

#if WITH_EDITORONLY_DATA
#include "UObject/ConstructorHelpers.h"
#endif // WITH_EDITORONLY_DATA

ATriggerableActor::ATriggerableActor()
{
	DummyRoot = CreateDefaultSubobject<USceneComponent>("DummyRoot");
	ActorRoot = CreateDefaultSubobject<USceneComponent>("ActorRoot");
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");

	SetRootComponent(DummyRoot);
	ActorRoot->SetupAttachment(DummyRoot);
	TriggerBox->SetupAttachment(DummyRoot);

#if WITH_EDITORONLY_DATA
	const ConstructorHelpers::FObjectFinder<UTexture2D> DefaultBillboardTextureFinder(
		TEXT("'/Engine/EditorResources/S_Actor.S_Actor'"));

	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(DummyRoot);
	Billboard->SetSprite(DefaultBillboardTextureFinder.Object);
#endif // WITH_EDITORONLY_DATA

	PrimaryActorTick.bCanEverTick = false;
}

void ATriggerableActor::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerableActor::OnTriggerBoxBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerableActor::OnTriggerBoxEndOverlap);

	// Overlap all physics objects
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ATriggerableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Disable overlap responsees
	TriggerBox->OnComponentBeginOverlap.RemoveAll(this);
	TriggerBox->OnComponentEndOverlap.RemoveAll(this);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATriggerableActor::OnTriggered(AActor* OtherActor)
{
}

void ATriggerableActor::OnUntriggered(AActor* OtherActor)
{
}

void ATriggerableActor::SetTriggerable(bool bNewTriggerable)
{

	if (bTriggerable != bNewTriggerable)
	{
		bTriggerable = bNewTriggerable;

		UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': triggerable is set to '%s', "
			"overlaps count = %d"),
			*GetName(), *BOOL_TO_STRING(bTriggerable), OverlapsCount);

		if (IsTriggered())
		{
			if (!bTriggerable)
			{
				UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': forcing an untriggered event"), *GetName());
				OnUntriggered(this);
			}
			else
			{
				UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': forcing a triggered event"), *GetName());
				OnTriggered(this);
			}
		}
	}
}

bool ATriggerableActor::IsTriggerable() const
{
	return bTriggerable;
}

bool ATriggerableActor::IsTriggered() const
{
	return OverlapsCount > 0;
}

USceneComponent* ATriggerableActor::GetActorRoot() const
{
	return ActorRoot;
}

void ATriggerableActor::OnTriggerBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex
	, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (!IsValid(this))
	{
		return;
	}

	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (IsActorAllowed(OtherActor))
	{
		const bool bWasTriggered = IsTriggered();

		++OverlapsCount;

		// Invoke trigger if not triggered
		if (!bWasTriggered)
		{
			if (IsTriggerable())
			{
				UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': Begin overlapping with the "
					"first actor '%s', triggering"),
					*GetName(), *OtherActor->GetName());

				OnTriggered(OtherActor);
			}
			else
			{
				UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': Begin overlapping with the "
					"first actor '%s', cannot trigger because not triggerable"),
					*GetName(), *OtherActor->GetName());
			}
		}
		else
		{
			UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': Begin overlapping with actor "
				"'%s', but is already triggered (%d overlaps in total)"),
				*GetName(), *OtherActor->GetName(), OverlapsCount);
		}
	}
	else
	{
		UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': Begin overlapping with unallowed "
			"actor '%s' (class = '%s')"),
			*GetName(), *OtherActor->GetName(), *OtherActor->GetClass()->GetName());
	}
}

void ATriggerableActor::OnTriggerBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor
	, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex)
{
	if (!IsValid(this))
	{
		return;
	}

	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	if (!IsTriggered())
	{
		// Just ignore if not triggered before
		UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': End overlapping with actor "
			"'%s', but is not triggered"),
			*GetName(), *OtherActor->GetName());
		return;
	}

	if (IsActorAllowed(OtherActor))
	{
		--OverlapsCount;

		// Invoke untrigger if no other actors inside
		if (!IsTriggered())
		{
			if (IsTriggerable())
			{
				UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': End overlapping with the "
					"last actor '%s', untriggering"),
					*GetName(), *OtherActor->GetName());

				OnUntriggered(OtherActor);
			}
			else
			{
				UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': End overlapping with the "
					"last actor '%s', cannot untrigger because not triggerable"),
					*GetName(), *OtherActor->GetName());
			}
		}
		else
		{
			UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': End overlapping with actor "
				"'%s', but is still triggered (%d overlaps left)"),
				*GetName(), *OtherActor->GetName(), OverlapsCount);
		}
	}
	else
	{
		UE_LOG(LogTriggerableActor, Log, TEXT("[ATriggerableActor] '%s': End overlapping with unallowed "
			"actor '%s' (class = '%s')"),
			*GetName(), *OtherActor->GetName(), *OtherActor->GetClass()->GetName());
	}
}

bool ATriggerableActor::IsActorAllowed(AActor* Actor) const
{
	for (auto&& AllowedActorClass : AllowedActors)
	{
		if (Actor->GetClass()->IsChildOf(AllowedActorClass))
		{
			return true;
		}
	}
	return false;
}
