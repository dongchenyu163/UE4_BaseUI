#include "BaseUI_Module.h"

DEFINE_LOG_CATEGORY(BaseUI_Module);

#define LOCTEXT_NAMESPACE "FBaseUI_Module"

void FBaseUI_Module::StartupModule()
{
	UE_LOG(BaseUI_Module, Warning, TEXT("BaseUI_Module module has been loaded"));
}

void FBaseUI_Module::ShutdownModule()
{
	UE_LOG(BaseUI_Module, Warning, TEXT("BaseUI_Module module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBaseUI_Module, BaseUI_Module)