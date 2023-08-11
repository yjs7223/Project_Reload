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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool sensing;

	float AimFwd_Radius;
	float AimBwd_Radius;
	float AimSide_Radius;

	float AimFwd_Angle;
	float AimBwd_Angle;
	float AimSide_Angle;

	FTimerHandle sensingTimer;

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
		void DrawCircleSector(float Radius, float Angle, int32 NumSegments);

	bool IsPlayerInsideFanArea(float LocationRadius, float FanAngle, FVector FanDirection);

	// ��� ����
	UFUNCTION(BlueprintCallable, Category = "Sense")
		void ShotSenseRange();

	// �÷��̾���� ��ġ �˻�
	UFUNCTION(BlueprintCallable, Category = "Range")
		bool MinRangeCheck();

	void DrawSense();

	void SetDataTable(FName EnemyName);
};
