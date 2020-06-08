// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyModeCameraControllor.h"
#include "Kismet/GameplayStatics.h"

bool CameraManipulator::LineSphereFirstIntersect(const FVector lineOri, const FVector lineDir, const FVector sphereOri, const float radius, FVector &intersectPt)
{
	//�������ĵ�ֱ�߾��룬������������ģ����ཻ
	float distToLine = FMath::PointDistToLine(sphereOri, lineDir, lineOri);
	if (distToLine > radius)
		return false;

	//����������㵽���ľ��룬��С�ڰ뾶�����������ڲ�������false
	float distToSphereOri = FVector::Dist(lineOri, sphereOri);
	if (distToSphereOri <= radius)
		return false;

	//�������巽�̡����߷��̣���⽻��
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
	/*�ⲿ���Ѿ���ACamera�����ڽ��г�ʼ����*/
}

FlyModeCameraControllor::~FlyModeCameraControllor()
{

}
//������Ļ�����ȡ���������Լ�����
bool FlyModeCameraControllor::ScreenCursorInfoToWorld(const FVector2D screenCursorPt, FVector& WorldPt,
	FVector& WorldDir)
{
	return UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this->CameraInScene->GetCameraComponent(), 0)
		, screenCursorPt, WorldPt, WorldDir);

}
//���ץȡ��ת
void FlyModeCameraControllor::DragRotate(const FVector2D inCursorPt, const FVector2D inNextCursorPt)
{
	//��ȡ������ü�����Ӧ��ue����ϵ��
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
//��ȡ��Ļ�ϵ���ڵ�������3D����
bool FlyModeCameraControllor::CursorPointOnEarth(FVector2D CursorPoint, FVector& intersectPt)
{
	FVector worldPt;
	FVector worldDirection;
	ScreenCursorInfoToWorld(CursorPoint, worldPt, worldDirection);

	return CameraManipulator::LineSphereFirstIntersect(worldPt, worldDirection,
		this->CameraInScene->GetActorLocation(), this->CameraInScene->GetEarthRadius(), intersectPt);

}
//�㵽���ķ���
FVector FlyModeCameraControllor::GetPointToCenterVector(FVector TouchPoint)
{
	if (!CameraInScene->GetEarthActor())
	{
		return FVector::ZeroVector;
	}
	FVector Axis = (CameraInScene->GetEarthActor()->GetActorLocation() - TouchPoint).GetSafeNormal();
	return Axis;
}
//��MidHoldAxis������AngleDeg�Ƕ�
void FlyModeCameraControllor::RotateEarthByAxis(float AngleDeg)
{

	if (!this->CameraInScene->GetEarthActor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Error!!! EarthActor is NULL��"));
		return;
	}

	if (FMath::Abs(AngleDeg) < KINDA_SMALL_NUMBER)
		return;
	FQuat quat = FQuat(MidHoldAxis, AngleDeg);
	CameraInScene->GetEarthActor()->SetActorRotation(quat * CameraInScene->GetEarthActor()->GetActorRotation().Quaternion());
}
/*
 * ͨ���������갴���Լ����λ������ɶ�Ӧ�Ĺ���
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

		//��ȡ��Ļ�����Ӧ�ĵ�����3D����
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
 * ͨ���������갴���Լ����λ������ɶ�Ӧ�Ĺ��ܣ�����MouseKey����Ϊ��ǰ���µ���꣬���ݰ��µ�����¼����ж�Ӧ�ĺ�������
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
		//����X����仯��תΪ������
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
		//����Ҫ����oldCursorPt
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
//���ţ�������������Ļ�ϵ����λ�ò��䡣
void FlyModeCameraControllor::Zoom(float Axis)
{
	USpringArmComponent *SpringArm = CameraInScene->GetSpringArmComponent();
	float EarthRadius = CameraInScene->GetEarthRadius();
	AActor *EarthActor = CameraInScene->GetEarthActor();
	if (SpringArm->TargetArmLength <= EarthRadius * 1.01f && Axis > 0)
		return;
	if (SpringArm->TargetArmLength >= EarthRadius * 1.80f && Axis < 0)
		return;
	//Ԥ�ȼ�����Ŵ�/��С��������������λ�ã���Ļ����ƽ�У�worldDirection���䣩 
	float DeltaLength = (Axis * (SpringArm->TargetArmLength - EarthRadius) * 0.2f);
	
	FVector worldPt;
	FVector worldDirection;
	ScreenCursorInfoToWorld(oldCursorPt, worldPt, worldDirection);
	FVector intersectPt;
	
	//�Ŵ�/��С����Ļ��������򽻵�
	if (!CameraManipulator::LineSphereFirstIntersect(
		worldPt + DeltaLength * GetPointToCenterVector(CameraInScene->GetCameraComponent()->GetComponentLocation()),
		worldDirection, EarthActor->GetActorLocation(), EarthRadius,
		intersectPt))
		return;
	FVector Next = -GetPointToCenterVector(intersectPt);
	
	//��ǰλ����Ļ��������򽻵�
	if (!CameraManipulator::LineSphereFirstIntersect(worldPt,
		worldDirection, EarthActor->GetActorLocation(), EarthRadius,
		intersectPt))
		return;
	FVector Now = -GetPointToCenterVector(intersectPt);
	
	//��������������ĵ������������ת�ǡ�
	FQuat DeltaAngle = FQuat::FindBetweenNormals(Now, Next);
	//ִ����ת������
	EarthActor->SetActorRotation(DeltaAngle * EarthActor->GetActorRotation().Quaternion());
	SpringArm->TargetArmLength -= DeltaLength;
}
