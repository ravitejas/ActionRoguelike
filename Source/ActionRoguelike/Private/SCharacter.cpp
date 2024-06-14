// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	AttrComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void ASCharacter::MoveForward(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	float FloatValue = Value.Get<float>();
	AddMovementInput(ControlRot.Vector(), FloatValue);
}

void ASCharacter::MoveBackward(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	float FloatValue = -1.0f * Value.Get<float>();
	AddMovementInput(ControlRot.Vector(), FloatValue);
}

void ASCharacter::MoveRight(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	float FloatValue = Value.Get<float>();
	AddMovementInput(RightVector, FloatValue);
}

void ASCharacter::MoveLeft(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	float FloatValue = -1.0f * Value.Get<float>();
	AddMovementInput(RightVector, FloatValue);
}

void ASCharacter::SuperAttack()
{
	FVector const HandLocation = GetMesh()->GetSocketLocation("Muzzle_01") + FVector(0, 0, 50);
	FTransform SpawnTM = FTransform(GetController()->GetControlRotation(), HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(SuperProjectileClass, SpawnTM, SpawnParams);
	GetMesh()->IgnoreActorWhenMoving(Projectile, true);
}

void ASCharacter::Teleport()
{
	if (ensure(TeleportProjectileClass))
	{
		FVector Offset = (GetController()->GetControlRotation().Vector() * 50.0f);
		FVector const HandLocation = GetMesh()->GetSocketLocation("Muzzle_01") + Offset;
		FTransform SpawnTM = FTransform(GetController()->GetControlRotation(), HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		AActor* Projectile = GetWorld()->SpawnActor<AActor>(TeleportProjectileClass, SpawnTM, SpawnParams);
		GetMesh()->IgnoreActorWhenMoving(Projectile, true);
	}
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	float AnimPlayTime = 0.15f;
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AnimPlayTime);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector const HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	auto const TraceStartPos = CameraComp->GetComponentLocation();
	auto const TraceDir = GetController()->GetControlRotation().Vector();
	auto const TraceLengthCM = 800.0f;
	auto const TraceSegment = TraceDir * TraceLengthCM;

	FVector const TraceTargetPos = TraceStartPos + TraceSegment;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.RemoveObjectTypesToQuery(ECC_Camera);
	QueryParams.RemoveObjectTypesToQuery(ECC_Pawn);
	FHitResult HitResult;
	bool const HitSomeObject = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStartPos, TraceTargetPos, QueryParams);
	FVector ProjectileTargetPos;
	if (HitSomeObject)
	{
		ProjectileTargetPos = HitResult.ImpactPoint;
		//DrawDebugLine(GetWorld(), TraceStartPos, ProjectileTargetPos, FColor::Green, true, 5.0f, 0, 2.f);
	}
	else
	{
		ProjectileTargetPos = TraceTargetPos;
		//DrawDebugLine(GetWorld(), TraceStartPos, TraceTargetPos, FColor::Red, true, 5.0f, 0, 2.f);
	}	
	
	//DrawDebugSphere(GetWorld(), ProjectileTargetPos, 50, 32, FColor::Yellow, true, 2.0f, 0, 2.f);
	FVector const ProjectileDir = (ProjectileTargetPos - HandLocation);
	FTransform SpawnTM = FTransform(ProjectileDir.Rotation(), HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	GetMesh()->IgnoreActorWhenMoving(Projectile, true);
}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SuperAttack", EInputEvent::IE_Pressed, this, &ASCharacter::SuperAttack);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Teleport", EInputEvent::IE_Pressed, this, &ASCharacter::Teleport);

	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input)
	{
		Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASCharacter::MoveForward);
		Input->BindAction(MoveBackwardAction, ETriggerEvent::Triggered, this, &ASCharacter::MoveBackward);
		Input->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASCharacter::MoveRight);
		Input->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &ASCharacter::MoveLeft);
	}
}