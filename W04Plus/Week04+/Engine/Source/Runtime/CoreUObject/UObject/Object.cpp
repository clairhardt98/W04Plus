#include "Engine/Source/Runtime/CoreUObject/UObject/Object.h"

#include "UClass.h"
#include "UObjectHash.h"
#include "CoreUObject/UObject/Casts.h"
#include "World.h"


UClass* UObject::StaticClass()
{
    static UClass ClassInfo{TEXT("UObject"), sizeof(UObject), alignof(UObject), nullptr};
    return &ClassInfo;
}

UObject::UObject()
    : UUID(0)
    // TODO: Object를 생성할 때 직접 설정하기
    , InternalIndex(std::numeric_limits<uint32>::max())
    , NamePrivate("None")
{
}

UWorld* UObject::GetWorld()
{
    UObject* CrrOuter = GetOuter();
    while (CrrOuter)
    {
        UWorld* World = dynamic_cast<UWorld*>(CrrOuter);
        if (World)
        {
            return World;
        }
        CrrOuter = CrrOuter->GetOuter();
    }
    return nullptr;
}

bool UObject::IsA(const UClass* SomeBase) const
{
    const UClass* ThisClass = GetClass();
    return ThisClass->IsChildOf(SomeBase);
}
