#include "GizmoRectangleComponent.h"

#include "World.h"
#include "Actors/Player.h"
#include "LevelEditor/SLevelEditor.h"
#include "Math/JungleMath.h"
#include "UnrealEd/EditorViewportClient.h"

REGISTER_CLASS(UGizmoRectangleComponent, UGizmoBaseComponent)

UGizmoRectangleComponent::UGizmoRectangleComponent()
{
}

UGizmoRectangleComponent::~UGizmoRectangleComponent()
{
}

void UGizmoRectangleComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void UGizmoRectangleComponent::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
}

UObject* UGizmoRectangleComponent::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);

    DuplicateProperties(NewObject, Params);
    return NewObject;

}

void UGizmoRectangleComponent::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);

    UGizmoRectangleComponent* NewComponent = static_cast<UGizmoRectangleComponent*>(NewObject);
    if (!NewComponent)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not UGizmoRectangleComponent");
        return;
    }
}
