// Copyright (C) 2022 GB ltd. All rights reserved.

#pragma once

#include "RoflanEngine.h"
#include "GameFramework/Actor.h"
#include "TriggerableActor.generated.h"

class UBillboardComponent;
class UBoxComponent;
class USceneComponent;

/**
 * An actor that can be triggered with
 * another actor of a specific class (by overlap collision).
 * 
 * Supports multiple classes.
 * 
 * The actor can be in "triggered" and "non-triggered" states.
 * These states cannot repeat between multiple actors collisions.
 */
UCLASS()
class ROFLANENGINE_API ATriggerableActor : public AActor
{
	GENERATED_BODY()
	
public:
	ATriggerableActor();

	//~Begin AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End AActor interface

	//~Begin ATriggerableActor interface

	/**
	 * Called when the actor is not triggered and
	 * another actor collides with the trigger box
	 * 
	 * @param OtherActor - an actor that triggered the triggerable actor,
	 * can be 'this' if called from SetTriggerable
	 * 
	 * @see SetTriggerable
	 * @see TriggerBox
	 */
	virtual void OnTriggered(AActor* OtherActor);

	/**
	 * Called when the actor is triggered and
	 * the last actor leaves the trigger box
	 * 
	 * @param OtherActor - an actor that untriggered the triggerable actor,
	 * can be 'this' if called from SetTriggerable
	 * 
	 * @see SetTriggerable
	 * @see TriggerBox
	 */
	virtual void OnUntriggered(AActor* OtherActor);

	//~End ATriggerableActor interface

	/**
	 * Activates/diactivates an ability to trigger for this actor.
	 * 
	 * If new triggerable state is False and the actor is currently triggered
	 * an untrigger callback is called forcely.
	 * 
	 * If new triggerable state is True and the actor has allowed actors
	 * in bounds of the trigger - a trigger callback is called forcely.
	 * 
	 * @param bNewTriggerable - a new triggerable state value
	 * 
	 * @see bTriggerable
	 */
	UFUNCTION(BlueprintCallable, Category = "Triggerable Actor")
	void SetTriggerable(bool bNewTriggerable);

	/**
	 * @return True if an actor can be triggered
	 * 
	 * @see bTriggerable
	 */
	UFUNCTION(BlueprintCallable, Category = "Triggerable Actor")
	bool IsTriggerable() const;

	/**
	 * @return True if an actor is currently triggered
	 */
	UFUNCTION(BlueprintCallable, Category = "Triggerable Actor")
	bool IsTriggered() const;

protected:
	/**
	 * @return a root component for this actor component
	 * (unattached from the trigger)
	 * 
	 * @see ActorRoot
	 */
	UFUNCTION(BlueprintCallable, Category = "Triggerable Actor")
	USceneComponent* GetActorRoot() const;

private:
	/**
	 * A callback for trigger box collision overlap begin
	 * 
	 * @see TriggerBox
	 */
	UFUNCTION()
	void OnTriggerBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex
		, bool bFromSweep
		, const FHitResult& SweepResult);

	/**
	 * A callback for trigger box collision overlap end
	 *
	 * @see TriggerBox
	 */
	UFUNCTION()
	void OnTriggerBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex);

	/**
	 * Checks whether an actor can trigger this triggerable actor
	 * @param Actor - an actor to check
	 * @return True if the checked actor can trigger this triggerable actor
	 */
	bool IsActorAllowed(AActor* Actor) const;

	// A dummy root component to keep trigger box unattached from other parts
	UPROPERTY(VisibleAnywhere, Category = "Triggerable Actor")
	USceneComponent* DummyRoot = NULL;

	// A dummy root component for other parts of the actor
	UPROPERTY(VisibleAnywhere, Category = "Triggerable Actor")
	USceneComponent* ActorRoot = NULL;

#if WITH_EDITORONLY_DATA
	// An actor icon bollboard
	UPROPERTY(VisibleAnywhere, Category = "Triggerable Actor")
	UBillboardComponent* Billboard = NULL;
#endif // WITH_EDITORONLY_DATA

	// A box-shape collision, that can be collided by another objects in the world.
	UPROPERTY(VisibleAnywhere, Category = "Triggerable Actor")
	UBoxComponent* TriggerBox = NULL;

	// Classes of actors that can trigger this actor
	UPROPERTY(EditAnywhere, Category = "Triggerable Actor")
	TArray<TSubclassOf<AActor>> AllowedActors;

	/**
	 * Whether the actor can be triggered.
	 * 
	 * true - normal flow
	 * false - gather overlaps count, but not
	 * call trigger and untrigger events
	 */
	UPROPERTY(EditAnywhere, Category = "Triggerable Actor")
	bool bTriggerable = true;

	// A count of allowed actors inside of the trigger
	int32 OverlapsCount = 0;
};
