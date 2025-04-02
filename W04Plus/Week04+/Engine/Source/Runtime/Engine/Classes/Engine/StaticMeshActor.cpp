#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

REGISTER_CLASS(AStaticMeshActor, AActor)
AStaticMeshActor::AStaticMeshActor()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>();
    RootComponent = StaticMeshComponent;
}

UObject* AStaticMeshActor::Duplicate(UObject* Outer, UClass* ClassInfo)
{
    AStaticMeshActor* NewActor = Cast<AStaticMeshActor>(Super::Duplicate(Outer, ClassInfo));

    NewActor->StaticMeshComponent = StaticMeshComponent;

    return NewActor;
}

