// Fill out your copyright notice in the Description page of Project Settings.


#include "TrafficManager.h"
#include "Components/SceneComponent.h"
#include "EngineUtils.h"
#include "AICarPawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "TimerManager.h"

// Sets default values
ATrafficManager::ATrafficManager()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialPoolSize = 5;

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Position"));
	RootComponent = SpawnPoint;
}

// Called when the game starts or when spawned
void ATrafficManager::BeginPlay()
{
	Super::BeginPlay();

	InitializePool();
	SetOnSpawnTimer(AverageSpawnTime, Divergence);
}

void ATrafficManager::InitializePool()
{
	if (!AICarClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't Initialize Pool. AICarClass template not assigned!"));
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		for (int i = 0; i < InitialPoolSize; ++i)
		{
			CreateAndAddCarToPool(World, i, false);
		}
	}
}

void ATrafficManager::OnSpawnCar()
{
	//Find first unactive Car in the pool
	for (AAICarPawn* PoolCarAI : CarsPool)
	{
		if (PoolCarAI && !PoolCarAI->IsActive())
		{
			PoolCarAI->Mesh->SetPhysicsLinearVelocity(FVector(0.f,0.f,0.f));
			PoolCarAI->TeleportTo(SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			PoolCarAI->SetActive(true);
			SpawnedPoolIndexes.Add(PoolCarAI->GetPoolIndex());
			
			SetOnSpawnTimer(AverageSpawnTime, Divergence);
			
			return;
		}
	}

	//All Cars in the pool are already active hence:
	UWorld* World = GetWorld();
	if (World)
	{
		CreateAndAddCarToPool(World, SpawnedPoolIndexes.Num(), true);
		SetOnSpawnTimer(AverageSpawnTime, Divergence);
	}
}

void ATrafficManager::OnDespawnCarAI(AAICarPawn* CarToDespawn)
{
	CarToDespawn->SetActive(false);
	SpawnedPoolIndexes.Remove(CarToDespawn->GetPoolIndex());

	UE_LOG(LogTemp, Warning, TEXT("We despawned the car : %s"), *CarToDespawn->GetName());
}

void ATrafficManager::CreateAndAddCarToPool(UWorld* World, int32 Index, bool bShouldActivateOnInitialization)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	auto SpawnedAICarActor = World->SpawnActor<AAICarPawn>(AICarClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnInfo);//AICarClass

	if (SpawnedAICarActor != nullptr)
	{
		SpawnedAICarActor->SetActive(bShouldActivateOnInitialization);
		SpawnedAICarActor->SetPoolIndex(Index);
		SpawnedAICarActor->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		SpawnedAICarActor->OnDespawnCar.AddDynamic(this, &ATrafficManager::OnDespawnCarAI);
		CarsPool.Add(SpawnedAICarActor);
		SpawnedPoolIndexes.Add(Index);

#if WITH_EDITOR //Organize Agents and AIControllers into folders
		SpawnedAICarActor->SetFolderPath("Agents/AI Cars");
		SpawnedAICarActor->Controller->SetFolderPath("Agents/AI Controllers");
#endif
	}
}

void ATrafficManager::SetOnSpawnTimer(float averageSpawnTime, float divergence)
{
	RandomRespawnTime = FMath::RandRange(averageSpawnTime - divergence, averageSpawnTime + divergence);

	GetWorldTimerManager().SetTimer(SpawnerTimerHandle, this, &ATrafficManager::OnSpawnCar, RandomRespawnTime, false);
}