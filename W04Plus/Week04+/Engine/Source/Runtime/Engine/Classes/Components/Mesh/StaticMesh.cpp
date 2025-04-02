#include "StaticMesh.h"
#include "Engine/FLoaderOBJ.h"
#include "UObject/ObjectFactory.h"

REGISTER_CLASS(UStaticMesh, UObject)

UStaticMesh::UStaticMesh()
{

}

UStaticMesh::~UStaticMesh()
{
    if (staticMeshRenderData == nullptr) return;

    if (staticMeshRenderData->VertexBuffer) {
        staticMeshRenderData->VertexBuffer->Release();
        staticMeshRenderData->VertexBuffer = nullptr;
    }

    if (staticMeshRenderData->IndexBuffer) {
        staticMeshRenderData->IndexBuffer->Release();
        staticMeshRenderData->IndexBuffer = nullptr;
    }
}

uint32 UStaticMesh::GetMaterialIndex(FName MaterialSlotName) const
{
    for (uint32 materialIndex = 0; materialIndex < materials.Num(); materialIndex++) {
        if (materials[materialIndex]->MaterialSlotName == MaterialSlotName)
            return materialIndex;
    }

    return -1;
}

void UStaticMesh::GetUsedMaterials(TArray<UMaterial*>& Out) const
{
    for (const FStaticMaterial* Material : materials)
    {
        Out.Emplace(Material->Material);
    }
}

void UStaticMesh::SetData(OBJ::FStaticMeshRenderData* renderData)
{
    staticMeshRenderData = renderData;

    uint32 verticeNum = staticMeshRenderData->Vertices.Num();
    if (verticeNum <= 0) return;
    staticMeshRenderData->VertexBuffer = GetEngine().renderer.CreateVertexBuffer(staticMeshRenderData->Vertices, verticeNum * sizeof(FVertexSimple));

    uint32 indexNum = staticMeshRenderData->Indices.Num();
    if (indexNum > 0)
        staticMeshRenderData->IndexBuffer = GetEngine().renderer.CreateIndexBuffer(staticMeshRenderData->Indices, indexNum * sizeof(uint32));

    for (int materialIndex = 0; materialIndex < staticMeshRenderData->Materials.Num(); materialIndex++) {
        FStaticMaterial* newMaterialSlot = new FStaticMaterial();
        UMaterial* newMaterial = FManagerOBJ::CreateMaterial(staticMeshRenderData->Materials[materialIndex]);

        newMaterialSlot->Material = newMaterial;
        newMaterialSlot->MaterialSlotName = staticMeshRenderData->Materials[materialIndex].MTLName;

        materials.Add(newMaterialSlot);
    }
}

UObject* UStaticMesh::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);

    DuplicateProperties(NewObject, Params);
    return NewObject;
}

void UStaticMesh::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);

    UStaticMesh* NewStaticMesh = static_cast<UStaticMesh*>(NewObject);
    if (!NewStaticMesh)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not UStaticMesh");
        return;
    }

    if (Params.bDeepCopy && staticMeshRenderData)
    {
        OBJ::FStaticMeshRenderData* NewRenderData = new OBJ::FStaticMeshRenderData(*staticMeshRenderData);
        NewStaticMesh->staticMeshRenderData = NewRenderData;
    }
    else
    {
        // 얕은 복사의 경우 포인터만 복사합니다.
        NewStaticMesh->staticMeshRenderData = staticMeshRenderData;
    }

    // materials는 보통 공유 자원이므로 얕은 복사로 충분합니다.
    NewStaticMesh->materials = materials;
}
