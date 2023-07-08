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

	// ���� ��ä�� ����� �׸���
	UFUNCTION(BlueprintCallable, Category = "Sense")
		void DrawCircleSector(float Radius, float StartAngle, float EndAngle, int32 NumSegments);

	bool IsPlayerInsideFanArea(float LocationRadius, float FanAngle, FVector FanDirection);

	// ��� ����
	UFUNCTION(BlueprintCallable, Category = "Sense")
		bool ShotSenseRange();
};
