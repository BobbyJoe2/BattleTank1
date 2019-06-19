// Copyright Ethan Walz

#include "TankPlayerController.h"
#include "Gameframework/Actor.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Engine/World.h"
#include "Tank.h"
#include "Battle_Tank.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayController not possesing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s"), *(ControlledTank->GetName()));
	}
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();

}

ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair() {
	if (!GetControlledTank()) { return; }

	FVector HitLocation; //Out parameter

	if (GetSightRayHitLocation(HitLocation)) { // has side effect is going to line trace
		GetControlledTank()->AimAt(HitLocation);

		//TODO tell controlled tank to aim at this point
	}

		
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const {
	//find crosshair projection
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(CrossHairXLocation * ViewportSizeX, CrossHairYLocation * ViewportSizeY);

	//"De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;

	if(GetLookDirection(ScreenLocation, LookDirection)){
		//line-trace along that look direction, and see what we hit (up to max range)
		GetLookVectorHitLocation(LookDirection, HitLocation);

	}

	

	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector CameraWorldLocation; // to be discarded

	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const {
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * Reach);
	
		if(GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_PhysicsBody
		))
	{
		HitLocation = HitResult.Location;
		return true;
	}
		HitLocation = FVector(0);

		return false;


	auto hitResult = HitResult;
}