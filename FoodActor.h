// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "FoodActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FoodPickUpDelegate, AActor*, Eater);

UCLASS()

class SNAKEGAME_API AFoodActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFoodActor();

	
	UPROPERTY(BlueprintAssignable)
	FoodPickUpDelegate OnFoodEaten;

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	UFUNCTION()
	void OnSnakeCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpuls ,const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* PickUpEffect;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	USoundBase* EatAppleSound;
};
