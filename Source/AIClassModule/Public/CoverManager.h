#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "CoverManager.generated.h"


/**
 *
 */
UCLASS()
class AICLASSMODULE_API ACoverManager : public AInfo
{
	GENERATED_BODY()


public:
	ACoverManager();
	virtual void BeginPlay() override;

	bool ChangeEncounter();
public:

	class ACoverSystem* CoverSystem;

	//Commander
	class AAICommander* commander;
	class ASubEncounterSpace* NowSub;


protected:


private:

};