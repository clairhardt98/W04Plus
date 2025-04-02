#include "CubeComp.h"
#include "Math/JungleMath.h"
#include "World.h"
#include "UnrealEd/PrimitiveBatch.h"
#include "UObject/ObjectFactory.h"
#include "PropertyEditor/ShowFlags.h"

#include "UnrealEd/EditorViewportClient.h"
#include "LevelEditor/SLevelEditor.h"

#include "Engine/FLoaderOBJ.h"
#include "UObject/Casts.h"

REGISTER_CLASS(UCubeComp, UStaticMeshComponent)
UCubeComp::UCubeComp()
{
    SetType(StaticClass()->GetName());
    AABB.max = { 1,1,1 };
    AABB.min = { -1,-1,-1 };

}

UCubeComp::~UCubeComp()
{
}

UObject* UCubeComp::Duplicate(UObject* Outer, UClass* ClassInfo)
{
    UCubeComp* NewComponent = Cast<UCubeComp>(Super::Duplicate(Outer, ClassInfo));
    return NewComponent;
}

void UCubeComp::InitializeComponent()
{
    Super::InitializeComponent();

    FManagerOBJ::CreateStaticMesh("Assets/helloBlender.obj");
    SetStaticMesh(FManagerOBJ::GetStaticMesh(L"helloBlender.obj"));
}

void UCubeComp::TickComponent(float DeltaTime)
{
    Super::TickComponent(DeltaTime);

}