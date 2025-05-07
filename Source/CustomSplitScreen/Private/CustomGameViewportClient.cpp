// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameViewportClient.h"

#include "CustomSplitScreenLocalPlayer.h"


UCustomGameViewportClient::UCustomGameViewportClient()
     : Super(FObjectInitializer::Get())
{
	// Resize SplitscreenInfo to fit our custom config
	SplitscreenInfo.SetNum(ECustomSplitScreenType::SplitTypeCount, EAllowShrinking::No);
}

void UCustomGameViewportClient::UpdateActiveSplitscreenType()
{
	const int32 NumPlayers = GetGameInstance()->GetLocalPlayers().Num();

	if (NumPlayers >= 2 && bOverrideDefaultSplitScreenInfo)
	{
		ActiveSplitscreenType = static_cast<ESplitScreenType::Type>(ECustomSplitScreenType::CustomType);
		SplitscreenInfo[ECustomSplitScreenType::CustomType].PlayerData.Empty();

		// Update first local player.
		FPerPlayerSplitscreenDataBP FirstLocalPlayerResult;
		UpdateSplitScreenInfo(FirstLocalPlayerResult);
		const auto N = FPerPlayerSplitscreenData(
			FirstLocalPlayerResult.SizeX,
			FirstLocalPlayerResult.SizeY,
			FirstLocalPlayerResult.OriginX,
			FirstLocalPlayerResult.OriginY);
		SplitscreenInfo[ECustomSplitScreenType::CustomType].PlayerData.EmplaceAt(0, N);

		// Update added local players.
		for (const auto Itr : SplitScreenLocalPlayers)
		{
			const auto Index = Itr->GetLocalPlayerIndex();
			const auto NativeItr = FPerPlayerSplitscreenData(
				Itr->SplitScreenInfo.SizeX,
				Itr->SplitScreenInfo.SizeY,
				Itr->SplitScreenInfo.OriginX,
				Itr->SplitScreenInfo.OriginY);;
			SplitscreenInfo[ECustomSplitScreenType::CustomType].PlayerData.EmplaceAt(Index, NativeItr);
		}
	}
	else
	{
		// Otherwise fallback to default behaviour
		Super::UpdateActiveSplitscreenType();
	}
}

UCustomSplitScreenLocalPlayer* UCustomGameViewportClient::AddSplitScreenLocalPlayer(TSubclassOf<UCustomSplitScreenLocalPlayer> InLocalPlayerClass, FPerPlayerSplitscreenDataBP InSplitScreenData)
{
	if (!GetWorld())
	{
		return nullptr;
	}

	if (InLocalPlayerClass)
	{
		const int32 NumPlayers = GetWorld()->GetGameInstance()->GetLocalPlayers().Num();
		
		// Platform user id
		FPlatformUserId UserId = FGenericPlatformMisc::GetPlatformUserForUserIndex(NumPlayers);
		FInputDeviceId DummyInputDevice = INPUTDEVICEID_NONE;
		IPlatformInputDeviceMapper::Get().RemapControllerIdToPlatformUserAndDevice(NumPlayers, UserId, DummyInputDevice);

		// New local player
		const auto Engine = GetWorld()->GetGameInstance()->GetEngine();
		const auto NewPlayer = NewObject<UCustomSplitScreenLocalPlayer>(Engine, InLocalPlayerClass);
		NewPlayer->PlayerController = GetWorld()->GetFirstPlayerController();
		NewPlayer->SplitScreenInfo = InSplitScreenData;

		GetWorld()->GetGameInstance()->AddLocalPlayer(NewPlayer, UserId);
		SplitScreenLocalPlayers.Add(NewPlayer);
		return NewPlayer;
	}
	
	return nullptr;
}

bool UCustomGameViewportClient::RemoveSplitScreenLocalPlayer(UCustomSplitScreenLocalPlayer* InLocalPlayer)
{
	if (!GetWorld())
	{
		return false;
	}

	if (SplitScreenLocalPlayers.Remove(InLocalPlayer))
	{
		if (InLocalPlayer->PlayerController == GetWorld()->GetFirstPlayerController())
		{
			InLocalPlayer->PlayerController = nullptr;
		}
		
		return GetWorld()->GetGameInstance()->RemoveLocalPlayer(InLocalPlayer);
	}

	return false;
}
