// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplitScreen_BFL.h"

UCustomGameViewportClient* UCustomSplitScreen_BFL::GetCustomSplitScreenViewportClient(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	
	return Cast<UCustomGameViewportClient>(WorldContextObject->GetWorld()->GetGameInstance()->GetGameViewportClient());
}

TArray<ULocalPlayer*> UCustomSplitScreen_BFL::GetLocalPlayers(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return TArray<ULocalPlayer*>();
	}
	
	return WorldContextObject->GetWorld()->GetGameInstance()->GetLocalPlayers();
}

UCustomSplitScreenLocalPlayer* UCustomSplitScreen_BFL::AddSplitScreenCamera(UObject* WorldContextObject, UCameraComponent* CameraComponent)
{
	const auto NewLocal = AddSplitScreenLocalPlayer(WorldContextObject, UCustomSplitScreenLocalPlayer::StaticClass());
	const auto NewCustomLocal = Cast<UCustomSplitScreenLocalPlayer>(NewLocal);
	NewCustomLocal->SetCameraComponent(CameraComponent);
	return NewCustomLocal;
}

ULocalPlayer* UCustomSplitScreen_BFL::AddSplitScreenLocalPlayer(UObject* WorldContextObject, TSubclassOf<ULocalPlayer> InLocalPlayerClass)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		return nullptr;
	}

	if (InLocalPlayerClass)
	{
		const int32 NumPlayers = WorldContextObject->GetWorld()->GetGameInstance()->GetLocalPlayers().Num();
		
		// Platform user id
		FPlatformUserId UserId = FGenericPlatformMisc::GetPlatformUserForUserIndex(NumPlayers);
		FInputDeviceId DummyInputDevice = INPUTDEVICEID_NONE;
		IPlatformInputDeviceMapper::Get().RemapControllerIdToPlatformUserAndDevice(NumPlayers, UserId, DummyInputDevice);

		// New local player
		const auto Engine = WorldContextObject->GetWorld()->GetGameInstance()->GetEngine();
		const auto NewPlayer = NewObject<ULocalPlayer>(Engine, InLocalPlayerClass);
		NewPlayer->PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController();

		WorldContextObject->GetWorld()->GetGameInstance()->AddLocalPlayer(NewPlayer, UserId);
		return NewPlayer;
	}
	
	return nullptr;
}

bool UCustomSplitScreen_BFL::RemoveSplitScreenLocalPlayer(UObject* WorldContextObject, ULocalPlayer* InLocalPlayer)
{
	if (!WorldContextObject || !InLocalPlayer)
	{
		return false;
	}

	if (InLocalPlayer->PlayerController == WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		InLocalPlayer->PlayerController = nullptr;
	}
	
	return WorldContextObject->GetWorld()->GetGameInstance()->RemoveLocalPlayer(InLocalPlayer);
}
