// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once
#include "UClass.h"
#include "ObjectFactory.h"

// name을 문자열화 해주는 매크로
#define INLINE_STRINGIFY(name) #name

constexpr auto INVALID_OBJECT = static_cast<class UObject*>(nullptr);

// RTTI를 위한 클래스 매크로
#define DECLARE_CLASS(TClass, TSuperClass) \
private: \
    TClass(const TClass&) = delete; \
    TClass& operator=(const TClass&) = delete; \
    TClass(TClass&&) = delete; \
    TClass& operator=(TClass&&) = delete; \
public: \
    using Super = TSuperClass; \
    using ThisClass = TClass; \
    static UClass* StaticClass() { \
        static UClass ClassInfo{ TEXT(#TClass), static_cast<uint32>(sizeof(TClass)), static_cast<uint32>(alignof(TClass)), TSuperClass::StaticClass() }; \
        return &ClassInfo; \
    }

#define REGISTER_CLASS(TClass, TSuperClass) \
    using Super = TSuperClass; \
    using ThisClass = TClass; \
    static UClass TClass##_StaticClass{ TEXT(#TClass), static_cast<uint32>(sizeof(TClass)), static_cast<uint32>(alignof(TClass)), TSuperClass::StaticClass() }; \
    struct TClass##_ClassInfo { \
        TClass##_ClassInfo() { \
            TClass##_StaticClass.Creator = []() -> void* { return FObjectFactory::ConstructObject<TClass>(); }; \
            UClass::RegisterClass(&TClass##_StaticClass); \
        } \
    }; \
    static TClass##_ClassInfo TClass##_AutoRegister;

// #define PROPERTY(Type, VarName, DefaultValue) \
// private: \
//     Type VarName DefaultValue; \
// public: \
//     Type Get##VarName() const { return VarName; } \
//     void Set##VarName(const Type& New##VarName) { VarName = New##VarName; }

// Getter & Setter 생성
#define PROPERTY(type, name) \
    void Set##name(const type& value) { name = value; } \
    type Get##name() const { return name; }