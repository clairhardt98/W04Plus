// UText.cpp
#include "UText.h"
#include "World.h"
#include "Engine/Source/Editor/PropertyEditor/ShowFlags.h"
#include "UnrealEd/EditorViewportClient.h"
#include "LevelEditor/SLevelEditor.h"
#include "Math/MathUtility.h"
#include "Math/JungleMath.h"

REGISTER_CLASS(UText, UPrimitiveComponent)

UText::UText()
{
    SetType(StaticClass()->GetName());
}

UText::~UText()
{
    if (vertexTextBuffer)
    {
        vertexTextBuffer->Release();
        vertexTextBuffer = nullptr;
    }
}

void UText::InitializeComponent()
{
    Super::InitializeComponent();
    SetTexture(L"Assets/Texture/font.png");
    SetRowColumnCount(106, 106);
    SetText(L"안녕하세요 Jungle 1");
}

void UText::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);
}

void UText::ClearText()
{
    vertexTextureArr.Empty();
    quad.Empty();
}

void UText::SetRowColumnCount(int _cellsPerRow, int _cellsPerColumn)
{
    RowCount = _cellsPerRow;
    ColumnCount = _cellsPerColumn;
}

int UText::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    if (!(ShowFlags::GetInstance().currentFlags & static_cast<uint64>(EEngineShowFlags::SF_BillboardText))) {
        return 0;
    }
    quad.Empty();
    for (const auto& v : vertexTextureArr)
    {
        quad.Add(FVector(v.x, v.y, v.z));
    }
    return CheckPickingOnNDC(quad, pfNearHitDistance);
}

void UText::SetText(FWString _text)
{
    text = _text;
    vertexTextureArr.Empty();
    quad.Empty();

    if (_text.empty())
    {
        Console::GetInstance().AddLog(LogLevel::Warning, "Text is empty");
        if (vertexTextBuffer)
        {
            vertexTextBuffer->Release();
            vertexTextBuffer = nullptr;
        }
        return;
    }

    uint32 BitmapWidth = Texture->width;
    uint32 BitmapHeight = Texture->height;

    float CellWidth = float(BitmapWidth) / ColumnCount;
    float CellHeight = float(BitmapHeight) / RowCount;

    float nTexelUOffset = CellWidth / BitmapWidth;
    float nTexelVOffset = CellHeight / BitmapHeight;

    for (int i = 0; i < _text.size(); i++)
    {
        FVertexTexture leftUP = { 0.0f,1.0f,1.0f,0.0f,0.0f };
        FVertexTexture rightUP = { 0.0f, -1.0f,1.0f,1.0f,0.0f };
        FVertexTexture leftDown = { 0.0f, 1.0f,-1.0f,0.0f,1.0f };
        FVertexTexture rightDown = { 0.0f, -1.0f,-1.0f,1.0f,1.0f };

        rightUP.u *= nTexelUOffset;
        leftDown.v *= nTexelVOffset;
        rightDown.u *= nTexelUOffset;
        rightDown.v *= nTexelVOffset;

        leftUP.y -= quadWidth * i;
        rightUP.y -= quadWidth * i;
        leftDown.y -= quadWidth * i;
        rightDown.y -= quadWidth * i;

        float startU = 0.0f;
        float startV = 0.0f;
        setStartUV(_text[i], startU, startV);

        leftUP.u += (nTexelUOffset * startU);
        leftUP.v += (nTexelVOffset * startV);
        rightUP.u += (nTexelUOffset * startU);
        rightUP.v += (nTexelVOffset * startV);
        leftDown.u += (nTexelUOffset * startU);
        leftDown.v += (nTexelVOffset * startV);
        rightDown.u += (nTexelUOffset * startU);
        rightDown.v += (nTexelVOffset * startV);

        vertexTextureArr.Add(leftUP);
        vertexTextureArr.Add(rightUP);
        vertexTextureArr.Add(leftDown);
        vertexTextureArr.Add(rightUP);
        vertexTextureArr.Add(rightDown);
        vertexTextureArr.Add(leftDown);
    }

    float lastX = -1.0f + quadSize * _text.size();
    quad.Add(FVector(-1.0f, 1.0f, 0.0f));
    quad.Add(FVector(-1.0f, -1.0f, 0.0f));
    quad.Add(FVector(lastX, 1.0f, 0.0f));
    quad.Add(FVector(lastX, -1.0f, 0.0f));

    UINT byteWidth = static_cast<UINT>(vertexTextureArr.Num() * sizeof(FVertexTexture));
    CreateTextTextureVertexBuffer(vertexTextureArr, byteWidth);
}

