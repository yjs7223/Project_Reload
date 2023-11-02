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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		float Target_MinRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		float AimFwd_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		float AimBwd_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		float AimSide_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
    	float AimFwd_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		float AimBwd_Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		float AimSide_Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensing")
		class AAI_Controller* AIController;

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
	/*UFUNCTION(BlueprintCallable, Category = "Sense")
		void DrawCircleSector(float Radius, float Angle, int32 NumSegments);*/

	bool IsPlayerInsideFanArea(float LocationRadius, float FanAngle, FVector FanDirection);

	// ��� ����
	UFUNCTION(BlueprintCallable, Category = "Sense")
		void ShotSenseRange();

	// �÷��̾���� ��ġ �˻�
	UFUNCTION(BlueprintCallable, Category = "Range")
		bool MinRangeCheck();

	//void DrawSense();

	void SetDataTable(const FName EnemyName);
};
