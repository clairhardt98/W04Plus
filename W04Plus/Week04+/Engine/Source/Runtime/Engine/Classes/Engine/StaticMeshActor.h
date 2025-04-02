#pragma once
#include "GameFramework/Actor.h"


class AStaticMeshActor : public AActor
{
    DECLARE_CLASS(AStaticMeshActor, AActor)

public:
    AStaticMeshActor();

    UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

public:
    virtual UObject* Duplicate(UObject* Outer, UClass* ClassInfo)override;

private:
    UStaticMeshComponent* StaticMeshComponent = nullptr;
};
