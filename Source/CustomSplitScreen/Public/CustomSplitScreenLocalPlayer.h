// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameViewportClient.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "CustomSplitScreenLocalPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLocalPlayerCameraCompChanged, UCameraComponent*, PreCamera, UCameraComponent*, CurrentCamera);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CUSTOMSPLITSCREEN_API UCustomSplitScreenLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	virtual void GetViewPoint(FMinimalViewInfo& OutViewInfo) const override;

	UFUNCTION(BlueprintPure)
	UCameraComponent* GetCameraComponent() const;

	UFUNCTION(BlueprintCallable)
	bool SetCameraComponent(UCameraComponent* InComp);

	UPROPERTY(BlueprintAssignable)
	FOnLocalPlayerCameraCompChanged OnLocalPlayerCameraCompChanged;

	UFUNCTION(BlueprintNativeEvent)
	FMinimalViewInfo GetViewInfo() const;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPerPlayerSplitscreenDataBP SplitScreenInfo;

	virtual FSceneView* CalcSceneView(class FSceneViewFamily* ViewFamily, FVector& OutViewLocation, FRotator& OutViewRotation, FViewport* Viewport, class FViewElementDrawer* ViewDrawer = 0, int32 StereoViewIndex = INDEX_NONE) override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UPrimitiveComponent*> HiddenComps;
	
private:
	UPROPERTY()
	UCameraComponent* CameraComp;
};
