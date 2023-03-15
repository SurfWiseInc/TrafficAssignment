// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "AICarPawn.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnCarSignature, AAICarPawn*, ThisCarPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDespawnCarSignature, AAICarPawn*, ThisCarPawn);
/**
 * 
 */
UCLASS()
class TRAFFICASSIGNMENT_API AAICarPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()


	AAICarPawn(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY()
	FOnSpawnCarSignature OnSpawnCar;

	UPROPERTY()
	FOnDespawnCarSignature OnDespawnCar;
	
	//
	//	Pool Functionality
	//
	void SetActive(bool isActive);
	void SetPoolIndex(int32 poolIndex);
	bool IsActive();
	int32 GetPoolIndex();
private:

	bool bIsActive;
	int32 PoolIndex;

	//
	//	Control
	//
	UPROPERTY(VisibleAnywhere, Category = "AIController")
		float ThrottleValue;

	UPROPERTY(VisibleAnywhere, Category = "AIController")
		float BrakeValue;

	UPROPERTY()
		float RandThrottleValue;

	UFUNCTION()
		void SetInput();

	UFUNCTION()
		void ScanForObstacles();

	UPROPERTY(EditAnywhere, Category = "AIController")
		float SafeDistance;

	UPROPERTY(EditAnywhere, Category = "AIController")
		float BrakeCoeff;

public:
	UFUNCTION()
		void DespawnAICar(AAICarPawn* CarToDespawn);


};
