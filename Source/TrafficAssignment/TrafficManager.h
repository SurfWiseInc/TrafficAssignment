// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficManager.generated.h"

UCLASS()
class TRAFFICASSIGNMENT_API ATrafficManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrafficManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:


	//
	//	Respawn
	//
	FTimerHandle SpawnerTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Respawn")
		float AverageSpawnTime = 6.f;

	UPROPERTY(EditAnywhere, Category = "Respawn")
		float Divergence = 2.f;

	UPROPERTY()
		float RandomRespawnTime;

	UPROPERTY(EditAnywhere, Category = "Respawn")
		class USceneComponent* SpawnPoint;

	UFUNCTION()
		void SetOnSpawnTimer(float AvrSpawnTime, float Div);



	//Template Car
	UPROPERTY(EditAnywhere, Category = "CarsPool")
		TSubclassOf<class AAICarPawn> AICarClass;

	//
	//	Pool of cars	
	//
	UPROPERTY(VisibleAnywhere, Category = "CarsPool")
		TArray<class AAICarPawn*> CarsPool;

	TArray<int32> SpawnedPoolIndexes;

	UPROPERTY(EditAnywhere)
		int32 InitialPoolSize;

	UFUNCTION()
		void InitializePool();

	UFUNCTION()
		void CreateAndAddCarToPool(UWorld* World, int32 Index, bool bShouldActiateOnInitialization);
	
	UFUNCTION()
		void OnSpawnCar();

	UFUNCTION()
		void OnDespawnCarAI(AAICarPawn* CarToDespawn);



};
