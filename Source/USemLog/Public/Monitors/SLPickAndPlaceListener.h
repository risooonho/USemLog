#pragma once
// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "USemLog.h"
#include "Components/ActorComponent.h"
#include "SLStructs.h" // FSLEntity
#include "SLContactShapeInterface.h"
#include "SLPickAndPlaceListener.generated.h"


/**
* Hand type
*/
UENUM()
enum class ESLPaPStateCheck : uint8
{
	NONE								UMETA(DisplayName = "NONE"),
	Slide								UMETA(DisplayName = "Slide"),
	PickUp								UMETA(DisplayName = "PickUp"),
	TransportOrPutDown					UMETA(DisplayName = "TransportOrPutDown"),
	PutDown								UMETA(DisplayName = "PutDown"),
	
};

/**
 * Various PaP events helpers
 */
struct FSLPaPSlide
{
public:
	// Default ctor
	FSLPaPSlide() = default;
	
	float MovedDist;

	// End time of the event 
	float StartTime;
};

/** Notify the beginning and the end of the pick and place related events */
DECLARE_MULTICAST_DELEGATE_FourParams(FSLPaPSubEventSignature, const FSLEntity& /*Self*/, AActor* /*Other*/, float /*StartTime*/, float /*EndTime*/);

/**
 * Checks for manipulator related events (contact, grasp, lift, transport, slide)
 */
UCLASS( ClassGroup=(SL), meta=(BlueprintSpawnableComponent), DisplayName = "SL PickAndPlace Listener")
class USEMLOG_API USLPickAndPlaceListener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLPickAndPlaceListener();

	// Dtor
	~USLPickAndPlaceListener();

	// Check if owner is valid and semantically annotated
	bool Init();

	// Start listening to grasp events, update currently overlapping objects
	void Start();

	// Stop publishing grasp events
	void Finish(float EndTime, bool bForced = false);

	// Get init state
	bool IsInit() const { return bIsInit; };

	// Get started state
	bool IsStarted() const { return bIsStarted; };

	// Get finished state
	bool IsFinished() const { return bIsFinished; };

private:
	// Subscribe to grasp events from sibling
	bool SubscribeForGraspEvents();

	// Get grasped objects contact shape component
	ISLContactShapeInterface* GetContactShapeComponent(AActor* Actor) const;

	// Called on grasp begin
	void OnSLGraspBegin(const FSLEntity& Self, AActor* Other, float Time, const FString& GraspType);

	// Called on grasp end
	void OnSLGraspEnd(const FSLEntity& Self, AActor* Other, float Time);

	// Update callback
	void Update();

	// Object released, terminate active even
	void FinishActiveEvent(float CurrTime);

	// Backtrace and check if a put-down event happened
	bool HasPutDownEventHappened(const float CurrTime,const FVector& CurrObjLocation,  uint32& OutPutDownEndIdx);

	// State update functions
	void Update_NONE();
	void Update_Slide();
	void Update_PickUp();
	void Update_TransportOrPutDown();

public:
	// PaP events delegates
	FSLPaPSubEventSignature OnManipulatorSlideEvent;
	FSLPaPSubEventSignature OnManipulatorPickUpEvent;
	FSLPaPSubEventSignature OnManipulatorPutDownEvent;
	FSLPaPSubEventSignature OnManipulatorTransportEvent;
	
private:
	// Skip initialization if true
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bIgnore;

	// True if initialized
	bool bIsInit;

	// True if started
	bool bIsStarted;

	// True if finished
	bool bIsFinished;

	// Semantic data of the owner
	FSLEntity SemanticOwner;

	// Object currently grasped
	AActor* CurrGraspedObj;

	// Previous event check
	ESLPaPStateCheck EventCheck;

	// Cache of various relevant locations
	FVector PrevRelevantLocation;

	// Cache of various relevant times
	float PrevRelevantTime;

	// Contact shape of the grasped object, holds information if the object is supported by a surface
	ISLContactShapeInterface* GraspedObjectContactShape;
	
	// Update timer handle
	FTimerHandle UpdateTimerHandle;

	/* Update function bindings */
	// Function pointer type for calling the correct update function
	typedef void(USLPickAndPlaceListener::*UpdateFunctionPointerType)();

	// Function pointer for the state check update callback
	UpdateFunctionPointerType UpdateFunctionPtr;

	/* PickUp related */
	// Set when the object is lifted from the supported area more than the MinPickUpHeight value
	bool bLiftOffHappened;

	// The location where the object was started to be lifted (use this to compare against MaxPickUpDistXY and MaxPickUpHeight)
	FVector LiftOffLocation;

	/* PutDown related */
	// Past locations and time during transport in order to backtrace and detect put-down events
	TArray<TPair<float, FVector>> RecentMovementBuffer;

	/* Constants */
	constexpr static float UpdateRate = 0.05f;

	// Slide
	constexpr static float MinSlideDistXY = 9.f;
	constexpr static float MinSlideDuration = 0.9f;
	
	// PickUp
	constexpr static float MaxPickUpDistXY = 9.f;
	constexpr static float MinPickUpHeight = 3.f;
	constexpr static float MaxPickUpHeight = 12.f;

	// PutDown
	constexpr static int32 RecentMovementBufferSize = 256;
	constexpr static float RecentMovementBufferDuration = 3.3f;
	constexpr static float PutDownMovementBacktrackDuration = 1.5f;
	constexpr static float MinPutDownHeight = 2.f;

	constexpr static float MaxPutDownHeight = 8.f;
	constexpr static float MaxPutDownDistXY = 9.f;
};

