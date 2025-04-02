#pragma once
#include "PrimitiveComponent.h"
#include "Define.h"
class UBillboardComponent;

class ULightComponentBase : public USceneComponent
{
    DECLARE_CLASS(ULightComponentBase, USceneComponent)

public:
    ULightComponentBase();
    virtual ~ULightComponentBase() override;

    virtual void InitializeComponent() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

    virtual void TickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance);
    void InitializeLight();
    void SetColor(FVector4 newColor);
    FVector4 GetColor() const;
    float GetRadius() const;
    void SetRadius(float r);

    virtual UObject* DuplicateObject(const FObjectDuplicationParameters& Params) const override;
    virtual void DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const override;

private:
    FVector4 color;
    float radius;
    FBoundingBox AABB;

public:
    FBoundingBox GetBoundingBox() const {return AABB;}
    float GetRadius() {return radius;}
    FVector4 GetColor() {return color;}
};
