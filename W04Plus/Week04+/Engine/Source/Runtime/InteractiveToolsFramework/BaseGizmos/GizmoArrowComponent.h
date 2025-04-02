#pragma once
#include "GizmoBaseComponent.h"
#include "UObject/ObjectTypes.h"

class UGizmoArrowComponent : public UGizmoBaseComponent
{
    DECLARE_CLASS(UGizmoArrowComponent, UGizmoBaseComponent)

public:
    UGizmoArrowComponent();
    virtual ~UGizmoArrowComponent() override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;

    virtual UObject* DuplicateObject(const FObjectDuplicationParameters& Params) const override;
    virtual void DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const override;

private:
    ARROW_DIR Dir;

public:
    ARROW_DIR GetDir() const { return Dir; }
    void SetDir(ARROW_DIR _Dir) { Dir = _Dir; }
};
