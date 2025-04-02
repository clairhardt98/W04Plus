#include "Components/SceneComponent.h"
#include "World.h"
#include "Math/JungleMath.h"
#include "UObject/ObjectFactory.h"
#include "UTextUUID.h"

REGISTER_CLASS(USceneComponent, UActorComponent)
USceneComponent::USceneComponent() :RelativeLocation(FVector(0.f, 0.f, 0.f)), RelativeRotation(FVector(0.f, 0.f, 0.f)), RelativeScale3D(FVector(1.f, 1.f, 1.f))
{
}

USceneComponent::~USceneComponent()
{
	if (uuidText) delete uuidText;
}
void USceneComponent::InitializeComponent()
{
    Super::InitializeComponent();

}

void USceneComponent::TickComponent(float DeltaTime)
{
	Super::TickComponent(DeltaTime);
}


int USceneComponent::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    int nIntersections = 0;
    return nIntersections;
}

FVector USceneComponent::GetForwardVector()
{
	FVector Forward = FVector(1.f, 0.f, 0.0f);
	Forward = JungleMath::FVectorRotate(Forward, QuatRotation);
	return Forward;
}

FVector USceneComponent::GetRightVector()
{
	FVector Right = FVector(0.f, 1.f, 0.0f);
	Right = JungleMath::FVectorRotate(Right, QuatRotation);
	return Right;
}

FVector USceneComponent::GetUpVector()
{
	FVector Up = FVector(0.f, 0.f, 1.0f);
	Up = JungleMath::FVectorRotate(Up, QuatRotation);
	return Up;
}


void USceneComponent::AddLocation(FVector _added)
{
	RelativeLocation = RelativeLocation + _added;

}

void USceneComponent::AddRotation(FVector _added)
{
	RelativeRotation = RelativeRotation + _added;

}

void USceneComponent::AddScale(FVector _added)
{
	RelativeScale3D = RelativeScale3D + _added;

}

UObject* USceneComponent::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);
    DuplicateProperties(NewObject, Params);
    return NewObject;
}

void USceneComponent::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);
    USceneCompoonent* NewSceneComp = static_cast<USceneComponent*>(NewObject);
    USceneComponent* TempComp = new USceneComponent();
    TempComp->bHasBeenInitialized = NewObject->bHasBeenInitialized;
    TempComp->bHasBegunPlay = NewObject->bHasBegunPlay;
    TempComp->bIsBeingDestyed = NewObject->bIsBeingDestroyed;
    TempComp->bIsActive = NewObject->bIsActive;
    TempComp->bAutoActive = NewObject->bAutoActive;

    TempComp->SetOwner(NewSceneComp->GetOwner());
    TempComp->

    if (!TempComp)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not USceneComponent");
        return;
    }

    // 트랜스폼 정보 복사 (값 형식이므로 단순 대입)
    TempComp->RelativeLocation = NewObject->RelativeLocation;
    TempComp->RelativeRotation = NewObject->RelativeRotation;
    TempComp->QuatRotation = NewObject->QuatRotation;
    TempComp->RelativeScale3D = NewObject->RelativeScale3D;

    // AttachParent는 보통 복제 과정에서 외부에서 재설정하도록 하거나 복제하지 않습니다.
    NewSceneComp->AttachParent = nullptr;

    // 자식 컴포넌트 복제
    NewSceneComp->AttachChildren.Empty();
    if (Params.bDeepCopy)
    {
        for (USceneComponent* Child : AttachChildren)
        {
            if (Child)
            {
                FObjectDuplicationParameters ChildParams(
                    Child,
                    NewSceneComp, 
                    NAME_None,
                    Params.DuplicationFlags,
                    Params.bDeepCopy,
                    Params.DuplicationMap ? Params.DuplicationMap : nullptr
                );
                USceneComponent* NewChild = static_cast<USceneComponent*>(Child->DuplicateObject(ChildParams));
                if (NewChild)
                {
                    NewSceneComp->AttachChildren.Add(NewChild);
                    NewChild->AttachParent = NewSceneComp;
                }
            }
        }
    }
    else
    {
        // 얕은 복사의 경우 원본의 자식 포인터를 그대로 대입합니다.
        NewSceneComp->AttachChildren = this->AttachChildren;
    }
}

FVector USceneComponent::GetWorldRotation()
{
	if (AttachParent)
	{
		return FVector(AttachParent->GetLocalRotation() + GetLocalRotation());
	}
	else
		return GetLocalRotation();
}

FVector USceneComponent::GetWorldScale()
{
	if (AttachParent)
	{
		return FVector(AttachParent->GetWorldScale() + GetLocalScale());
	}
	else
		return GetLocalScale();
}

FVector USceneComponent::GetWorldLocation()
{
	if (AttachParent)
	{
		return FVector(AttachParent->GetWorldLocation() + GetLocalLocation());
	}
	else
		return GetLocalLocation();
}

FVector USceneComponent::GetLocalRotation()
{
	return JungleMath::QuaternionToEuler(QuatRotation);
}

void USceneComponent::SetRotation(FVector _newRot)
{
	RelativeRotation = _newRot;
	QuatRotation = JungleMath::EulerToQuaternion(_newRot);
}

void USceneComponent::SetupAttachment(USceneComponent* InParent)
{
    if (
        InParent != AttachParent                                  // 설정하려는 Parent가 기존의 Parent와 다르거나
        && InParent != this                                       // InParent가 본인이 아니고
        && InParent != nullptr                                    // InParent가 유효한 포인터 이며
        && (
            AttachParent == nullptr                               // AttachParent도 유효하며
            || !AttachParent->AttachChildren.Contains(this)  // 이미 AttachParent의 자식이 아닌 경우
        ) 
    ) {
        AttachParent = InParent;
        InParent->AttachChildren.AddUnique(this);
    }
}