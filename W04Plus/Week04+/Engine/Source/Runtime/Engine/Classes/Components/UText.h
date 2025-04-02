#pragma once
// UText.h
#pragma once
#include "PrimitiveComponent.h"
#include "UTexture.h"


class UText : public UPrimitiveComponent
{
    DECLARE_CLASS(UText, UPrimitiveComponent)

public:
    UText();
    virtual ~UText() override;

    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime) override;
    virtual int CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance) override;

    void ClearText();
    void SetText(FWString _text);
    FWString GetText() { return text; }
    void SetRowColumnCount(int _cellsPerRow, int _cellsPerColumn);

    void SetTexture(FWString _fileName);
    void SetBillboardMode(bool bEnable);
    bool CheckPickingOnNDC(const TArray<FVector>& checkQuad, float& hitDistance);
    FMatrix CreateBillboardMatrix();
    FMatrix CreateStandardModelMatrix();

    TArray<FVertexTexture> vertexTextureArr;

    std::shared_ptr<FTexture> Texture;

    ID3D11Buffer* vertexTextBuffer = nullptr;
    UINT numTextVertices = 0;
    bool GetBillboardMode() const { return bBillboardMode; }


    UObject* Duplicate(UObject* Outer, UClass* ClassInfo) override;

protected:
    FWString text;
    TArray<FVector> quad;


    int RowCount = 0;
    int ColumnCount = 0;

    int quadSize = 2;
    float quadWidth = 2.0f;
    float quadHeight = 2.0f;
    bool bBillboardMode = true;

    void setStartUV(char alphabet, float& outStartU, float& outStartV);
    void setStartUV(wchar_t hangul, float& outStartU, float& outStartV);
    void CreateTextTextureVertexBuffer(const TArray<FVertexTexture>& _vertex, UINT byteWidth);
    

    
};
