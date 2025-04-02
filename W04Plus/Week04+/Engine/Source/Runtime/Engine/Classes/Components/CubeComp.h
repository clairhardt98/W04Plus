#pragma once
#include "StaticMeshComponent.h"

class UCubeComp : public UStaticMeshComponent
{
    DECLARE_CLASS(UCubeComp, UStaticMeshComponent)

public:
    UCubeComp();
    virtual ~UCubeComp() override;

    UObject* Duplicate(UObject* Outer, UClass* ClassInfo) override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
};
