// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EventGeneraterAdapter.h"
#include "EarthActor.h"
#include "Camera.generated.h"
enum class EnumCameraMode :uint8;
UCLASS()
class SMARTCITY_API ACamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	float FOV;
	FVector Location;
	FRotator Rotation;
	FVector UpDirection;
	float AspectRatio;
	FVector2D ScreenResolution;
	bool IsPerspectView;
	FMatrix ViewPorjectionMatrix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent *CameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent *SpringArm;

	UStaticMeshComponent *StaticMesh;
	//通过蓝图创建
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor *EarthActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EarthRadius;

	EnumCameraMode CameraMode;

public:
	float GetFOV() const;
	FVector GetLocation() const;
	FRotator GetRotation() const;
	FVector GetUpDirection() const;
	float GetAspectRatio() const;
	FVector2D GetScreenResolution() const;
	bool GetIsPerspectView() const;
	FMatrix GetViewPorjectionMatrix() const;
	UCameraComponent* GetCameraComponent() const;
	USpringArmComponent* GetSpringArmComponent() const;
	UStaticMeshComponent* GetStaticMeshComponent() const;
	AActor* GetEarthActor() const;
	float GetEarthRadius() const;
	EnumCameraMode GetCameraMode() const;
	/*Start Setters*/
public:
	void SetFOV(float _FOV);
	void SetLocation(FVector _Location);
	void SetRotation(FRotator _Rotation);
	void SetUpDirection(FVector _UpDirection);
	void SetAspectRatio(float _AspectRatio);
	void SetScreenResolution(FVector2D _ScreenResolution);
	void SetIsPerspectView(bool _IsPerspectView);
	void SetViewPorjectionMatrix(FMatrix _ViewPorjectionMatrix);
	void SetCameraComponent(UCameraComponent* _CameraComponent);
	void SetSpringArmComponent(USpringArmComponent* _SpringArmComponent);
	void SetStaticMeshComponent(UStaticMeshComponent* _StaticMeshComponent);
	void SetEarthActor(AActor* _EarthActor);
	void SetEarthRadius(float _EarthRadius);
	void SetCameraMode(EnumCameraMode _CameraMode);
public:
	void UpdateParameters();
	
	void InitCameraMode();

};
