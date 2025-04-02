#include "TransformGizmo.h"
#include "GizmoArrowComponent.h"
#include "Define.h"
#include "GizmoCircleComponent.h"
#include "Actors/Player.h"
#include "GizmoRectangleComponent.h"
#include "World.h"
#include "Engine/FLoaderOBJ.h"

UTransformGizmo::UTransformGizmo()
{
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_loc_x.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_loc_y.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_loc_z.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_rot_x.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_rot_y.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_rot_z.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_scale_x.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_scale_y.obj");
    FManagerOBJ::CreateStaticMesh("Assets/gizmo_scale_z.obj");

    SetRootComponent(
        AddComponent<USceneComponent>()
    );

    UGizmoArrowComponent* locationX = AddComponent<UGizmoArrowComponent>();
    locationX->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_loc_x.obj"));
	locationX->SetupAttachment(RootComponent);
    locationX->SetGizmoType(UGizmoBaseComponent::ArrowX);
	ArrowArr.Add(locationX);

    UGizmoArrowComponent* locationY = AddComponent<UGizmoArrowComponent>();
    locationY->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_loc_y.obj"));
    locationY->SetupAttachment(RootComponent);
    locationY->SetGizmoType(UGizmoBaseComponent::ArrowY);
    ArrowArr.Add(locationY);

    UGizmoArrowComponent* locationZ = AddComponent<UGizmoArrowComponent>();
    locationZ->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_loc_z.obj"));
    locationZ->SetupAttachment(RootComponent);
    locationZ->SetGizmoType(UGizmoBaseComponent::ArrowZ);
    ArrowArr.Add(locationZ);

    UGizmoRectangleComponent* ScaleX = AddComponent<UGizmoRectangleComponent>();
    ScaleX->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_scale_x.obj"));
    ScaleX->SetupAttachment(RootComponent);
    ScaleX->SetGizmoType(UGizmoBaseComponent::ScaleX);
    RectangleArr.Add(ScaleX);

    UGizmoRectangleComponent* ScaleY = AddComponent<UGizmoRectangleComponent>();
    ScaleY->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_scale_y.obj"));
    ScaleY->SetupAttachment(RootComponent);
    ScaleY->SetGizmoType(UGizmoBaseComponent::ScaleY);
    RectangleArr.Add(ScaleY);

    UGizmoRectangleComponent* ScaleZ = AddComponent<UGizmoRectangleComponent>();
    ScaleZ->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_scale_z.obj"));
    ScaleZ->SetupAttachment(RootComponent);
    ScaleZ->SetGizmoType(UGizmoBaseComponent::ScaleZ);
    RectangleArr.Add(ScaleZ);

    UGizmoCircleComponent* CircleX = AddComponent<UGizmoCircleComponent>();
    CircleX->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_rot_x.obj"));
    CircleX->SetupAttachment(RootComponent);
    CircleX->SetGizmoType(UGizmoBaseComponent::CircleX);
    CircleArr.Add(CircleX);

    UGizmoCircleComponent* CircleY = AddComponent<UGizmoCircleComponent>();
    CircleY->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_rot_y.obj"));
    CircleY->SetupAttachment(RootComponent);
    CircleY->SetGizmoType(UGizmoBaseComponent::CircleY);
    CircleArr.Add(CircleY);

    UGizmoCircleComponent* CircleZ = AddComponent<UGizmoCircleComponent>();
    CircleZ->SetStaticMesh(FManagerOBJ::GetStaticMesh(L"gizmo_rot_z.obj"));
    CircleZ->SetupAttachment(RootComponent);
    CircleZ->SetGizmoType(UGizmoBaseComponent::CircleZ);
    CircleArr.Add(CircleZ);
}

void UTransformGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (const AActor* PickedActor = GetWorld()->GetSelectedActor())
    {
        SetActorLocation(PickedActor->GetActorLocation());
        if (GetWorld()->GetEditorPlayer()->GetCoordiMode() == CoordiMode::CDM_LOCAL)
        {
            // TODO: 임시로 RootComponent의 정보로 사용
            SetActorRotation(PickedActor->GetActorRotation());
        }
        else if (GetWorld()->GetEditorPlayer()->GetCoordiMode() == CoordiMode::CDM_WORLD)
            SetActorRotation(FVector(0.0f, 0.0f, 0.0f));
    }
}

UObject* UTransformGizmo::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);
    DuplicateProperties(this->GetClass(), Params);

    return NewObject;
}

void UTransformGizmo::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);

    UTransformGizmo* NewGizmo = Cast<UTransformGizmo>(NewObject);
    if (!NewGizmo)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not UTransformGizmo");
        return;
    }

    // ArrowArr 복제: 각 Arrow 컴포넌트를 재귀적으로 복제
    for (UStaticMeshComponent* ArrowComp : ArrowArr)
    {
        if (ArrowComp)
        {
            FObjectDuplicationParameters CompParams(
                ArrowComp,
                NewGizmo, // 새 Gizmo를 Outer로 지정하여 소유 관계 유지
                NAME_None,
                Params.DuplicationFlags,
                Params.bDeepCopy,
                Params.DuplicationMap ? Params.DuplicationMap : nullptr
            );
            UStaticMeshComponent* NewArrowComp = Cast<UStaticMeshComponent>(ArrowComp->DuplicateObject(CompParams));
            if (NewArrowComp)
            {
                NewGizmo->ArrowArr.Add(NewArrowComp);
                // 필요하다면 새 컴포넌트의 소유자도 NewGizmo로 설정
                // NewArrowComp->Owner = NewGizmo; 
            }
        }
    }

    // CircleArr 복제: 각 원형 컴포넌트 복제
    for (UStaticMeshComponent* CircleComp : CircleArr)
    {
        if (CircleComp)
        {
            FObjectDuplicationParameters CompParams(
                CircleComp,
                NewGizmo,
                NAME_None,
                Params.DuplicationFlags,
                Params.bDeepCopy,
                Params.DuplicationMap ? Params.DuplicationMap : nullptr
            );
            UStaticMeshComponent* NewCircleComp = Cast<UStaticMeshComponent>(CircleComp->DuplicateObject(CompParams));
            if (NewCircleComp)
            {
                NewGizmo->CircleArr.Add(NewCircleComp);
            }
        }
    }

    // RectangleArr 복제: 각 사각형 컴포넌트 복제
    for (UStaticMeshComponent* RectComp : RectangleArr)
    {
        if (RectComp)
        {
            FObjectDuplicationParameters CompParams(
                RectComp,
                NewGizmo,
                NAME_None,
                Params.DuplicationFlags,
                Params.bDeepCopy,
                Params.DuplicationMap ? Params.DuplicationMap : nullptr
            );
            UStaticMeshComponent* NewRectComp = Cast<UStaticMeshComponent>(RectComp->DuplicateObject(CompParams));
            if (NewRectComp)
            {
                NewGizmo->RectangleArr.Add(NewRectComp);
            }
        }
    }
}
