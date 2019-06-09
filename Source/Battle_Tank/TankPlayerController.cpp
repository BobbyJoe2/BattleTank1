// Copyright Ethan Walz

#include "TankPlayerController.h"
#include "Battle_Tank.h"


ATank* ATankPlayerController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}


