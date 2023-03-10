// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Monster.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(BoxCollider);
	ProjectileMovementComponent->InitialSpeed = 1000.f;
	ProjectileMovementComponent->MaxSpeed = 1000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollider->SetRelativeScale3D(FVector(2.0f, 2.25f, 1.5f));
	BoxCollider->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);

}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Someone Out"));
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Enemy = Cast<AMonster>(OtherActor);
		if (Enemy)
		{
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(10, DamageEvent, PlayerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AProjectile::DestroyProjectile, 2.0f, true);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::DestroyProjectile()
{
	this->Destroy();
}

void AProjectile::CheckEnemy()
{
	
}



