// Fill out your copyright notice in the Description page of Project Settings.


#include "AICarPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "DisplayDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "CollisionShape.h"

AAICarPawn::AAICarPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAICarPawn::BeginPlay()
{
	Super::BeginPlay();

	RandThrottleValue = FMath::RandRange(0.5, 1.0);
	ThrottleValue = RandThrottleValue;  //So that each car rides with different speed.

	auto WheeledMovemendComp = Cast<UChaosWheeledVehicleMovementComponent>(VehicleMovementComponent);
	WheeledMovemendComp->EnableMechanicalSim(true);
}

void AAICarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ScanForObstacles();
	SetInput();
}

void AAICarPawn::SetInput()
{
	VehicleMovementComponent->SetThrottleInput(ThrottleValue);
	VehicleMovementComponent->SetBrakeInput(BrakeValue);
}

void AAICarPawn::SetActive(bool isActive)
{
	bIsActive = isActive;
	SetActorHiddenInGame(!isActive);
	SetActorEnableCollision(isActive);
	this->SetActorTickEnabled(isActive);
}

void AAICarPawn::SetPoolIndex(int32 Index)
{
	PoolIndex = Index;
}

bool AAICarPawn::IsActive()
{
	return bIsActive;
}

int32 AAICarPawn::GetPoolIndex()
{
	return PoolIndex;
}

void AAICarPawn::DespawnAICar(AAICarPawn* CarToDespawn)
{
	if (CarToDespawn != nullptr)
	{
		//Traffic Manager handles despawning and respawning.
		OnDespawnCar.Broadcast(CarToDespawn); 
	}
}

void AAICarPawn::ScanForObstacles()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult HitResult;
		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(200.f);
		bool isHit = World->SweepSingleByChannel(HitResult,
									GetActorLocation() + GetActorForwardVector() * 500.f,
									GetActorLocation() + GetActorForwardVector() * 50000.f,
									FQuat::Identity,
									ECC_GameTraceChannel1,
									MyColSphere);

		if(isHit && (HitResult.Distance < SafeDistance) )
		{
			BrakeValue = BrakeCoeff * (SafeDistance - HitResult.Distance) / SafeDistance;
		}
		else
		{
			BrakeValue = 0.f;
		}
	}
}
