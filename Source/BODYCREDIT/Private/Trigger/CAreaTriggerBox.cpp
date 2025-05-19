// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/CAreaTriggerBox.h"
#include "Components/BoxComponent.h"
#include "global.h"

// Sets default values
ACAreaTriggerBox::ACAreaTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxComp, "BoxComp");
}

// Called when the game starts or when spawned
void ACAreaTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACAreaTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

