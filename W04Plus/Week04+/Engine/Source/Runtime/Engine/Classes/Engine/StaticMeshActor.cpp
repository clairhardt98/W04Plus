#include "StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

REGISTER_CLASS(AStaticMeshActor, AActor)
AStaticMeshActor::AStaticMeshActor()
{
    StaticMeshComponent = AddComponent<UStaticMeshComponent>();
    RootComponent = StaticMeshComponent;
}

UObject* AStaticMeshActor::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);

    DuplicateProperties(NewObject, Params);
    return NewObject;

}

void AStaticMeshActor::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{

    Super::DuplicateProperties(NewObject, Params);

    AStaticMeshActor* NewActor = static_cast<AStaticMeshActor*>(NewObject);
    if (!NewActor)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not AStaticMeshActor");
        return;
    }

    if (StaticMeshComponent)
    {
        FObjectDuplicationParameters CompParams(
            StaticMeshComponent,
            NewActor,
            NAME_None,
            Params.DuplicationFlags,
            Params.bDeepCopy,
            Params.DuplicationMap ? Params.DuplicationMap : nullptr
        );

        UStaticMeshComponent* NewStaticMeshComp = static_cast<UStaticMeshComponent*>(StaticMeshComponent->DuplicateObject(CompParams));
        if (NewStaticMeshComp)
        {
            NewActor->StaticMeshComponent = NewStaticMeshComp;
        }
    }

}
