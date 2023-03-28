// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFrameWork/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Monster.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "MyPlayer.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/Abilities/SM_Greystone_WhirlBase_01.SM_Greystone_WhirlBase_01'"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxCollider->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	BoxCollider->SetRelativeScale3D(FVector(2.0f, 2.25f, 1.5f));
	BoxCollider->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);


	//RootComponent = MeshComp;
	//BoxCollider->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(BoxCollider);
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;
	

	InitialLifeSpan = 1.f;

}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (IsOwnerPlayer)
		{
			auto Enemy = Cast<AMonster>(OtherActor);
			if (Enemy)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Fire : Skill Q Hit"));
				FDamageEvent DamageEvent;
				if (OwnerPlayer == nullptr)
				{
					auto MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					SetPlayer(MyPlayer);
				}
				Enemy->TakeDamage(10, DamageEvent, OwnerPlayer->GetController(), OwnerPlayer);
			}
		}
		else
		{
			auto TargetPlayer = Cast<AMyPlayer>(OtherActor);
			if (TargetPlayer)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player : Skill Hit "));
				FDamageEvent DamageEvent;
				//TargetPlayer->TakeDamage(10, DamageEvent, OwnerPlayer->GetController(), OwnerPlayer);
			}
		}
	}
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AProjectile::DestroyProjectile, 2.0f, true);
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



