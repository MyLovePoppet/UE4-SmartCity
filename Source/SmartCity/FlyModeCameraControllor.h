// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraControllor.h"
/**
 *
 */
class FlyModeCameraControllor;

class CameraManipulator
{
public:
	static bool LineSphereFirstIntersect(const FVector lineOri, const FVector lineDir, const FVector sphereOri, const float radius, FVector &intersectPt);
};

class SMARTCITY_API FlyModeCameraControllor : public CameraControllor
{
public:
	FlyModeCameraControllor();
	~FlyModeCameraControllor();

protected:

	bool ScreenCursorInfoToWorld(const FVector2D screenCursorPt, FVector & WorldPt, FVector & WorldDir);
	void DragRotate(const FVector2D inCursorPt, const FVector2D inNextCursorPt);
	bool CursorPointOnEarth(FVector2D CursorPoint, FVector &intersectPt);
	FVector GetPointToCenterVector(FVector TouchPoint);
	void RotateEarthByAxis(float AngleDeg);
public:
	virtual void OnMouseDown(FKey MouseKey, FVector2D MousePosition) override;
	virtual void OnMouseUp(FKey MouseKey, FVector2D MousePosition) override;
	virtual void OnMouseMove(float Axis, FKey MouseKey, FVector2D MousePosition) override;
	virtual void OnMouseScrollWheelUpPress(float Axis, FVector2D MousePosition) override;

	void Zoom(float Axis);
private:
	FVector2D oldCursorPt;
	FVector2D currentCursorPt;
	//中间旋转所绕轴
	FVector MidHoldAxis;
};
