#include "UTextUUID.h"
#include "Engine/Classes/GameFramework/Actor.h"

REGISTER_CLASS(UTextUUID, UText)

UTextUUID::UTextUUID()
{
    SetScale(FVector(0.1f, 0.25f, 0.25f));
    SetLocation(FVector(0.0f, 0.0f, -0.5f));
}

UTextUUID::~UTextUUID()
{
}

int UTextUUID::CheckRayIntersection(FVector& rayOrigin, FVector& rayDirection, float& pfNearHitDistance)
{
    return 0;
}

void UTextUUID::SetUUID(uint32 UUID)
{
    SetText(std::to_wstring(UUID));
}

void UTextUUID::InitializeComponent()
{
    Super::InitializeComponent();
    SetText(std::to_wstring(GetOwner()->GetUUID()));
}


