#pragma once
#include "EngineLoop.h"
#include "NameTypes.h"

extern FEngineLoop GEngineLoop;

class UClass;
class UWorld;

namespace EDuplicateMode
{
    enum Type
    {
        /** No specific information about the reason for duplication */
        Normal,
        /** Object is being duplicated as part of a world duplication */
        World,
        /** Object is being duplicated as part of the process for entering Play In Editor */
        PIE
    };
};

struct FObjectDuplicationParameters {
    const class UObject* SourceObject;
    // Outer; 유효하지 않으면 Transient Package
    UObject* Outer;
    FName NewName;
    // 복제 플래그
    uint32 DuplicationFlags;
    bool bDeepCopy;
    // 순환 참조 처리를 위한 매핑 테이블 (없으면 nullptr)
    std::unordered_map<const UObject*, UObject*>* DuplicationMap;

    FObjectDuplicationParameters(
        const UObject* InSourceObject,
        UObject* InOuter = nullptr,
        FName InNewName = NAME_None,
        uint32 InDuplicationFlags = 0,
        bool bInDeepCopy = true,
        std::unordered_map<const UObject*, UObject*>* InDuplicationMap = nullptr
    )
        : SourceObject(InSourceObject)
        , Outer(InOuter)
        , NewName(InNewName)
        , DuplicationFlags(InDuplicationFlags)
        , bDeepCopy(bInDeepCopy)
        , DuplicationMap(InDuplicationMap)
    {
    }
};

class UObject
{
private:
    UObject(const UObject&) = delete;
    UObject& operator=(const UObject&) = delete;
    UObject(UObject&&) = delete;
    UObject& operator=(UObject&&) = delete;

public:
    using Super = UObject;
    using ThisClass = UObject;

    static UClass* StaticClass();

private:
    friend class FObjectFactory;
    friend class FSceneMgr;
    friend class UClass;

    uint32 UUID;
    uint32 InternalIndex; // Index of GUObjectArray

    FName NamePrivate;
    UClass* ClassPrivate = nullptr;

public:
    UObject();
    virtual ~UObject() = default;

    UWorld* GetWorld()
    {
        return GEngineLoop.GetWorld();
    }

    FEngineLoop& GetEngine()
    {
        return GEngineLoop;
    }

    FName GetFName() const { return NamePrivate; }
    FString GetName() const { return NamePrivate.ToString(); }

    uint32 GetUUID() const { return UUID; }
    uint32 GetInternalIndex() const { return InternalIndex; }

    UClass* GetClass() const { return ClassPrivate; }

    virtual UObject* DuplicateObject(const FObjectDuplicationParameters& Params) const;
    virtual void DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const {}

    /** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
    bool IsA(const UClass* SomeBase) const;

    template <typename T>
        requires std::derived_from<T, UObject>
    bool IsA() const
    {
        return IsA(T::StaticClass());
    }

public:
    void* operator new(size_t size)
    {
        UE_LOG(LogLevel::Display, "UObject Created : %d", size);

        void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(size);
        UE_LOG(
            LogLevel::Display,
            "TotalAllocationBytes : %d, TotalAllocationCount : %d",
            FPlatformMemory::GetAllocationBytes<EAT_Object>(),
            FPlatformMemory::GetAllocationCount<EAT_Object>()
        );
        return RawMemory;
    }

    void operator delete(void* ptr, size_t size)
    {
        UE_LOG(LogLevel::Display, "UObject Deleted : %d", size);
        FPlatformMemory::Free<EAT_Object>(ptr, size);
    }

    FVector4 EncodeUUID() const {
        FVector4 result;

        result.x = UUID % 0xFF;
        result.y = UUID >> 8 & 0xFF;
        result.z = UUID >> 16 & 0xFF;
        result.a = UUID >> 24 & 0xFF;

        return result;
    }
private:
};
