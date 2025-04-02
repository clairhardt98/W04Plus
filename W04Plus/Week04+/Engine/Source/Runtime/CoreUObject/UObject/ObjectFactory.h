#pragma once
#include "Object.h"
#include "UObjectHash.h"

#include "Core/EngineStatics.h"
#include "UObjectArray.h"
#include "UClass.h"

class FObjectFactory
{
public:
    template<typename T>
        requires std::derived_from<T, UObject>
    static T* ConstructObject()
    {
        uint32 id = UEngineStatics::GenUUID();
        FString Name = T::StaticClass()->GetName() + "_" + std::to_string(id);

        T* Obj = new T;  // TODO: FPlatformMemory::Malloc으로 변경, placement new 사용시 Free방법 생각하기
        Obj->ClassPrivate = T::StaticClass();
        Obj->NamePrivate = Name;
        Obj->UUID = id;

        GUObjectArray.AddObject(Obj);

        UE_LOG(LogLevel::Display, "Created New Object : %s", *Name);
        return Obj;
    }

    // 일단 이렇게 만들긴 했는데 ConstructObject와 겹치는게 너무 많아서 없애거나 수정하는게 좋을 듯 함.
    template<typename T>
        requires std::derived_from<T, UObject>
    static T* ConstructObjectFromClass(UClass* InClass, UObject* Outer, FName NewName)
    {
        uint32 id = UEngineStatics::GenUUID();

        FString Name = (NewName != NAME_None)
            ? NewName.ToString()
            : T::StaticClass()->GetName() + "_" + std::to_string(id);

        // T 타입으로 생성
        T* Obj = new T();
        Obj->ClassPrivate = InClass;
        Obj->NamePrivate = FName(*Name);
        Obj->UUID = id;

        // Outer 관련 해서 뭐 해야하나

        GUObjectArray.AddObject(Obj);

        UE_LOG(LogLevel::Display, "Constructed object from class: %s", *Name);
        return Obj;
    }

    static UObject* GetTransientPackage();
};