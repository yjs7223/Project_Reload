/* Copyright (C) Eternal Monke Games - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Mansoor Pathiyanthra <codehawk64@gmail.com , mansoor@eternalmonke.com>, 2021
*/

#include "DragonIKPluginEditor.h"
#include "DragonFootSolverEditMode.h"
#include "DragonAimSolverEditMode.h"
//#include "DragonFabrikSolverEditMode.h"
#include "DragonWarpSolverEditMode.h"


#define LOCTEXT_NAMESPACE "FDragonIKPluginEditorModule"

void FDragonIKPluginEditorModule::StartupModule()
{
#if WITH_EDITOR
	FEditorModeRegistry::Get().RegisterMode<FDragonControlBaseEditMode>(DragonControlBaseEditModes::DragonControlBase, LOCTEXT("AnimNode_DragonAimSolverEditModes", "AnimNode_DragonAimSolver"), FSlateIcon(), false);
	FEditorModeRegistry::Get().RegisterMode<FDragonFootSolverEditMode>(DragonFootSolverEditModes::DragonFootSolver, LOCTEXT("DragonFootSolverEditMode", "DragonFootSolver"), FSlateIcon(), false);
	FEditorModeRegistry::Get().RegisterMode<FDragonAimSolverEditMode>(DragonAimSolverEditModes::DragonAimSolver, LOCTEXT("DragonAimSolverEditMode", "DragonAimSolver"), FSlateIcon(), false);
//	FEditorModeRegistry::Get().RegisterMode<FDragonFabrikSolverEditMode>(DragonFabrikSolverEditModes::DragonFabrikSolver, LOCTEXT("DragonFabrikSolverEditMode", "DragonFabrikSolver"), FSlateIcon(), false);
	FEditorModeRegistry::Get().RegisterMode<FDragonWarpSolverEditMode>(DragonWarpSolverEditModes::DragonWarpSolver, LOCTEXT("DragonWarpSolverEditMode", "DragonWarpSolver"), FSlateIcon(), false);
#endif

	
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDragonIKPluginEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDragonIKPluginEditorModule, DragonIKPluginEditor)

//IMPLEMENT_MODULE(FDragonIKPluginEditorModule, MyEditorPlugin)