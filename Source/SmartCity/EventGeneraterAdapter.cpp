// Fill out your copyright notice in the Description page of Project Settings.


#include "EventGeneraterAdapter.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "FlyModeCameraControllor.h"


// Sets default values
AEventGeneraterAdapter::AEventGeneraterAdapter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("EventGARoot");
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	CameraMode = EnumCameraMode::EnumCameraModeFly;
	InitCameraState();
}

AEventGeneraterAdapter::~AEventGeneraterAdapter()
{

}

// Called when the game starts or when spawned
/*
 * 根据不同的CameraMode来初始化Camera的状态等。
 */
void AEventGeneraterAdapter::BeginPlay()
{
	Super::BeginPlay();
	CameraControllorInScene->SetCameraInScene(this->Camera);
	//this->Camera->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	CameraControllorInScene->SetGlobeEventGeneratorAdapter(this);
	if (GetWorld())
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	if (GetWorld())
		GetWorld()->GetFirstPlayerController()->SetViewTarget(this->Camera);
	//InitCameraState();
}

// Called every frame
void AEventGeneraterAdapter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CameraControllorInScene->GetCameraInScene()->UpdateParameters();
}

// Called to bind functionality to input
/**
 * 还未实现：double click
 */
/*
 * 先使用Anykey，再在各个事件处理内判断是哪个按键按下
 */
void AEventGeneraterAdapter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("KeyDown", EKeys::AnyKey));
	PlayerInputComponent->BindAction("KeyDown", IE_Pressed, this, &AEventGeneraterAdapter::KeyDownAction);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("KeyUp", EKeys::AnyKey));
	PlayerInputComponent->BindAction("KeyUp", IE_Released, this, &AEventGeneraterAdapter::KeyUpActon);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MouseDown", EKeys::AnyKey));
	PlayerInputComponent->BindAction("MouseDown", IE_Pressed, this, &AEventGeneraterAdapter::MouseDownAction);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("MouseUp", EKeys::AnyKey));
	PlayerInputComponent->BindAction("MouseUp", IE_Released, this, &AEventGeneraterAdapter::MouseUpAction);

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MouseMove", EKeys::MouseX));
	PlayerInputComponent->BindAxis("MouseMove", this, &AEventGeneraterAdapter::MouseMove);
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MouseMove", EKeys::MouseY));
	PlayerInputComponent->BindAxis("MouseMove", this, &AEventGeneraterAdapter::MouseMove);
	
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MouseScrollMove", EKeys::MouseWheelAxis));
	PlayerInputComponent->BindAxis("MouseScrollMove", this, &AEventGeneraterAdapter::MouseScrollWheelUp);
}

EnumCameraMode AEventGeneraterAdapter::GetCameraMode() const
{
	return CameraMode;
}

CameraControllor* AEventGeneraterAdapter::GetCameraControllorInScene() const
{
	return CameraControllorInScene.Get();
}

void AEventGeneraterAdapter::SetCameraMode(EnumCameraMode _EnumCameraMode)
{
	this->CameraMode = _EnumCameraMode;
	InitCameraState();
}

void AEventGeneraterAdapter::SetCameraControllorInScene(CameraControllor* _CameraControllor)
{
	//this->CameraControllorInScene = std::shared_ptr<CameraControllor>(_CameraControllor);
	this->CameraControllorInScene = MakeShareable(_CameraControllor);
}

void AEventGeneraterAdapter::UpdateCameraMode(EnumCameraMode NewCameraMode)
{
	this->CameraMode = NewCameraMode;
	InitCameraState();
}

void AEventGeneraterAdapter::KeyDownAction()
{
	//GEngine->AddOnScreenDebugMessage(0,1.0f, FColor::Red, TEXT("123"));
	if (CameraControllorInScene)
		CameraControllorInScene->OnKeyDown();

}

