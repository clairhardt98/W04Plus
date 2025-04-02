#include "LightComponent.h"
#include "UBillboardComponent.h"
#include "Math/JungleMath.h"
#include "UnrealEd/PrimitiveBatch.h"
#include "Engine/Classes/GameFramework/Actor.h"
REGISTER_CLASS(ULightComponentBase, USceneComponent)

ULightComponentBase::ULightComponentBase()
{
    // FString name = "SpotLight";
    // SetName(name);
    
}


ULightComponentBase::~ULightComponentBase()
{
}
void ULightComponentBase::InitializeComponent()
{
    InitializeLight();
}
void ULightComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetOwner()->RemoveOwnedComponent(texture2D);
    texture2D = nullptr;
}
void ULightComponentBase::SetColor(FVector4 newColor)
{
    color = newColor;
}

FVector4 ULightComponentBase::GetColor() const
{
    return color;
}

float ULightComponentBase::GetRadius() const
{
    return radius;
}

void ULightComponentBase::SetRadius(float r)
{
    radius = r;
}

UObject* ULightComponentBase::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);

    DuplicateProperties(NewObject, Params);
    return NewObject;
}

void ULightComponentBase::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);

    ULightComponentBase* NewComponent = static_cast<ULightComponentBase*>(NewObject);
    if (!NewComponent)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not ULightComponentBase");
        return;
    }

    NewComponent->color = this->color;
    NewComponent->radius = this->radius;
    NewComponent->AABB = this->AABB;
    NewComponent->texture2D = this->texture2D;
}

void ULightComponentBase::InitializeLight()
{
    texture2D = GetOwner()->AddComponent<UBillboardComponent>();
    texture2D->SetTexture(L"Assets/Texture/spotLight.png");
    //texture2D->InitializeComponent();
    AABB.max = { 1.f,1.f,0.1f };
    AABB.min = { -1.f,-1.f,-0.1f };
    color = { 1,1,1,1 };
    radius = 5;
}

void ULightComponentBase::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

    texture2D->TickComponent(DeltaTime);
    texture2D->SetLocation(GetWorldLocation());

}

int ULightComponentBase::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    bool res =AABB.Intersect(rayOrigin, rayDirection, pfNearHitDistance);
    return res;
}

