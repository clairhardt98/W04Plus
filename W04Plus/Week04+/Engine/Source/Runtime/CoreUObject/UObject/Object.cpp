#include "Engine/Source/Runtime/CoreUObject/UObject/Object.h"

#include "UClass.h"
#include "UObjectHash.h"

#include "ObjectFactory.h"
#include "ObjectMacros.h"
#include "EngineStatics.h"


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

UObject* UObject::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    if (Params.SourceObject != this)
    {
        UE_LOG(LogLevel::Error, "DuplicateObject: SourceObject mismatch");
        return nullptr;
    }

    UObject* Outer = Params.Outer;
    if (Outer == nullptr || Outer == INVALID_OBJECT) Outer = FObjectFactory::GetTransientPackage();


    // 순환 참조 문제 방지를 위해 DuplicationMap이 제공되면 체크
    if (Params.DuplicationMap && Params.DuplicationMap->find(this) != Params.DuplicationMap->end()) return (*Params.DuplicationMap)[this];

    FName NewName = (Params.NewName != NAME_None) ? Params.NewName : this->NamePrivate;

    // test error
    //UObject* NewObject = FObjectFactory::ConstructObject<UObject>();
    UObject* NewObject = FObjectFactory::ConstructObjectFromClass<UObject>(GetClass(), Outer, NewName);
    // DuplicationMap이 있다면 현재 객체와 새 객체를 등록하여 순환 참조 방지
    if (Params.DuplicationMap)
    {
        (*Params.DuplicationMap)[this] = NewObject;
    }

    // 기본 멤버 복사
    NewObject->ClassPrivate = this->ClassPrivate;
    NewObject->UUID = UEngineStatics::GenUUID();

    //if (Params.bDeepCopy)
    //{
    //    // 가지고 있는거를 재귀적으로?
    //    this->
    //}
    //else
    //{
    //    //NewObject->
    //}
    DuplicateProperties(NewObject, Params);

    return NewObject;
}

void UObject::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
}

bool UObject::IsA(const UClass* SomeBase) const
{
    const UClass* ThisClass = GetClass();
    return ThisClass->IsChildOf(SomeBase);
}
