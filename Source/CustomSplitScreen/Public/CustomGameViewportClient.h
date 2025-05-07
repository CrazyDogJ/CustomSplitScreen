// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "Engine/ViewportSplitScreen.h"
#include "CustomGameViewportClient.generated.h"

class UCustomSplitScreenLocalPlayer;

USTRUCT(BlueprintType)
struct FPerPlayerSplitscreenDataBP
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SizeX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SizeY;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OriginX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OriginY;

	FPerPlayerSplitscreenDataBP()
		: SizeX(0)
		, SizeY(0)
		, OriginX(0)
		, OriginY(0)
	{
	}

	FPerPlayerSplitscreenDataBP(float NewSizeX, float NewSizeY, float NewOriginX, float NewOriginY)
		: SizeX(NewSizeX)
		, SizeY(NewSizeY)
		, OriginX(NewOriginX)
		, OriginY(NewOriginY)
	{
	}
};

UENUM(BlueprintType)
namespace ECustomSplitScreenType
{
	enum Type
	{
		// 0 value required by UE
		None,
		// Custom Splitscreen, next value after last built-in splitscreen type
		CustomType = ESplitScreenType::SplitTypeCount,
		SplitTypeCount
   };
}

/**
 * 
 */
UCLASS(Blueprintable)
class CUSTOMSPLITSCREEN_API UCustomGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	UCustomGameViewportClient();

	virtual void UpdateActiveSplitscreenType() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSplitScreenInfo(FPerPlayerSplitscreenDataBP& PlayerSplitscreenData);

	/** Should we override the default split screen behaviour */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bOverrideDefaultSplitScreenInfo = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UCustomSplitScreenLocalPlayer*> SplitScreenLocalPlayers;
	
	/** Add local player split screen view. */
	UFUNCTION(BlueprintCallable)
	UCustomSplitScreenLocalPlayer* AddSplitScreenLocalPlayer(TSubclassOf<UCustomSplitScreenLocalPlayer> InLocalPlayerClass, FPerPlayerSplitscreenDataBP InSplitScreenData);

	/** Remove local player split screen view. */
	UFUNCTION(BlueprintCallable)
	bool RemoveSplitScreenLocalPlayer(UCustomSplitScreenLocalPlayer* InLocalPlayer);
};
