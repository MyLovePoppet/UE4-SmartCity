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
	 * 没有设置为纯虚函数
	 */
public:
	/*
	 * 暂未实现键盘事件以及鼠标的双击事件
	 */
	virtual void OnDoubleClick();
	virtual void OnKeyDown();
	virtual void OnKeyUp();
	/*
	 * 先判断是哪个鼠标事件，再作为参数传入CameraController执行对应的操作
	 * 有可能是鼠标左键，中键等
	 */
	virtual void OnMouseDown(FKey MouseKey, FVector2D MousePosition);
	virtual void OnMouseUp(FKey MouseKey, FVector2D MousePosition);
	/*
	 * 鼠标滚轮和移动操作无需判断事件，直接将Axis作为参数传入CameraController执行对应的操作即可
	 */
	virtual void OnMouseMove(float Axis, FKey MouseKey, FVector2D MousePosition);
	virtual void OnMouseScrollWheelUpPress(float Axis, FVector2D MousePosition);
};
