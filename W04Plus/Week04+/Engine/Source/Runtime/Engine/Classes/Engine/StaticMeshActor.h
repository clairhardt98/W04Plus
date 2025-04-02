#pragma once
#include "GameFramework/Actor.h"


class AStaticMeshActor : public AActor
{
    DECLARE_CLASS(AStaticMeshActor, AActor)

public:
    AStaticMeshActor();

    UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

    virtual UObject* DuplicateObject(const FObjectDuplicationParameters& Params) const override;
    virtual void DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const override;

private:
    UStaticMeshComponent* StaticMeshComponent = nullptr;
};
