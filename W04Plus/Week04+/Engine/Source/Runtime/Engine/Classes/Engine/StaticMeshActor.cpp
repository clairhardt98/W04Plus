#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

REGISTER_CLASS(AStaticMeshActor, AActor)
AStaticMeshActor::AStaticMeshActor()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>();
    RootComponent = StaticMeshComponent;
}
