// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyModeCameraControllor.h"
#include "Kismet/GameplayStatics.h"

bool CameraManipulator::LineSphereFirstIntersect(const FVector lineOri, const FVector lineDir, const FVector sphereOri, const float radius, FVector &intersectPt)
{
	//计算球心到直线距离，若距离大于球心，不相交
	float distToLine = FMath::PointDistToLine(sphereOri, lineDir, lineOri);
	if (distToLine > radius)
		return false;

	//计算射线起点到球心距离，若小于半径，代表在球内部，返回false
	float distToSphereOri = FVector::Dist(lineOri, sphereOri);
	if (distToSphereOri <= radius)
		return false;

	//联立球体方程、射线方程，求解交点
	FVector unitLineDir = lineDir.GetSafeNormal();
	FVector sphereOriToLineOriV = lineOri - sphereOri;
	float a = FVector::DotProduct(unitLineDir, unitLineDir);
	float b = 2 * FVector::DotProduct(unitLineDir, sphereOriToLineOriV);
	float c = FVector::DotProduct(sphereOriToLineOriV, sphereOriToLineOriV) - radius * radius;
	float b2_4ac = b * b - 4 * a*c;

	if (b2_4ac == 0)
	{
		float t = (b*-1.f) / (2 * a);
		intersectPt = lineOri + lineDir * t;
		return true;
	}
	else if (b2_4ac > 0)
	{
		float t1 = (b*-1.f + FMath::Sqrt(b2_4ac)) / (2 * a);
		float t2 = (b*-1.f - FMath::Sqrt(b2_4ac)) / (2 * a);

		FVector t1_pt = lineOri + lineDir * t1;
		FVector t2_pt = lineOri + lineDir * t2;

		float t1_dist = FVector::Dist(lineOri, t1_pt);
		float t2_dist = FVector::Dist(lineOri, t2_pt);

		intersectPt = (t1_dist > t2_dist) ? t2_pt : t1_pt;
		return true;
	}
	return false;
}



FlyModeCameraControllor::FlyModeCameraControllor()
{
	/*这部分已经在ACamera的类内进行初始化了*/
}

FlyModeCameraControllor::~FlyModeCameraControllor()
{

}
//根据屏幕坐标获取世界坐标以及方向
bool FlyModeCameraControllor::ScreenCursorInfoToWorld(const FVector2D screenCursorPt, FVector& WorldPt,
	FVector& WorldDir)
{
	return UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this->CameraInScene->GetCameraComponent(), 0)
		, screenCursorPt, WorldPt, WorldDir);

}
//鼠标抓取旋转
void FlyModeCameraControllor::DragRotate(const FVector2D inCursorPt, const FVector2D inNextCursorPt)
{
	//获取相机近裁剪面点对应的ue坐标系点
	FVector intersectPt;
	FVector nextIntersectPt;

	FVector EarthLocation = this->CameraInScene->GetActorLocation();

	if (CursorPointOnEarth(inCursorPt, intersectPt) &&
		CursorPointOnEarth(inNextCursorPt, nextIntersectPt))
	{
		FVector centerToIntersectVec = intersectPt - EarthLocation;
		FVector centerToNextIntersectVec = nextIntersectPt - EarthLocation;
		FQuat DeltaQuat = FQuat::FindBetween(centerToIntersectVec, centerToNextIntersectVec);
		FQuat TargetQuat = DeltaQuat * CameraInScene->GetEarthActor()->GetActorRotation().Quaternion();
		CameraInScene->GetEarthActor()->SetActorRotation(TargetQuat);
	}
}
//获取屏幕上点击在地球表面的3D坐标
bool FlyModeCameraControllor::CursorPointOnEarth(FVector2D CursorPoint, FVector& intersectPt)
{
	FVector worldPt;
	FVector worldDirection;
	ScreenCursorInfoToWorld(CursorPoint, worldPt, worldDirection);

	return CameraManipulator::LineSphereFirstIntersect(worldPt, worldDirection,
		this->CameraInScene->GetActorLocation(), this->CameraInScene->GetEarthRadius(), intersectPt);

}
//点到地心方向
FVector FlyModeCameraControllor::GetPointToCenterVector(FVector TouchPoint)
{
	if (!CameraInScene->GetEarthActor())
	{
		return FVector::ZeroVector;
	}
	FVector Axis = (CameraInScene->GetEarthActor()->GetActorLocation() - TouchPoint).GetSafeNormal();
	return Axis;
}
//绕MidHoldAxis轴自旋AngleDeg角度
void FlyModeCameraControllor::RotateEarthByAxis(float AngleDeg)
{

	if (!this->CameraInScene->GetEarthActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Error!!! EarthActor is NULL！"));
		return;
	}

	if (FMath::Abs(AngleDeg) < KINDA_SMALL_NUMBER)
		return;
	FQuat quat = FQuat(MidHoldAxis, AngleDeg);
	CameraInScene->GetEarthActor()->SetActorRotation(quat * CameraInScene->GetEarthActor()->GetActorRotation().Quaternion());
}
/*
 * 通过传入的鼠标按键以及鼠标位置来完成对应的功能
 */
