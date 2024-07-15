// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\HelperTypes.h"
#include "InventoryItem.generated.h"

//Forward declarations:
class USphereComponent;

UCLASS()
class AUTOMATIONTESTSPROJ_API AInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryItem();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryData InventoryData;
	
};
