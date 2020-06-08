// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera.h"
#include "CameraControllor.h"
#include "EventGeneraterAdapter.generated.h"
UENUM()
enum class EnumCameraMode :uint8 {
	EnumCameraModeFly,
	EnumCameraModeFlyTo,
	EnumCameraModeGround
};
class CameraControllor;
class ACamera;
UCLASS()
class SMARTCITY_API AEventGeneraterAdapter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEventGeneraterAdapter();
	~AEventGeneraterAdapter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*������Ϊpublic��ʾ��ͼ���Է��ʣ�����תΪC++����ֱ�ӻ���private��get��set�����Ѿ�д��*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraMode")
		EnumCameraMode CameraMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraControllor")
		ACamera *Camera;
private:
	//EnumCameraMode CameraMode;
	//std::shared_ptr<CameraControllor> CameraControllorInScene;
	TSharedPtr<CameraControllor>CameraControllorInScene;

public:
	void UpdateCameraMode(EnumCameraMode NewCameraMode);





	/*Getters and Setters*/
public:
	EnumCameraMode GetCameraMode() const;
	CameraControllor *GetCameraControllorInScene() const;

	void SetCameraMode(EnumCameraMode _CameraMode);
	void SetCameraControllorInScene(CameraControllor *_CameraControllor);


	/*�������¼�������ת����CameraControllor����ִ��*/
private:
	void KeyDownAction();
	void KeyUpActon();
	void MouseDownAction();
	void MouseUpAction();
	void MouseMove(float Axis);
	void MouseScrollWheelUp(float Axis);

private:
	/*��ʼ��һЩCamera��״̬����begin play�������л�CameraMode��ʱ����е���*/
	void InitCameraState();
};

