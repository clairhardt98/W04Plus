#include "Actor.h"

#include "World.h"

void AActor::BeginPlay()
{
    // TODO: 나중에 삭제를 Pending으로 하던가 해서 복사비용 줄이기
    const auto CopyComponents = OwnedComponents;
    for (UActorComponent* Comp : CopyComponents)
    {
        Comp->BeginPlay();
    }
}

void AActor::Tick(float DeltaTime)
{
    // TODO: 임시로 Actor에서 Tick 돌리기
    // TODO: 나중에 삭제를 Pending으로 하던가 해서 복사비용 줄이기
    const auto CopyComponents = OwnedComponents;
    for (UActorComponent* Comp : CopyComponents)
    {
        Comp->TickComponent(DeltaTime);
    }
}

void AActor::Destroyed()
{
    // Actor가 제거되었을 때 호출하는 EndPlay
    EndPlay(EEndPlayReason::Destroyed);
}

void AActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 본인이 소유하고 있는 모든 컴포넌트의 EndPlay 호출
    for (UActorComponent* Component : GetComponents())
    {
        if (Component->HasBegunPlay())
        {
            Component->EndPlay(EndPlayReason);
        }
    }
    UninitializeComponents();
}

UObject* AActor::DuplicateObject(const FObjectDuplicationParameters& Params) const
{
    UObject* NewObject = Super::DuplicateObject(Params);
    DuplicateProperties(this->GetClass(), Params);

    return NewObject;
}

void AActor::DuplicateProperties(UObject* NewObject, const FObjectDuplicationParameters& Params) const
{
    Super::DuplicateProperties(NewObject, Params);

    AActor* NewActor = static_cast<AActor*>(NewObject);
    if (!NewActor)
    {
        UE_LOG(LogLevel::Error, "DuplicateProperties: NewObject is not AActor");
        return;
    }

    for (UActorComponent* Comp : OwnedComponents)
    {
        if (Comp)
        {
            // 각 컴포넌트에 대해 동일한 Outer와 DuplicationMap을 사용하여 복제
            FObjectDuplicationParameters CompParams(
                Comp,
                Params.Outer, // 또는 NewActor를 Outer로 사용할 수도 있음
                NAME_None,
                Params.DuplicationFlags,
                Params.bDeepCopy,
                Params.DuplicationMap ? Params.DuplicationMap : nullptr // 이미 부모에서 로컬 맵을 생성했을 수도 있음
            );

            UActorComponent* DuplicatedComp = static_cast<UActorComponent*>(Comp->DuplicateObject(CompParams));
            if (DuplicatedComp)
            {
                NewActor->OwnedComponents.Add(DuplicatedComp);
                DuplicatedComp->Owner = NewActor;
            }
        }
    }

    //// RootComponent가 존재한다면 이를 복제합니다.
    //if (RootComponent)
    //{
    //    FObjectDuplicationParameters RootParams(
    //        RootComponent,
    //        Params.Outer, // 또는 NewActor
    //        NAME_None,
    //        Params.DuplicationFlags,
    //        Params.bDeepCopy,
    //        Params.DuplicationMap ? Params.DuplicationMap : nullptr
    //    );
    //    // USceneComponent는 UObject의 파생 클래스이므로 DuplicateObject를 호출할 수 있습니다.
    //    USceneComponent* DuplicatedRoot = Cast<USceneComponent>(RootComponent->DuplicateObject(RootParams));
    //    NewActor->RootComponent = DuplicatedRoot;
    //}

}

bool AActor::Destroy()
{
    if (!IsActorBeingDestroyed())
    {
        if (UWorld* World = GetWorld())
        {
            World->DestroyActor(this);
            bActorIsBeingDestroyed = true;
        }
    }

    return IsActorBeingDestroyed();
}

void AActor::RemoveOwnedComponent(UActorComponent* Component)
{
    OwnedComponents.Remove(Component);
}

void AActor::InitializeComponents()
{
    TSet<UActorComponent*> Components = GetComponents();
    for (UActorComponent* ActorComp : Components)
    {
        if (ActorComp->bAutoActive && !ActorComp->IsActive())
        {
            ActorComp->Activate();
        }

        if (!ActorComp->HasBeenInitialized())
        {
            ActorComp->InitializeComponent();
        }
    }
}

void AActor::UninitializeComponents()
{
    TSet<UActorComponent*> Components = GetComponents();
    for (UActorComponent* ActorComp : Components)
    {
        if (ActorComp->HasBeenInitialized())
        {
            ActorComp->UninitializeComponent();
        }
    }
}

bool AActor::SetRootComponent(USceneComponent* NewRootComponent)
{
    if (NewRootComponent == nullptr || NewRootComponent->GetOwner() == this)
    {
        if (RootComponent != NewRootComponent)
        {
            USceneComponent* OldRootComponent = RootComponent;
            RootComponent = NewRootComponent;

            OldRootComponent->SetupAttachment(RootComponent);
        }
        return true;
    }
    return false;
}

bool AActor::SetActorLocation(const FVector& NewLocation)
{
    if (RootComponent)
    {
        RootComponent->SetLocation(NewLocation);
        return true;
    }
    return false;
}

bool AActor::SetActorRotation(const FVector& NewRotation)
{
    if (RootComponent)
    {
        RootComponent->SetRotation(NewRotation);
        return true;
    }
    return false;
}

bool AActor::SetActorScale(const FVector& NewScale)
{
    if (RootComponent)
    {
        RootComponent->SetScale(NewScale);
        return true;
    }
    return false;
}
