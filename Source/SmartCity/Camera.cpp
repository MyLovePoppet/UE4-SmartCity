// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

#include "Kismet/GameplayStatics.h"



// Sets default values
ACamera::ACamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh0"));

	//SpringArm->SetupAttachment(Ev)
	SpringArm->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->SetActive(true);
	if (StaticMesh)
		StaticMesh->SetupAttachment(RootComponent);

	//EarthActor放在蓝图内进行初始化
	//EarthActor = CreateDefaultSubobject<AActor>(TEXT("Earth"));

	EarthRadius = 5000.f;
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}
void ACamera::InitCameraMode()
{
	switch (CameraMode)
	{
	case EnumCameraMode::EnumCameraModeFly:

		SpringArm->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);

		SpringArm->TargetArmLength = 8000.f;
		SpringArm->bEnableCameraLag = true;
		SpringArm->CameraLagSpeed = 6.f;
		SpringArm->bDoCollisionTest = false;

		CameraComponent->bUsePawnControlRotation = false;
		CameraComponent->FieldOfView = 120.f;
		CameraComponent->SetActive(true);

		if (StaticMesh)
		{
			//StaticMesh->bCastDynamicShadow = true;
			StaticMesh->bAffectDynamicIndirectLighting = true;
			//StaticMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
			StaticMesh->SetCollisionProfileName(TEXT("CharacterMesh"));
			StaticMesh->SetGenerateOverlapEvents(false);
			//StaticMesh->SetNotifyRigidBodyCollision(false);
		}

		break;

	case EnumCameraMode::EnumCameraModeFlyTo:

		break;

	case EnumCameraMode::EnumCameraModeGround:

		break;


	default:

		break;
	}
}
// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();
	//在开始运行时进行根据CameraMode设置对应的相机参数
	InitCameraMode();
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input
void ACamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Fill out your copyright notice in the Description page of Project Settings.

float ACamera::GetFOV() const
{
	return FOV;
}

FVector ACamera::GetLocation() const
{
	return Location;
}

FRotator ACamera::GetRotation() const
{
	return Rotation;
}

FVector ACamera::GetUpDirection() const
{
	return UpDirection;
}

float ACamera::GetAspectRatio() const
{
	return AspectRatio;
}

FVector2D ACamera::GetScreenResolution() const
{
	return ScreenResolution;
}

bool ACamera::GetIsPerspectView() const
{
	return IsPerspectView;
}

FMatrix ACamera::GetViewPorjectionMatrix() const
{
	return ViewPorjectionMatrix;
}
UCameraComponent* ACamera::GetCameraComponent() const
{
	return CameraComponent;
}

USpringArmComponent* ACamera::GetSpringArmComponent() const
{
	return SpringArm;
}

UStaticMeshComponent* ACamera::GetStaticMeshComponent() const
{
	return StaticMesh;
}

AActor* ACamera::GetEarthActor() const
{
	return EarthActor;
}

float ACamera::GetEarthRadius() const
{
	return EarthRadius;
}

EnumCameraMode ACamera::GetCameraMode() const
{
	return CameraMode;
}

void ACamera::UpdateParameters()
{
	CameraComponent->SetFieldOfView(this->FOV);
	CameraComponent->SetRelativeLocation(this->Location);
	CameraComponent->SetRelativeRotation(this->Rotation);
	CameraComponent->SetAspectRatio(this->AspectRatio);
	//UpDirection
	//IsPerspectiveView
	//ViewPorjectionMatrix

}


void ACamera::SetFOV(float _FOV)
{
	this->FOV = _FOV;
	CameraComponent->SetFieldOfView(this->FOV);
}

void ACamera::SetLocation(FVector _Location)
{
	this->Location = _Location;
	CameraComponent->SetRelativeLocation(this->Location);
}

void ACamera::SetRotation(FRotator _Rotation)
{
	this->Rotation = _Rotation;
	CameraComponent->SetRelativeRotation(this->Rotation);
}

void ACamera::SetUpDirection(FVector _UpDirection)
{
	this->UpDirection = _UpDirection;
	//
}

void ACamera::SetAspectRatio(float _AspectRatio)
{
	this->AspectRatio = _AspectRatio;
	CameraComponent->SetAspectRatio(this->AspectRatio);
}

void ACamera::SetScreenResolution(FVector2D _ScreenResolution)
{
	this->ScreenResolution = _ScreenResolution;
	//
}

void ACamera::SetIsPerspectView(bool _IsPerspectView)
{
	this->IsPerspectView = _IsPerspectView;
	//
}

void ACamera::SetViewPorjectionMatrix(FMatrix _ViewPorjectionMatrix)
{
	this->ViewPorjectionMatrix = _ViewPorjectionMatrix;
	//
}

void ACamera::SetCameraComponent(UCameraComponent* _CameraComponent)
{
	this->CameraComponent = _CameraComponent;
	//
}

void ACamera::SetSpringArmComponent(USpringArmComponent* _SpringArmComponent)
{
	this->SpringArm = _SpringArmComponent;
}

void ACamera::SetStaticMeshComponent(UStaticMeshComponent* _StaticMeshComponent)
{
	this->StaticMesh = _StaticMeshComponent;
}

void ACamera::SetEarthActor(AActor* _EarthActor)
{
	this->EarthActor = _EarthActor;
}

void ACamera::SetEarthRadius(float _EarthRadius)
{
	this->EarthRadius = _EarthRadius;
}

void ACamera::SetCameraMode(EnumCameraMode _CameraMode)
{
	this->CameraMode = _CameraMode;
}
