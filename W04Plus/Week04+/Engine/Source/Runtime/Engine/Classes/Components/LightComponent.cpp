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

UObject* ULightComponentBase::Duplicate(UObject* Outer, UClass* ClassInfo)
{
    ULightComponentBase* NewComponent = Cast<ULightComponentBase>(Super::Duplicate(Outer, ClassInfo));
    NewComponent->color = color;
    NewComponent->radius = radius;
    NewComponent->AABB = AABB;
    // test
    NewComponent->texture2D = texture2D;

    return NewComponent;
}

void ULightComponentBase::InitializeLight()
{
    UBillboardComponent* texture2D = GetOwner()->AddComponent<UBillboardComponent>();
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

   

}

int ULightComponentBase::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    bool res =AABB.Intersect(rayOrigin, rayDirection, pfNearHitDistance);
    return res;
}

