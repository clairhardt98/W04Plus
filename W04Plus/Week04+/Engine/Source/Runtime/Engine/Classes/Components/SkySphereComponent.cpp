#include "SkySphereComponent.h"

#include "World.h"
#include "Engine/Source/Runtime/Core/Math/JungleMath.h"
#include "LevelEditor/SLevelEditor.h"
#include "PropertyEditor/ShowFlags.h"
#include "UnrealEd/EditorViewportClient.h"
#include "UnrealEd/PrimitiveBatch.h"

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

UObject* USkySphereComponent::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);

    DuplicateProperties(NewObject, Params);
    return NewObject;
}

void USkySphereComponent::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);

    USkySphereComponent* NewComponent = static_cast<USkySphereComponent*>(NewObject);
    if (!NewComponent)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not UActorComponent");
        return;
    }

    NewComponent->UOffset = this->UOffset;
    NewComponent->VOffset = this->VOffset;
}
