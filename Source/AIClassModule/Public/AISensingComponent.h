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

	// 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* AIRangeData;
	// 현재 데이터 테이블
	struct FST_Range* curAIRangeData;

	// 원의 부채꼴 계산기
	float CalculateSectorArea(float centralAngle, float radius);

	// 사격 센서
	UFUNCTION(BlueprintCallable, Category = "Sense")
		bool ShotSenseRange();
};
