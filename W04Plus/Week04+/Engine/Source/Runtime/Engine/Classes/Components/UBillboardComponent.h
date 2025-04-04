#pragma once
#include "PrimitiveComponent.h"
#include "UTexture.h"

class UBillboardComponent : public UPrimitiveComponent
{
    DECLARE_CLASS(UBillboardComponent, UPrimitiveComponent)
    
public:
    UBillboardComponent();
    virtual ~UBillboardComponent() override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(
        FVector& rayOrigin,
        FVector& rayDirection, float& pfNearHitDistance
    ) override;

    void SetTexture(FWString _fileName);
    void SetUUIDParent(USceneComponent* _parent);
    FMatrix CreateBillboardMatrix();

    UObject* Duplicate(UObject* Outer, UClass* ClassInfo) override;

    ID3D11Buffer* vertexTextureBuffer;
    ID3D11Buffer* indexTextureBuffer;
    UINT numVertices;
    UINT numIndices;
    float finalIndexU = 0.0f;
    float finalIndexV = 0.0f;
    std::shared_ptr<FTexture> Texture;

    ID3D11ShaderResourceView* GetTextureID() { return TextureID; }

    ID3D11ShaderResourceView* TextureID;
protected:

    USceneComponent* m_parent = nullptr;

    bool CheckPickingOnNDC(const TArray<FVector>& checkQuad, float& hitDistance);

private:
    void CreateQuadTextureVertexBuffer();
};
