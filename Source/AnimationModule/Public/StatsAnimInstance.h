// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Engine/DataTable.h"
#include "StatsAnimInstance.generated.h"
/**
 *
 */
USTRUCT(Atomic, BlueprintType)
struct FStatAnimationTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsVaild();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Back;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Front;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Back_Die;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Front_Die;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Left_Die;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KnockBack_Right_Die;
};

/**
 * 
 */
UCLASS()
class ANIMATIONMODULE_API UStatsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UStatsAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UWeaponAnimInstance();

	void KnockBackEnd(UAnimMontage* Montage, bool bInterrupted);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	ACharacter* owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	class UStatComponent* mStats;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "PakurComp"))
	TObjectPtr<class UActorComponent> m_PakurComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	bool mIsDie;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	bool mIsHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	bool m_IsStun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	bool m_IsKnockback;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	float m_HitX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	float m_HitY;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = animation)
	float m_HitReactionScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* m_AnimationTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	struct FStatAnimationTable m_CurrentAnimation;
};