void UText::TextMVPRendering()
{
    FEngineLoop::renderer.PrepareTextureShader();

    FMatrix Model = bBillboardMode ? CreateBillboardMatrix() : CreateStandardModelMatrix();
    FMatrix View = GetEngine().GetLevelEditor()->GetActiveViewportClient()->GetViewMatrix();
    FMatrix Proj = GetEngine().GetLevelEditor()->GetActiveViewportClient()->GetProjectionMatrix();

    FMatrix MVP = Model * View * Proj;
    FMatrix NormalMatrix = FMatrix::Transpose(FMatrix::Inverse(Model));

    FVector4 UUIDColor = EncodeUUID() / 255.0f;
    bool bSelected = (GetWorld()->GetSelectedActor() == GetOwner());

    FEngineLoop::renderer.UpdateConstant(MVP, NormalMatrix, UUIDColor, bSelected);
    FEngineLoop::renderer.RenderTextPrimitive(vertexTextBuffer, numTextVertices, Texture->TextureSRV, Texture->SamplerState);
}

FMatrix UText::CreateBillboardMatrix()
{
    FMatrix View = GetEngine().GetLevelEditor()->GetActiveViewportClient()->GetViewMatrix();
    View.M[0][3] = View.M[1][3] = View.M[2][3] = 0.0f;
    View.M[3][0] = View.M[3][1] = View.M[3][2] = 0.0f;
    //View.M[0][2] = -View.M[0][2];
   /* View.M[1][2] = -View.M[1][2];
    View.M[2][2] = -View.M[2][2];*/
    FMatrix billboard({
        {0,-1,0,0},
        {-1,0,0,0},
        {0,0,-1,0},
        {0,0,0,1},
        });
    FMatrix LookAt = View * billboard;
    FMatrix Scale = FMatrix::CreateScale(RelativeScale3D.x, RelativeScale3D.y, RelativeScale3D.z);
    FMatrix Trans = FMatrix::CreateTranslationMatrix(GetWorldLocation());
    return Scale * LookAt * Trans;
}

FMatrix UText::CreateStandardModelMatrix()
{
    return JungleMath::CreateModelMatrix(GetWorldLocation(), GetWorldRotation(), GetWorldScale());
}

void UText::SetTexture(FWString _fileName)
{
    Texture = FEngineLoop::resourceMgr.GetTexture(_fileName);
}

void UText::SetBillboardMode(bool bEnable)
{
    bBillboardMode = bEnable;
}

bool UText::CheckPickingOnNDC(const TArray<FVector>& checkQuad, float& hitDistance)
{
    bool result = false;
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GEngineLoop.hWnd, &mousePos);

    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    FEngineLoop::graphicDevice.DeviceContext->RSGetViewports(&numViewports, &viewport);

    FVector pickPosition;
    pickPosition.x = ((2.0f * mousePos.x / viewport.Width) - 1);
    pickPosition.y = -((2.0f * mousePos.y / viewport.Height) - 1);
    pickPosition.z = 1.0f;

    FMatrix M = bBillboardMode ? CreateBillboardMatrix() : CreateStandardModelMatrix();
    FMatrix V = GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->GetViewMatrix();
    FMatrix P = GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->GetProjectionMatrix();
    FMatrix MVP = M * V * P;

    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;
    float avgZ = 0.0f;

    for (const FVector& p : checkQuad)
    {
        FVector4 v = FVector4(p.x, p.y, p.z, 1.0f);
        FVector4 clip = FMatrix::TransformVector(v, MVP);
        if (clip.a != 0) clip = clip / clip.a;

        minX = FMath::Min(minX, clip.x);
        maxX = FMath::Max(maxX, clip.x);
        minY = FMath::Min(minY, clip.y);
        maxY = FMath::Max(maxY, clip.y);
        avgZ += clip.z;
    }

    avgZ /= checkQuad.Num();

    if (pickPosition.x >= minX && pickPosition.x <= maxX && pickPosition.y >= minY && pickPosition.y <= maxY)
    {
        FMatrix& P = GEngineLoop.GetLevelEditor()->GetActiveViewportClient()->GetProjectionMatrix();
        float A = P.M[2][2];
        float B = P.M[3][2];
        float z_view_pick = (pickPosition.z - B) / A;
        float z_view_text = (avgZ - B) / A;
        hitDistance = 1000.0f;
        result = true;
    }
    return result;
}

