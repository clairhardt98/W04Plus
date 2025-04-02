#pragma once
#include "D3D11RHI/GraphicDevice.h"
#include "core/Container/String.h"
struct FTexture
{
    FTexture(ID3D11ShaderResourceView* SRV, ID3D11Texture2D* Texture2D, ID3D11SamplerState* Sampler, uint32 _width, uint32 _height, FString _Name)
        : TextureSRV(SRV), Texture(Texture2D), SamplerState(Sampler), width(_width), height(_height), Name(_Name)
    {}
    ~FTexture()
    {
		
    }
    void Release() {
        if (TextureSRV) { TextureSRV->Release(); TextureSRV = nullptr; }
        if (Texture) { Texture->Release(); Texture = nullptr; }
        if (SamplerState) { SamplerState->Release(); SamplerState = nullptr; }
    }
    FString Name; //진짜 pure이름
    ID3D11ShaderResourceView* TextureSRV = nullptr;
    ID3D11Texture2D* Texture = nullptr;
    ID3D11SamplerState* SamplerState = nullptr;
    uint32 width;
    uint32 height;
};
