#include "BaseUI_K2Nodes.h"

#include "FBaseUI_K2NodeFactory.h"

#define LOCTEXT_NAMESPACE "FBaseUI_K2NodesModule"

void FBaseUI_K2NodesModule::StartupModule()
{
	TSharedPtr<FBaseUI_K2NodeFactory> BaseUI_K2NodeFactoryPtr = MakeShareable(new FBaseUI_K2NodeFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(BaseUI_K2NodeFactoryPtr);
}

void FBaseUI_K2NodesModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBaseUI_K2NodesModule, BaseUI_K2Nodes)