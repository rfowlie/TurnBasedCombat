//Copyright 2022~2024 DevGrain. All Rights Reserved.

#include "JointNative.h"

#define LOCTEXT_NAMESPACE "FJointModule"

void FJointNativeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FJointNativeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FJointNativeModule, JointNative)

