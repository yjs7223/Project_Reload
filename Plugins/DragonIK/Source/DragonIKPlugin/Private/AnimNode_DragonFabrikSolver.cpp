/* Copyright (C) Eternal Monke Games - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Mansoor Pathiyanthra <codehawk64@gmail.com , mansoor@eternalmonke.com>, 2021
*/


#include "AnimNode_DragonFabrikSolver.h"
#include "DragonIKPlugin.h"
#include "Animation/AnimInstanceProxy.h"
#include "DrawDebugHelpers.h"
#include "AnimationRuntime.h"
#include "AnimationCoreLibrary.h"
#include "Algo/Reverse.h"






// Initialize the component pose as well as defining the owning skeleton
void FAnimNode_DragonFabrikSolver::Initialize_AnyThread(const FAnimationInitializeContext & Context)
{
	FAnimNode_DragonControlBase::Initialize_AnyThread(Context);
	owning_skel = Context.AnimInstanceProxy->GetSkelMeshComponent();


}











void FAnimNode_DragonFabrikSolver::Evaluate_AnyThread(FPoseContext & Output)
{
}





void FAnimNode_DragonFabrikSolver::ConditionalDebugDraw(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* PreviewSkelMeshComp) const
{
	
#if WITH_EDITORONLY_DATA
	if (PreviewSkelMeshComp && PreviewSkelMeshComp->GetWorld())
	{
		

	}
#endif

}

//Perform update operations inside this
void FAnimNode_DragonFabrikSolver::UpdateInternal(const FAnimationUpdateContext & Context)
{
	FAnimNode_DragonControlBase::UpdateInternal(Context);

}



//Nothing would be needed here
void FAnimNode_DragonFabrikSolver::EvaluateComponentSpaceInternal(FComponentSpacePoseContext & Context)
{
}

FAnimNode_DragonFabrikSolver::FAnimNode_DragonFabrikSolver()
{
//	FAnimNode_DragonFabrikSolver();

	FabrikBoneTransforms = TArray<FBoneTransform>();
}

void FAnimNode_DragonFabrikSolver::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{

	FTransform Target_Temp_Transform = Target_Transform;

	if (first_start)
	{
		FabrikBoneTransforms.Empty();
		FabrikBoneTransforms_duplicate.Empty();

		Target_Temp_Transform = FTransform::Identity;
	}

	UDragonIK_Library::Evaluate_CCDIK_Modified(first_start, Output, owning_skel, StartSplineBone, EndSplineBone, Precision, MaxIterations, true, false, Target_Transform, FabrikBoneTransforms, FabrikBoneTransforms_duplicate);



//	first_start = false;


//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "- FabrikBoneTransforms_duplicate Feet : " + FString::SanitizeFloat(FabrikBoneTransforms_duplicate.Num()));
//FABRIK_BodySystem(Output.Pose, Output, OutBoneTransforms);	
//	OutBoneTransforms = TArray<FBoneTransform>();





	for (int32 i = 0; i < FabrikBoneTransforms.Num(); i++)
	{
		OutBoneTransforms.Add(FabrikBoneTransforms[i]);

		if (i < FabrikBoneTransforms.Num() - 1)
		{
			/*
			FVector LookAt_Axis = FVector(1, 0, 0);
			FAxis LookatAxis_Temp;
			LookatAxis_Temp.bInLocalSpace = false;
			LookatAxis_Temp.Axis = LookAt_Axis.GetSafeNormal();

			FTransform comp_trans = FabrikBoneTransforms[i].Transform;
			// lookat vector
			FVector LookAtVector = LookatAxis_Temp.GetTransformedAxis(FabrikBoneTransforms[i].Transform).GetSafeNormal();

			FVector NewDir = (FabrikBoneTransforms[i + 1].Transform.GetLocation() - FabrikBoneTransforms[i].Transform.GetLocation()).GetUnsafeNormal();

			FQuat Rot_Diff = FQuat::FindBetween(LookAtVector, NewDir).GetNormalized();
			OutBoneTransforms[i].Transform.SetRotation(Rot_Diff * OutBoneTransforms[i].Transform.GetRotation());
			*/



		}

	}	




	for (int32 i = 0; i < OutBoneTransforms.Num()-1; i++)
	{

	//	FVector Axis_Vector = Quaternion.Inverse(bones[i].solverRotation) * (bones[i + 1].solverPosition - bones[i].solverPosition);

		/*
		FTransform CurrentBone = Output.Pose.GetComponentSpaceTransform(OutBoneTransforms[i].BoneIndex);
		FTransform childBone = Output.Pose.GetComponentSpaceTransform(OutBoneTransforms[i + 1].BoneIndex);

		FVector Axis_Vector = CurrentBone.GetRotation().Inverse() * (childBone.GetLocation() - CurrentBone.GetLocation());

		FQuat r = FQuat::FindBetween(OutBoneTransforms[i].Transform.GetRotation() * Axis_Vector, OutBoneTransforms[i + 1].Transform.GetLocation() - OutBoneTransforms[i].Transform.GetLocation());

		OutBoneTransforms[i].Transform.SetRotation(r * OutBoneTransforms[i].Transform.GetRotation());
		*/
		/*
		if (FabrikBoneTransforms_duplicate.Num() > 0)
		{
			//FVector OldDir = (Output.Pose.GetComponentSpaceTransform(OutBoneTransforms[i + 1].BoneIndex).GetLocation() - Output.Pose.GetComponentSpaceTransform(OutBoneTransforms[i].BoneIndex).GetLocation()).GetSafeNormal();
			FVector OldDir = (FabrikBoneTransforms_duplicate[i + 1].Transform.GetLocation() - FabrikBoneTransforms_duplicate[i].Transform.GetLocation()).GetSafeNormal();

			FVector NewDir = (OutBoneTransforms[i + 1].Transform.GetLocation() - OutBoneTransforms[i].Transform.GetLocation()).GetSafeNormal();


			FVector const RotationAxis = FVector::CrossProduct(OldDir, NewDir).GetSafeNormal();
			float const RotationAngle = FMath::Acos(FVector::DotProduct(OldDir, NewDir));
			FQuat const DeltaRotation = FQuat(RotationAxis, RotationAngle);
			OutBoneTransforms[i].Transform.SetRotation(DeltaRotation * OutBoneTransforms[i].Transform.GetRotation());

		}*/

	}

	FabrikBoneTransforms_duplicate = FabrikBoneTransforms;


}





bool FAnimNode_DragonFabrikSolver::IsValidToEvaluate(const USkeleton * Skeleton, const FBoneContainer & RequiredBones)
{

	//return true;
	//return (EndSplineBone.IsValidToEvaluate(RequiredBones) && StartSplineBone.IsValidToEvaluate(RequiredBones) && RequiredBones.IsValid() && (RequiredBones.BoneIsChildOf(EndSplineBone.BoneIndex, StartSplineBone.BoneIndex)));

	return (EndSplineBone.IsValidToEvaluate(RequiredBones) && StartSplineBone.IsValidToEvaluate(RequiredBones));

}


// SPLINE IK CODE INITIALIZATION CODE


// SPLINE IK CODE END








void FAnimNode_DragonFabrikSolver::InitializeBoneReferences(FBoneContainer & RequiredBones)
{


	SavedBoneContainer = &RequiredBones;

	EndSplineBone.Initialize(*SavedBoneContainer);
	StartSplineBone.Initialize(*SavedBoneContainer);

	
}




void FAnimNode_DragonFabrikSolver::FABRIK_BodySystem(FCSPose<FCompactPose>& MeshBases, FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{


	
}








