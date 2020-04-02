// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/SLIndividual.h"
#include "SLVisualIndividual.generated.h"

/**
 * 
 */
UCLASS()
class USEMLOG_API USLVisualIndividual : public USLIndividual
{
	GENERATED_BODY()

public:
    // Ctor
    USLVisualIndividual();

    // Called after the C++ constructor and after the properties have been initialized, including those loaded from config.
    virtual void PostInitProperties() override;

    // Save data to owners tag
    virtual bool SaveToTag(bool bOverwrite = false) override;

    // Load data from owners tag
    virtual bool LoadFromTag(bool bOverwrite = false) override;

    // All properties are set for runtime
    virtual bool IsRuntimeReady() const;
    
    /* Visual mask */
    void SetVisualMask(const FString& InVisualMask, bool bClearCalibratedValue = true);
    FString GetVisualMask() const { return VisualMask; };
    bool HasVisualMask() const { return !VisualMask.IsEmpty(); };

    /* Calibrated visual mask */
    void SetCalibratedVisualMask(const FString& InCalibratedVisualMask) { CalibratedVisualMask = InCalibratedVisualMask; };
    FString GetCalibratedVisualMask() const { return CalibratedVisualMask; };
    bool HasCalibratedVisualMask() const { return !CalibratedVisualMask.IsEmpty(); };

private:
    // Apply color to the dynamic material
    bool ApplyVisualMaskColorToDynamicMaterial();

protected:
    // Skeletal body part individual unique id
    UPROPERTY(EditAnywhere, Category = "SL")
    FString VisualMask;

    // Skeletal body part individual class
    UPROPERTY(EditAnywhere, Category = "SL")
    FString CalibratedVisualMask;

    // The visual component of the owner
    class UStaticMeshComponent* VisualStaticMeshComponent;

    // Material template used for creating dynamic materials
    class UMaterial* VisualMaskMaterial;

    // Dynamic material used for setting various mask colors
    class UMaterialInstanceDynamic* VisualMaskDynamicMaterial;
};