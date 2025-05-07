// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomGameViewportClient.h"
#include "CustomSplitScreenLocalPlayer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomSplitScreen_BFL.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMSPLITSCREEN_API UCustomSplitScreen_BFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static UCustomGameViewportClient* GetCustomSplitScreenViewportClient(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static TArray<ULocalPlayer*> GetLocalPlayers(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UCustomSplitScreenLocalPlayer* AddSplitScreenCamera(UObject* WorldContextObject, UCameraComponent* CameraComponent);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static ULocalPlayer* AddSplitScreenLocalPlayer(UObject* WorldContextObject, TSubclassOf<ULocalPlayer> InLocalPlayerClass);
	
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static bool RemoveSplitScreenLocalPlayer(UObject* WorldContextObject, ULocalPlayer* InLocalPlayer);
};
