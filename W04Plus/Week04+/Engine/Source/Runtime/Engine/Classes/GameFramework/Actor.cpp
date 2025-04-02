#include "Actor.h"

#include "World.h"
#include <Components/UParticleSubUVComp.h>

REGISTER_CLASS(AActor, UObject)
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
    //const auto CopyComponents = OwnedComponents;
    for (UActorComponent* Comp : OwnedComponents)
    {
        if (Comp->IsA<UParticleSubUVComp>())
        {
            int i = 1;
        }
        if (Comp && Comp->bIsComponentTickEnabled())
        {
            Comp->TickComponent(DeltaTime);
        }
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



void AActor::DuplicateSiblings(UObject* Outer)
{
    // 여기선 ActorComponent들을 복사한다
    // Caller => src Actor
    // Outer => dst Actor

    AActor* NewActor = Cast<AActor>(Outer);

    for (UActorComponent* Comp : OwnedComponents)
    {
        FString ClassName = Comp->GetClass()->GetName();
        UClass* ClassInfo = UClass::FindClass(ClassName);
        if (!ClassInfo)
            continue;

        Comp->Duplicate(Outer, ClassInfo);

        //Comp->bHasBeenInitialized = false;
        //Comp->bHasBegunPlay = false;
        //Comp->
    }

}

UObject* AActor::Duplicate(UObject* Outer, UClass* ClassInfo)
{
    UWorld* NewWorld = Cast<UWorld>(Outer);
    AActor* NewActor = NewWorld->SpawnActorByClass(ClassInfo, false);

    DuplicateSiblings(NewActor);

    return NewActor;
}

UActorComponent* AActor::AddComponentByClass(UClass* ComponentClass)
{
    if (ComponentClass == nullptr)
        return nullptr;

    UActorComponent* Component = ComponentClass->CreateObject<UActorComponent>();
    if (Component == nullptr)
        return nullptr;

    OwnedComponents.Add(Component);
    Component->Owner = this;
    Component->SetOuter(this);

    if (USceneComponent* NewSceneComp = Cast<USceneComponent>(Component))
    {
        if (RootComponent == nullptr)
        {
            RootComponent = NewSceneComp;
        }
        else
        {
            NewSceneComp->SetupAttachment(RootComponent);
        }
    }


    Component->InitializeComponent();

    return Component;
}
