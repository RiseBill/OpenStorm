#include "LocationMarker.h"


#include "UObject/Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Camera/CameraComponent.h"
#include "../EngineHelpers/StringUtils.h"

ALocationMarker::ALocationMarker()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	UStaticMesh* mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	UMaterial* material = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Materials/TestImageMaterial.TestImageMaterial'")).Object;

	meshComponent->SetStaticMesh(mesh);
	meshComponent->SetMaterial(0, material);
	meshComponent->SetRelativeScale3D(FVector(0.05, 0.05, 0.05));
	
	textComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	textComponent->SetTextMaterial(ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Engine/EditorResources/FieldNodes/_Resources/DefaultTextMaterialOpaque.DefaultTextMaterialOpaque'")).Object);
	textComponent->SetHorizontalAlignment(EHTA_Center);
	textComponent->SetWorldSize(20);
	textComponent->SetRelativeRotation(FRotator(0, 180, 0));

	textComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	meshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	
}


void ALocationMarker::BeginPlay() {
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();
}

void ALocationMarker::Tick(float DeltaTime) {
	auto camera = Cast<UCameraComponent>(GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
	if (camera != NULL) {
		//FVector playerLocation = camera->GetComponentLocation();
		//FVector selfLocation = GetActorLocation();
		//FRotator rotation = FRotationMatrix::MakeFromX(playerLocation - selfLocation).Rotator();
		//SetActorRotation(rotation);
		SetActorRotation(camera->GetComponentRotation());
	}
}

void ALocationMarker::SetText(std::string text) {
	textComponent->SetText(FText::FromString(StringUtils::STDStringToFString(text)));
}

