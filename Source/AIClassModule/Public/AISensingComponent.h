#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ST_Range.h"
#include "AISensingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AICLASSMODULE_API UAISensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAISensingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������ ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* AIRangeData;
	// ���� ������ ���̺�
	struct FST_Range* curAIRangeData;

	// ���� ��ä�� ����
	float CalculateSectorArea(float centralAngle, float radius);

	// ��� ����
	UFUNCTION(BlueprintCallable, Category = "Sense")
		bool ShotSenseRange();
};
