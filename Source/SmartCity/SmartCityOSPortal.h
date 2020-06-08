// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventGeneraterAdapter.h"
#include "GameFramework/Actor.h"
#include "SmartCityOSPortal.generated.h"

UCLASS()
class SMARTCITY_API ASmartCityOSPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartCityOSPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GlobeEventGenerater")
		AEventGeneraterAdapter *GlobeEventGA;
};