void UText::setStartUV(wchar_t hangul, float& outStartU, float& outStartV)
{
    //대문자만 받는중
    int StartU = 0;
    int StartV = 0;
    int offset = -1;

    if (hangul == L' ') {
        outStartU = 0;  // Space는 특별히 UV 좌표를 (0,0)으로 설정
        outStartV = 0;
        offset = 0;
        return;
    }
    else if (hangul >= L'A' && hangul <= L'Z') {

        StartU = 11;
        StartV = 0;
        offset = hangul - L'A'; // 대문자 위치
    }
    else if (hangul >= L'a' && hangul <= L'z') {
        StartU = 37;
        StartV = 0;
        offset = (hangul - L'a'); // 소문자는 대문자 다음 위치
    }
    else if (hangul >= L'0' && hangul <= L'9') {
        StartU = 1;
        StartV = 0;
        offset = (hangul - L'0'); // 숫자는 소문자 다음 위치
    }
    else if (hangul >= L'가' && hangul <= L'힣')
    {
        StartU = 63;
        StartV = 0;
        offset = hangul - L'가'; // 대문자 위치
    }

    if (offset == -1)
    {
        Console::GetInstance().AddLog(LogLevel::Warning, "Text Error");
    }

    int offsetV = (offset + StartU) / ColumnCount;
    int offsetU = (offset + StartU) % ColumnCount;

    outStartU = static_cast<float>(offsetU);
    outStartV = static_cast<float>(StartV + offsetV);
}
void UText::setStartUV(char alphabet, float& outStartU, float& outStartV)
{
    //대문자만 받는중
    int StartU=0;
    int StartV=0;
    int offset = -1;


    if (alphabet == ' ') {
        outStartU = 0;  // Space는 특별히 UV 좌표를 (0,0)으로 설정
        outStartV = 0;
        offset = 0;
        return;
    }
    else if (alphabet >= 'A' && alphabet <= 'Z') {

        StartU = 1;
        StartV = 4;
        offset = alphabet - 'A'; // 대문자 위치
    }
    else if (alphabet >= 'a' && alphabet <= 'z') {
        StartU = 1;
        StartV = 6;
        offset = (alphabet - 'a'); // 소문자는 대문자 다음 위치
    }
    else if (alphabet >= '0' && alphabet <= '9') {
        StartU = 0;
        StartV = 3;
        offset = (alphabet - '0'); // 숫자는 소문자 다음 위치
    }

    if (offset == -1)
    {
        Console::GetInstance().AddLog(LogLevel::Warning, "Text Error");
    }

    int offsetV = (offset + StartU) / ColumnCount;
    int offsetU = (offset + StartU) % ColumnCount;

    outStartU = static_cast<float>(offsetU);
    outStartV = static_cast<float>(StartV + offsetV);

}
void UText::CreateTextTextureVertexBuffer(const TArray<FVertexTexture>& _vertex,UINT byteWidth)
{
	numTextVertices = static_cast<UINT>(_vertex.Num());
	// 2. Create a vertex buffer
	D3D11_BUFFER_DESC vertexbufferdesc = {};
	vertexbufferdesc.ByteWidth = byteWidth;
	vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
	vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexbufferSRD = { _vertex.GetData()};

	ID3D11Buffer* vertexBuffer;
	
	HRESULT hr = FEngineLoop::graphicDevice.Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);
	if (FAILED(hr))
	{
		UE_LOG(LogLevel::Warning, "VertexBuffer Creation faild");
	}
	vertexTextBuffer = vertexBuffer;

	//FEngineLoop::resourceMgr.RegisterMesh(&FEngineLoop::renderer, "JungleText", _vertex, _vertex.Num() * sizeof(FVertexTexture),
	//	nullptr, 0);

}
