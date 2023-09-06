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
		int temp = 0;

	void isEqual();

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncNode* AsyncFerox(const int ID);

	virtual void Activate() override;

};
