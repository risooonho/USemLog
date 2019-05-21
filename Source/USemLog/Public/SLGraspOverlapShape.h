// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "USemLogSkel.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "SLGraspOverlapShape.generated.h"

/**
* Hand type
*/
UENUM()
enum class ESLGraspOverlapGroup : uint8
{
	A					UMETA(DisplayName = "A"),
	B					UMETA(DisplayName = "B"),
};

/** Delegate to notify that a contact begins between the grasp overlap and an item**/
DECLARE_MULTICAST_DELEGATE_OneParam(FSLGraspOverlapBeginSignature, AActor* /*OtherActor*/);

/** Delegate to notify that a contact ended between the grasp overlap and an item**/
DECLARE_MULTICAST_DELEGATE_OneParam(FSLGraspOverlapEndSignature, AActor* /*OtherActor*/);

/**
 * Semantic overlap generator for grasp detection
 */
UCLASS(ClassGroup = (SL), meta = (BlueprintSpawnableComponent), DisplayName = "SL Grasp Overlap Shape")
class USEMLOG_API USLGraspOverlapShape : public USphereComponent
{
	GENERATED_BODY()

public:
	// Ctor
	USLGraspOverlapShape();

	// Dtor
	~USLGraspOverlapShape();
	
	// Attach to bone 
	bool Init();

	// Start listening to overlaps
	void Start();

	// Pause/continue the overlap detection
	void Idle(bool bInIdle);

	// Stop publishing overlap events
	void Finish(bool bForced = false);

	// Get init state
	bool IsInit() const { return bIsInit; };

	// Get started state
	bool IsStarted() const { return bIsStarted; };

	// Get idle state
	bool IsIdle() const { return bIsIdle; };

	// Get finished state
	bool IsFinished() const { return bIsFinished; };

#if WITH_EDITOR
	// Called when a property is changed in the editor
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// Attach component to bone
	bool AttachToBone();

	// Set debug color
	void SetColor(FColor Color);

	// Publish currently overlapping components
	void TriggerInitialOverlaps();

	// Event called when something starts to overlaps this component
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// Event called when something stops overlapping this component 
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:
	// Forward begin of contact with an item
	FSLGraspOverlapBeginSignature OnBeginSLGraspOverlap;

	// Forward end of contact with an item
	FSLGraspOverlapEndSignature OnEndSLGraspOverlap;

	// Group to which the shape belongs to for the grasping detection
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	ESLGraspOverlapGroup Group;

private:
	// True if initialized
	bool bIsInit;

	// True if started
	bool bIsStarted;

	// True if idle (paused)
	bool bIsIdle;

	// True if finished
	bool bIsFinished;

	// Cache valid contacts
	TSet<AActor*> ActiveContacts;

	//// Active contact count (the additional share the contacts)
	//TMap<AActor*, int32> ActiveContactCounter;

	//// Optional additional collisions triggering the same event
	//UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	//TArray<USphereComponent*> AdditionalCollisions;

	// Name of the skeletal bone to attach the shape to
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	FName BoneName;

	// Snap to the position of the bone when attached
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bSnapToBone;

#if WITH_EDITOR
	// Mimic a button to add the skeletal bone
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bAttachButton;
#endif // WITH_EDITOR

	// If the owner is not a skeletal actor, one needs to add the children (fingers) manually
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bIsNotSkeletal;

	// Explicit references to the other fingers (ignore them during overlap)
	UPROPERTY(EditAnywhere, Category = "Semantic Logger", meta = (editcondition = "bIsNotSkeletal"))
	TArray<AStaticMeshActor*> IgnoredList;

	// Debug with visibility at runtime
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bVisualDebug;
};
