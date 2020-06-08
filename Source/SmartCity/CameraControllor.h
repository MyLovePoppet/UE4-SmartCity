// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera.h"
#include "GeoMap.h"
#include "EventGeneraterAdapter.h"

/**
 *
 */
class AEventGeneraterAdapter;
class ACamera;

class SMARTCITY_API CameraControllor
{
public:
	CameraControllor() = default;
	virtual ~CameraControllor() = 0;

protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACamera *CameraInScene;
	AEventGeneraterAdapter *GlobeEventGA;


	GeoMap *CorrespondingMap;

	/*Start Getters and Setters*/
public:
	ACamera* GetCameraInScene() const;
	AEventGeneraterAdapter* GetGlobeEventGeneratorAdapter();
	GeoMap* GetCorrespondingMap() const;

	void SetCameraInScene(ACamera* _ACamera);
	void SetGlobeEventGeneratorAdapter(AEventGeneraterAdapter* _AEventGeneraterAdapter);
	void SetCorrespondingMap(GeoMap* _GeoMap);


	/*Start mouse and key function*/


	/*
	 * û������Ϊ���麯��
	 */
public:
	/*
	 * ��δʵ�ּ����¼��Լ�����˫���¼�
	 */
	virtual void OnDoubleClick();
	virtual void OnKeyDown();
	virtual void OnKeyUp();
	/*
	 * ���ж����ĸ�����¼�������Ϊ��������CameraControllerִ�ж�Ӧ�Ĳ���
	 * �п��������������м���
	 */
	virtual void OnMouseDown(FKey MouseKey, FVector2D MousePosition);
	virtual void OnMouseUp(FKey MouseKey, FVector2D MousePosition);
	/*
	 * �����ֺ��ƶ����������ж��¼���ֱ�ӽ�Axis��Ϊ��������CameraControllerִ�ж�Ӧ�Ĳ�������
	 */
	virtual void OnMouseMove(float Axis, FKey MouseKey, FVector2D MousePosition);
	virtual void OnMouseScrollWheelUpPress(float Axis, FVector2D MousePosition);
};
