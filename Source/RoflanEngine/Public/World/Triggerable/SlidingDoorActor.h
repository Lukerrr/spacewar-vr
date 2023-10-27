// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

#include "RoflanEngine.h"
#include "World/Triggerable/TriggerableActor.h"
#include "SlidingDoorActor.generated.h"

class UAudioComponent;
class UStaticMeshComponent;

/**
 * A enumeration for sliding door states
 */
UENUM(BlueprintType)
enum class ESlidingDoorState : uint8
{
	// Fully opened, but untriggered, needs to be closed after delay
	DOOR_TriggeredClose UMETA(DisplayName = "Triggered Close"),

	// Fully closed, but triggered, needs to be opened after delay
	DOOR_TriggeredOpen UMETA(DisplayName = "Triggered Open"),

	// Movement to close is active
	DOOR_Closing UMETA(DisplayName = "Closing"),

	// Movement to open is active
	DOOR_Opening UMETA(DisplayName = "Opening"),

	// Movement to close is finished, waiting for closed lock
	DOOR_ClosingEnd UMETA(DisplayName = "Closing End"),

	// Movement to open is finished, waiting for opened lock
	DOOR_OpeningEnd UMETA(DisplayName = "Opening End"),

	// Fully closed
	DOOR_Closed UMETA(DisplayName = "Closed"),

	// Fully opened
	DOOR_Opened UMETA(DisplayName = "Opened"),
};

/**
 * A door that contains two meshes sliding
 * in opposite directions per trigger
 */
UCLASS()
class ROFLANENGINE_API ASlidingDoorActor : public ATriggerableActor
{
	GENERATED_BODY()
	
public:
	ASlidingDoorActor();

	//~Begin AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	//~End AActor interface

	//~Begin ATriggerableActor interface
	virtual void OnTriggered(AActor* OtherActor) override;
	virtual void OnUntriggered(AActor* OtherActor) override;
	//~End ATriggerableActor interface

	UFUNCTION(BlueprintCallable, Category = "Sliding Door Actor")
	ESlidingDoorState GetState() const;

	/**
	 * Starts an opening animation immediately
	 */
	UFUNCTION(BlueprintCallable, Category = "Sliding Door Actor")
	void OpenDoor();

	/**
	 * Starts an opening animation in reverse immediately
	 */
	UFUNCTION(BlueprintCallable, Category = "Sliding Door Actor")
	void CloseDoor();

	/**
	 * Opens the door, considering any delays
	 */
	UFUNCTION(BlueprintCallable, Category = "Sliding Door Actor")
	void TriggerOpenDoor();

	/**
	 * Closes the door, considering any delays
	 */
	UFUNCTION(BlueprintCallable, Category = "Sliding Door Actor")
	void TriggerCloseDoor();

private:

	/**
	 * Called when a door is fully opened
	 */
	void OnOpened();

	/**
	 * Called when a door is fully closed
	 */
	void OnClosed();

	/**
	 * Called when a door opening movement is finished
	 * We need to have a delay before completely move to opened state
	 */
	void OnOpeningFinished();

	/**
	 * Called when a door closing movement is finished
	 * We need to have a delay before completely move to closed state
	 */
	void OnClosingFinished();

	/**
	 * Tick movement for doors opening or closing
	 * 
	 * @param bOpening - whether doors are opening
	 */
	void ProcessDoorMovement(bool bOpening);

	/**
	 * Set a new door state and call corresponding events
	 * 
	 * @param NewState - state value
	 */
	void SetState(ESlidingDoorState NewState);

	// A dummy root component for doors
	UPROPERTY(VisibleAnywhere, Category = "Sliding Door Actor")
	USceneComponent* DoorsRoot = NULL;

	// Left sliding door part
	UPROPERTY(VisibleAnywhere, Category = "Sliding Door Actor")
	UStaticMeshComponent* LeftDoor = NULL;

	// Right sliding door part
	UPROPERTY(VisibleAnywhere, Category = "Sliding Door Actor")
	UStaticMeshComponent* RightDoor = NULL;

	// A sound when door opening is triggered
	UPROPERTY(VisibleAnywhere, Category = "Door Sounds")
	UAudioComponent* OpenTriggerSound = NULL;

	// A sound when door begins opening
	UPROPERTY(VisibleAnywhere, Category = "Door Sounds")
	UAudioComponent* OpenStartSound = NULL;

	// A sound when door is fully opened
	UPROPERTY(VisibleAnywhere, Category = "Door Sounds")
	UAudioComponent* OpenEndSound = NULL;

	// A sound when door closing is triggered
	UPROPERTY(VisibleAnywhere, Category = "Door Sounds")
	UAudioComponent* CloseTriggerSound = NULL;

	// A sound when door begins closing
	UPROPERTY(VisibleAnywhere, Category = "Door Sounds")
	UAudioComponent* CloseStartSound = NULL;

	// A sound when door is fully closed
	UPROPERTY(VisibleAnywhere, Category = "Door Sounds")
	UAudioComponent* CloseEndSound = NULL;

	// A local vector of opening for left door
	UPROPERTY(EditAnywhere, Category = "Door Params")
	FVector LeftDoorSlideVector = FVector::RightVector;

	// A local vector of opening for left door
	UPROPERTY(EditAnywhere, Category = "Door Params")
	FVector RightDoorSlideVector = -FVector::RightVector;

	// A delay between trigger and open in seconds
	UPROPERTY(EditAnywhere, Category = "Door Params")
	float OpenDelay = 0.f;

	// A delay between untrigger and close in seconds
	UPROPERTY(EditAnywhere, Category = "Door Params")
	float CloseDelay = 0.f;

	// A delay between Opening->Opened state transition
	UPROPERTY(EditAnywhere, Category = "Door Params")
	float MinOpenedDelay = 0.f;

	// A delay between Closing->Closed state transition
	UPROPERTY(EditAnywhere, Category = "Door Params")
	float MinClosedDelay = 0.f;

	// A distance of door opening in units
	UPROPERTY(EditAnywhere, Category = "Door Params")
	float OpenDistance = 200.f;

	// A speed of door opening and closing
	UPROPERTY(EditAnywhere, Category = "Door Params")
	float MovementSpeed = 100.f;

	// A local position of closed left door
	FVector LeftDoorPosClosed = FVector::ZeroVector;

	// A local position of closed right door
	FVector RightDoorPosClosed = FVector::ZeroVector;

	// A timer to open door in delay
	FTimerHandle DelayedOpenTimer;

	// A timer to close door in delay
	FTimerHandle DelayedCloseTimer;

	// Doors state (TODO: implement a state machine component)
	ESlidingDoorState State = ESlidingDoorState::DOOR_Closed;
};
