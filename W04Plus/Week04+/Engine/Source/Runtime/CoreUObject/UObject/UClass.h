#pragma once
#include <concepts>
#include "Object.h"
#include "Core/Container/Array.h"

/**
 * UObject의 RTTI를 가지고 있는 클래스
 */
class UClass : public UObject
{
public:
    UClass(const char* InClassName, uint32 InClassSize, uint32 InAlignment, UClass* InSuperClass);
    virtual ~UClass() override = default;

    // 복사 & 이동 생성자 제거
    UClass(const UClass&) = delete;
    UClass& operator=(const UClass&) = delete;
    UClass(UClass&&) = delete;
    UClass& operator=(UClass&&) = delete;

    using ObjectCreator = void* (*)();
    ObjectCreator Creator = nullptr;

    template <typename T>
    T* CreateObject()
    {
        if (!Creator)
        {
            return nullptr;
        }
        return static_cast<T*>(Creator());
    }

    uint32 GetClassSize() const { return ClassSize; }
    uint32 GetClassAlignment() const { return ClassAlignment; }

    /** SomeBase의 자식 클래스인지 확인합니다. */
    bool IsChildOf(const UClass* SomeBase) const;

    template <typename T>
        requires std::derived_from<T, UObject>
    bool IsChildOf() const
    {
        return IsChildOf(T::StaticClass());
    }

    /**
     * 부모의 UClass를 가져옵니다.
     *
     * @note AActor::StaticClass()->GetSuperClass() == UObject::StaticClass()
     */
    UClass* GetSuperClass() const
    {
        return SuperClass;
    }

    UObject* GetDefaultObject() const
    {
        if (!ClassDefaultObject)
        {
            const_cast<UClass*>(this)->CreateDefaultObject();
        }
        return ClassDefaultObject;
    }

    static TMap<FString, UClass*>& GetRegistry()
    {
        static TMap<FString, UClass*> Registry;
        return Registry;
    }

    static UClass* FindClass(const FString& ClassName)
    {
        auto& Registry = GetRegistry();
        auto Found = Registry.Find(ClassName);
        if (Found == nullptr)
        {
            return nullptr;
        }
        return *Found;
    }

    static void RegisterClass(UClass* Class)
    {
        GetRegistry()[Class->GetName()] = Class;
    }

protected:
    virtual UObject* CreateDefaultObject();

private:
    [[maybe_unused]]
    uint32 ClassSize;

    [[maybe_unused]]
    uint32 ClassAlignment;

    UClass* SuperClass = nullptr;

    UObject* ClassDefaultObject = nullptr;
};
