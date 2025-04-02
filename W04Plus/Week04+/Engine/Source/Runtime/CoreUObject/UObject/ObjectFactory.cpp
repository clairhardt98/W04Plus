#include "ObjectFactory.h"

UObject* FObjectFactory::GetTransientPackage()
{
    static UObject* TransientPackage = FObjectFactory::ConstructObjectFromClass<UObject>(
        UObject::StaticClass(),
        nullptr,
        FName("TransientPackage")
    );
    return TransientPackage;
}
