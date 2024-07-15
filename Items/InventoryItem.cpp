#include "InventoryItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "AutomationTestsProj/Components/InventoryComponent.h"

AInventoryItem::AInventoryItem()
{
	PrimaryActorTick.bCanEverTick = false;

	//Create collision / set parameters:
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	check(CollisionComponent);
	CollisionComponent->InitSphereRadius(30.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);
	
}

void AInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//Handle collision with items:
	const auto Pawn = Cast<APawn>(OtherActor);
	if(Pawn)
	{
		//Create inv component from the one assigned to actor, add item score to the Data and destroy item
		if(const auto InvComp = Pawn->FindComponentByClass<UInventoryComponent>())
		{
			if(InvComp->AddItem(InventoryData))
			{
				Destroy();
			}
		}
	}
}



