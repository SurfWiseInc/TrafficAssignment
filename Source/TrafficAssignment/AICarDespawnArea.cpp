// Fill out your copyright notice in the Description page of Project Settings.


#include "AICarDespawnArea.h"
#include "Components/BoxComponent.h"
#include "TrafficManager.h"
#include "AICarPawn.h"

// Sets default values
AAICarDespawnArea::AAICarDespawnArea()
{
	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = MyBoxComponent;

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAICarDespawnArea::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAICarDespawnArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAICarDespawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICarDespawnArea::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
					int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		AAICarPawn* AICar = Cast<AAICarPawn>(OtherActor);
		if (AICar != nullptr)
		{
			UE_LOG(LogTemp, Warning,TEXT("We overlapped with actor"));
			AICar->DespawnAICar(AICar);
		}
	}
}