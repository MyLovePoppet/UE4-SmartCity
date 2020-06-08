// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraControllor.h"

/*
 * 初始化CameraController内部的
 */

CameraControllor::~CameraControllor()
{
	//CameraInScene->Destroy();

	//delete GeoMap;
}

ACamera* CameraControllor::GetCameraInScene() const
{
	return CameraInScene;
}

AEventGeneraterAdapter* CameraControllor::GetGlobeEventGeneratorAdapter()
{
	return GlobeEventGA;
}

GeoMap* CameraControllor::GetCorrespondingMap() const
{
	return CorrespondingMap;
}

void CameraControllor::SetCameraInScene(ACamera* _ACamera)
{
	this->CameraInScene = _ACamera;
}

void CameraControllor::SetGlobeEventGeneratorAdapter(AEventGeneraterAdapter* _AEventGeneraterAdapter)
{
	GlobeEventGA = _AEventGeneraterAdapter;
}

void CameraControllor::SetCorrespondingMap(GeoMap* _GeoMap)
{
	this->CorrespondingMap = _GeoMap;
}

void CameraControllor::OnDoubleClick()
{
}

void CameraControllor::OnKeyDown()
{
}

void CameraControllor::OnKeyUp()
{
}

void CameraControllor::OnMouseDown(FKey MouseKey, FVector2D MousePosition)
{
}

void CameraControllor::OnMouseUp(FKey MouseKey, FVector2D MousePosition)
{
}

void CameraControllor::OnMouseMove(float Axis, FKey MouseKey, FVector2D MousePosition)
{
}

void CameraControllor::OnMouseScrollWheelUpPress(float Axis, FVector2D MousePosition)
{
}


