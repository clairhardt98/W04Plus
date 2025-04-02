#include "SkySphereComponent.h"

#include "World.h"
#include "Engine/Source/Runtime/Core/Math/JungleMath.h"
#include "LevelEditor/SLevelEditor.h"
#include "PropertyEditor/ShowFlags.h"
#include "UnrealEd/EditorViewportClient.h"
#include "UnrealEd/PrimitiveBatch.h"
#include "UObject/Casts.h"

REGISTER_CLASS(USkySphereComponent, UStaticMeshComponent)
USkySphereComponent::USkySphereComponent()
{
    SetType(StaticClass()->GetName());
}

USkySphereComponent::~USkySphereComponent()
{
}

void USkySphereComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void USkySphereComponent::TickComponent(float DeltaTime)
{
    UOffset += 0.005f;
    VOffset += 0.005f;
    Super::TickComponent(DeltaTime);
}

UObject* USkySphereComponent::Duplicate(UObject* Outer, UClass* ClassInfo)
{
    USkySphereComponent* NewComponent = Cast<USkySphereComponent>(Super::Duplicate(Outer, ClassInfo));
    NewComponent->UOffset = UOffset;
    NewComponent->VOffset = VOffset;
    return NewComponent;
}
