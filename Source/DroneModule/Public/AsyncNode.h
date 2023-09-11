// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncNode.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponsDeleget, int32, ID, FString, Data);


UCLASS()
class DRONEMODULE_API UAsyncNode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:


	UPROPERTY(BlueprintAssignable)
		FResponsDeleget OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FResponsDeleget OnFail;

	UPROPERTY()
	float m_AcceptanceRadius;

	UPROPERTY()
	AAIController* m_ctr;

	UPROPERTY()
		FVector m_MoveLoc;

	UPROPERTY()
		int temp = 0;

	void isEqual();

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncNode* DroneMoveTo(const int ID, AAIController* ctr, FVector loc,float p_AccRadius);

	virtual void Activate() override;

	/*UFUNCTION()
		void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);*/


};
