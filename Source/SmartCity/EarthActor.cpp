// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthActor.h"

// Sets default values
AEarthActor::AEarthActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EarthRoot"));
	RootComponent->SetMobility(EComponentMobility::Movable);
	UStaticMeshComponent *SphereMesh;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	//SphereMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	SphereMesh->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AEarthActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEarthActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