void FlyModeCameraControllor::OnMouseDown(FKey MouseKey, FVector2D MousePosition)
{
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, "Down");
	if (MouseKey == EKeys::LeftMouseButton)
	{
		this->oldCursorPt = MousePosition;
	}
	if (MouseKey == EKeys::MiddleMouseButton)
	{
		this->oldCursorPt = MousePosition;
		FVector intersectPt = FVector::ZeroVector;
		CursorPointOnEarth(oldCursorPt, intersectPt);

		//获取屏幕坐标对应的地球上3D坐标
		MidHoldAxis = GetPointToCenterVector(intersectPt);
		
	}
	if(MouseKey==EKeys::RightMouseButton)
	{
		oldCursorPt = MousePosition;
	}

}

void FlyModeCameraControllor::OnMouseUp(FKey MouseKey, FVector2D MousePosition)
{

}
/*
 * 通过传入的鼠标按键以及鼠标位置来完成对应的功能，其中MouseKey参数为当前按下的鼠标，根据按下的鼠标事件进行对应的函数操作
 */
void FlyModeCameraControllor::OnMouseMove(float Axis, FKey MouseKey, FVector2D MousePosition)
{
	
	if (MouseKey==EKeys::LeftMouseButton)
	{
		if (FMath::Abs(Axis) <= KINDA_SMALL_NUMBER)
			return;
		currentCursorPt = MousePosition;
		DragRotate(oldCursorPt, currentCursorPt);
		oldCursorPt = currentCursorPt;
	}
	if (MouseKey == EKeys::MiddleMouseButton)
	{
		//GEngine->GetWorld()->GetFirstPlayerController()->GetMousePosition(currentCursorPt.X, currentCursorPt.Y);
		currentCursorPt = MousePosition;
		//鼠标的X坐标变化率转为度数。
		uint32 SizeX = GEngine->GameViewport->Viewport->GetSizeXY().X;
		float AngleDegX = (currentCursorPt.X - oldCursorPt.X) * 5.f / SizeX;
		RotateEarthByAxis(AngleDegX);
		oldCursorPt = currentCursorPt;
	}
	if(MouseKey==EKeys::RightMouseButton)
	{
		uint32 SizeX = GEngine->GameViewport->Viewport->GetSizeXY().X;
		currentCursorPt = MousePosition;
		float SpeedY = (currentCursorPt.Y - oldCursorPt.Y) / SizeX;
		Zoom(SpeedY);
		//不需要更新oldCursorPt
	}
}
 
void FlyModeCameraControllor::OnMouseScrollWheelUpPress(float Axis, FVector2D MousePosition)
{
	if (FMath::Abs(Axis) > KINDA_SMALL_NUMBER)
	{
		oldCursorPt = MousePosition;
		Zoom(Axis);
	}
}
//缩放，鼠标与地球在屏幕上的相对位置不变。
void FlyModeCameraControllor::Zoom(float Axis)
{
	USpringArmComponent *SpringArm = CameraInScene->GetSpringArmComponent();
	float EarthRadius = CameraInScene->GetEarthRadius();
	AActor *EarthActor = CameraInScene->GetEarthActor();
	if (SpringArm->TargetArmLength <= EarthRadius * 1.01f && Axis > 0)
		return;
	if (SpringArm->TargetArmLength >= EarthRadius * 1.80f && Axis < 0)
		return;
	//预先计算出放大/缩小后，摄像机将到达的位置，屏幕射线平行（worldDirection不变） 
	float DeltaLength = (Axis * (SpringArm->TargetArmLength - EarthRadius) * 0.2f);
	
	FVector worldPt;
	FVector worldDirection;
	ScreenCursorInfoToWorld(oldCursorPt, worldPt, worldDirection);
	FVector intersectPt;
	
	//放大/缩小后，屏幕射线与地球交点
	if (!CameraManipulator::LineSphereFirstIntersect(
		worldPt + DeltaLength * GetPointToCenterVector(CameraInScene->GetCameraComponent()->GetComponentLocation()),
		worldDirection, EarthActor->GetActorLocation(), EarthRadius,
		intersectPt))
		return;
	FVector Next = -GetPointToCenterVector(intersectPt);
	
	//当前位置屏幕射线与地球交点
	if (!CameraManipulator::LineSphereFirstIntersect(worldPt,
		worldDirection, EarthActor->GetActorLocation(), EarthRadius,
		intersectPt))
		return;
	FVector Now = -GetPointToCenterVector(intersectPt);
	
	//根据两交点与地心的向量，求出旋转角。
	FQuat DeltaAngle = FQuat::FindBetweenNormals(Now, Next);
	//执行旋转和缩放
	EarthActor->SetActorRotation(DeltaAngle * EarthActor->GetActorRotation().Quaternion());
	SpringArm->TargetArmLength -= DeltaLength;
}
