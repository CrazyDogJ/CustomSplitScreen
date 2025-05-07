// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplitScreenLocalPlayer.h"
#include "Kismet/KismetMathLibrary.h"

void UCustomSplitScreenLocalPlayer::GetViewPoint(FMinimalViewInfo& OutViewInfo) const
{
	Super::GetViewPoint(OutViewInfo);
	
	if (GetCameraComponent())
	{
		FMinimalViewInfo ViewInfo;
		GetCameraComponent()->GetCameraView(GetWorld()->GetDeltaSeconds(), ViewInfo);
		OutViewInfo.Location = ViewInfo.Location;
		OutViewInfo.Rotation = ViewInfo.Rotation;
		OutViewInfo.PostProcessSettings = ViewInfo.PostProcessSettings;
		OutViewInfo.PostProcessBlendWeight = ViewInfo.PostProcessBlendWeight;
		// Wow! no need in latest Unreal Engine Version!
		//auto AspectRatio = Size.X / Size.Y;
		//const float FOV_V = UKismetMathLibrary::DegAtan(UKismetMathLibrary::DegTan(ViewInfo.FOV / 2) / 1.33333) * 2;
		//OutViewInfo.FOV = UKismetMathLibrary::DegAtan(UKismetMathLibrary::DegTan(FOV_V/2) * AspectRatio) * 2;;
	}
	else
	{
		OutViewInfo = GetViewInfo();
	}
}

UCameraComponent* UCustomSplitScreenLocalPlayer::GetCameraComponent() const
{
	return CameraComp;
}

bool UCustomSplitScreenLocalPlayer::SetCameraComponent(UCameraComponent* InComp)
{
	if (CameraComp != InComp)
	{
		auto PreCamera = CameraComp;
		CameraComp = InComp;
		OnLocalPlayerCameraCompChanged.Broadcast(PreCamera, CameraComp);
		return true;
	}
	return false;
}

FSceneView* UCustomSplitScreenLocalPlayer::CalcSceneView(class FSceneViewFamily* ViewFamily, FVector& OutViewLocation,
	FRotator& OutViewRotation, FViewport* Viewport, class FViewElementDrawer* ViewDrawer, int32 StereoViewIndex)
{
	auto SuperSceneView = Super::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoViewIndex);
	
	TSet<FPrimitiveComponentId> Ids;
	for (const auto Itr : HiddenComps)
	{
		Ids.Add(Itr->GetPrimitiveSceneId());
	}
	
	if (SuperSceneView && !Ids.IsEmpty())
	{
		SuperSceneView->HiddenPrimitives.Append(Ids);
	}
	
	return SuperSceneView;
}

FMinimalViewInfo UCustomSplitScreenLocalPlayer::GetViewInfo_Implementation() const
{
	FMinimalViewInfo SuperInfo;
	Super::GetViewPoint(SuperInfo);
	return SuperInfo;
}
