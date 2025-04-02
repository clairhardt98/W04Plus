#include "OutlinerEditorPanel.h"
#include "EngineLoop.h"
#include "World.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/ActorComponent.h"


void OutlinerEditorPanel::Render()
{
    /* Pre Setup */
    ImGuiIO& io = ImGui::GetIO();
    
    float PanelWidth = (Width) * 0.2f - 6.0f;
    float PanelHeight = (Height) * 0.3f;

    float PanelPosX = (Width) * 0.8f + 5.0f;
    float PanelPosY = 5.0f;

    ImVec2 MinSize(140, 100);
    ImVec2 MaxSize(FLT_MAX, 500);
    
    /* Min, Max Size */
    ImGui::SetNextWindowSizeConstraints(MinSize, MaxSize);
    
    /* Panel Position */
    ImGui::SetNextWindowPos(ImVec2(PanelPosX, PanelPosY), ImGuiCond_Always);

    /* Panel Size */
    ImGui::SetNextWindowSize(ImVec2(PanelWidth, PanelHeight), ImGuiCond_Always);

    /* Panel Flags */
    ImGuiWindowFlags PanelFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    
    /* Render Start */
    ImGui::Begin("Outliner", nullptr, PanelFlags);

    if (ImGui::TreeNode("Scene Actors")) // 트리 노드 생성
    {
        UWorld* World = GEngineLoop.GetWorld();
        for (AActor* Actor : World->GetActors())
        {
            bool bIsActorSelected = Actor->GetRootComponent() && Actor->GetRootComponent() == World->GetSelectedComponent();

            //if (bIsActorSelected)
            //{
            //    RenderAddComponentCombo(Actor);
            //}

            ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

            // 선택된 액터 처리
            if (bIsActorSelected)
            {
                NodeFlags |= ImGuiTreeNodeFlags_Selected;
                NodeFlags |= ImGuiTreeNodeFlags_DefaultOpen; // 선택 시 자동으로 열기
            }


            //FString ActorName = Actor->GetActorLabel() + TEXT("(") + Actor->GetClass()->GetName() + TEXT(")");
            if (ImGui::TreeNodeEx(Actor, NodeFlags, "%s", *Actor->GetActorLabel()))
            {
                if (ImGui::IsItemClicked())
                {
                    World->SetPickedComponent(Actor->GetRootComponent());
                }

                const auto& Components = Actor->GetComponents();
                for (UActorComponent* Component : Components)
                {
                    USceneComponent* SceneComponent = Cast<USceneComponent>(Component);
                    if (SceneComponent == nullptr)
                        continue;

                    bool bIsComponentSelected = World->GetSelectedComponent() == SceneComponent;

                    if (ImGui::Selectable(*SceneComponent->GetName(), bIsComponentSelected))
                    {
                        World->SetPickedComponent(SceneComponent);
                    }
                }

                ImGui::TreePop(); // Actor 노드 닫기
            }
        }
        ImGui::TreePop(); // Scene Actors 트리 닫기
    }
    ImGui::End();
}
    
void OutlinerEditorPanel::OnResize(HWND hWnd)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    Width = clientRect.right - clientRect.left;
    Height = clientRect.bottom - clientRect.top;
}
