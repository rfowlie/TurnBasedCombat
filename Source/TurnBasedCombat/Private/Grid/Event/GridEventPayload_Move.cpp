// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid/Event/GridEventPayload_Move.h"

UGridEventPayload_Move* UGridEventPayload_Move::CreatePayload(const FVector& Location)
{
	UGridEventPayload_Move* Payload = NewObject<UGridEventPayload_Move>();
	Payload->Location = Location;
	return Payload;
}

UGridEventPayload_Move::UGridEventPayload_Move()
{
}