void AEventGeneraterAdapter::KeyUpActon()
{
	if (CameraControllorInScene)
		CameraControllorInScene->OnKeyUp();
}
/*
 * 先判断是哪个按键按下，再作为参数传入对应的处理函数
 */
void AEventGeneraterAdapter::MouseDownAction()
{
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Blue, TEXT("Hello"));
	if (!CameraControllorInScene)
		return;
	/*使用PlayerController->IsInputKeyDown()判断是不是给定的按键按下*/
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	if (PlayerController->WasInputKeyJustPressed(EKeys::LeftMouseButton))
		CameraControllorInScene->OnMouseDown(EKeys::LeftMouseButton, MousePosition);
	if (PlayerController->WasInputKeyJustPressed(EKeys::MiddleMouseButton))
		CameraControllorInScene->OnMouseDown(EKeys::MiddleMouseButton, MousePosition);
	if(PlayerController->WasInputKeyJustPressed(EKeys::RightMouseButton))
		CameraControllorInScene->OnMouseDown(EKeys::RightMouseButton, MousePosition);
}

void AEventGeneraterAdapter::MouseUpAction()
{
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, "On GA UP");
	if (!CameraControllorInScene)
		return;
	/*使用PlayerController->IsInputKeyDown()判断是不是给定的按键按下*/
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	if (PlayerController->WasInputKeyJustReleased(EKeys::LeftMouseButton))
		CameraControllorInScene->OnMouseUp(EKeys::LeftMouseButton, MousePosition);
	if (PlayerController->WasInputKeyJustReleased(EKeys::MiddleMouseButton))
		CameraControllorInScene->OnMouseUp(EKeys::MiddleMouseButton, MousePosition);
	if (PlayerController->WasInputKeyJustReleased(EKeys::RightMouseButton))
		CameraControllorInScene->OnMouseUp(EKeys::RightMouseButton, MousePosition);
}

void AEventGeneraterAdapter::MouseMove(float Axis)
{
	if (!CameraControllorInScene)
		return;
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	FKey Key;
	if (PlayerController->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		Key = EKeys::LeftMouseButton;
		CameraControllorInScene->OnMouseMove(Axis, Key, MousePosition);
	}
		
	if (PlayerController->IsInputKeyDown(EKeys::MiddleMouseButton))
	{
		Key = EKeys::MiddleMouseButton;
		CameraControllorInScene->OnMouseMove(Axis, Key, MousePosition);
	}
		
	if (PlayerController->IsInputKeyDown(EKeys::RightMouseButton))
	{
		Key = EKeys::RightMouseButton;
		CameraControllorInScene->OnMouseMove(Axis, Key, MousePosition);
	}	
}

void AEventGeneraterAdapter::MouseScrollWheelUp(float Axis)
{
	if (!CameraControllorInScene)
		return;
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	CameraControllorInScene->OnMouseScrollWheelUpPress(Axis, MousePosition);
}

void AEventGeneraterAdapter::InitCameraState()
{
	//只有在CameraController为空或者是CameraMode发生改变时才修改CameraState
	if (CameraControllorInScene != nullptr&&CameraControllorInScene->GetCameraInScene()->GetCameraMode() == CameraMode)
		return;
	switch (CameraMode)
	{
	case EnumCameraMode::EnumCameraModeFly:
		//CameraControllorInScene = std::make_shared<FlyModeCameraControllor>();
		CameraControllorInScene = MakeShareable(new FlyModeCameraControllor());

		break;

	case EnumCameraMode::EnumCameraModeFlyTo:

		break;

	case EnumCameraMode::EnumCameraModeGround:

		break;

	default:
		break;
	}
	if (CameraControllorInScene->GetCameraInScene())
		return;
	//设置一份Camera内的Mode进行备份
	CameraControllorInScene->GetCameraInScene()->SetCameraMode(CameraMode);
	//CameraControllorInScene->SetGlobeEventGeneratorAdapter(this);
}

