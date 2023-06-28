//// Copyright, (c) Sami Kangasmaa 2022

#include "CoverSystemModule.h"

#define LOCTEXT_NAMESPACE "FAICoverSystemModule"

void FAICoverSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAICoverSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAICoverSystemModule, AICoverSystem)