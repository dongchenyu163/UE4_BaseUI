// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/BaseUI_K2Node_GetHandlerByName.h"

#include "BaseClassesAndTypes/BaseUI_Static.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_IfThenElse.h"
#include "KismetCompiler.h"
#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceConfig.h"

#define LOCTEXT_NAMESPACE "UBaseUI_K2Node_GetHandlerByName"

namespace GetPropertyHelper
{
	const FName OutputPinName_HandlerObject = "Handler Object";
	const FName InputPinName_HandlerName = "HandlerName";
	const FName OutputExecPinName_Failed = "Failed";
	// const FName OutValuePinName = "Value";
	
	const FName FunctionInputParamName_HandlerName = "InHandlerName";
	const FName FunctionOutputParamName_HandlerPtr = "OutFoundedHandler";
}

void UBaseUI_K2Node_GetHandlerByName::AllocateDefaultPins()
{
	// const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetPropertyHelper::OutputExecPinName_Failed);

	// Add Target Object pin
	// UEdGraphPin* ObjectPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), GetPropertyHelper::OutputPinName_HandlerObject);
	// SetPinToolTip(*ObjectPin, LOCTEXT("ObjectPinDescription", "Object that owns this property"));

	// PropertyName Pin
	UEdGraphPin* HandlerNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetPropertyHelper::InputPinName_HandlerName);
	SetPinToolTip(*HandlerNamePin, LOCTEXT("HandlerNamePinDescription", "Choose a handler name to get"));

	// OutValue pin
	UEdGraphPin* HandlerObjectPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, GetPropertyHelper::OutputPinName_HandlerObject);
	SetPinToolTip(*HandlerObjectPin, LOCTEXT("HandlerObjectPinDescription", "The handler object ref you get, if found"));

	// Super::AllocateDefaultPins();
}

void UBaseUI_K2Node_GetHandlerByName::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* SrcValuePin = GetHandlerObjPtrPin();
	UK2Node_CallFunction* DstCallFunction = GetK2NodeDstCallFunction(CompilerContext, SourceGraph);
	if (UNLIKELY(!DstCallFunction))
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("GetUObjectPropertyByName_Error", "DstCallFunction is NULL.").ToString(), this);
		// we break exec links so this is the only error we get
		BreakAllNodeLinks();
		return;
	}

	// NODE EXEC INPUT: 
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(DstCallFunction->GetExecPin()));

	// DATA INPUT: Handler name input pin
	UEdGraphPin* FunctionInputPin_HandlerName = DstCallFunction->FindPinChecked(GetPropertyHelper::FunctionInputParamName_HandlerName);
	CompilerContext.MovePinLinksToIntermediate(*GetHandlerNamePin(), *FunctionInputPin_HandlerName);

	// DATA OUTPUT: Returned handler object pointer
	UEdGraphPin* FunctionOutputPin_HandlerObject = DstCallFunction->FindPinChecked(GetPropertyHelper::FunctionOutputParamName_HandlerPtr);
	CompilerContext.MovePinLinksToIntermediate(*GetHandlerObjPtrPin(), *FunctionOutputPin_HandlerObject);
	// Handling handler object pin auto change type.
	UEdGraphPin*& DstValuePin = FunctionOutputPin_HandlerObject;
	DstValuePin->PinType = SrcValuePin->PinType;
	DstValuePin->PinType.PinSubCategoryObject = SrcValuePin->PinType.PinSubCategoryObject;
	CompilerContext.MovePinLinksToIntermediate(*SrcValuePin, *DstValuePin);

	// NODE EXEC OUTPUT:FunctionOutputParamName_HandlerPtr

	// Prepare variables
	UEdGraphPin* DstFunctionThenPin = DstCallFunction->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	UEdGraphPin* DstFunctionReturnPin = DstCallFunction->GetReturnValuePin();
	UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	BranchNode->AllocateDefaultPins();

	// Link execution pins
	// BRANCH EXEC INPUT:
	DstFunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
	// BRANCH EXEC OUTPUT:
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
	CompilerContext.MovePinLinksToIntermediate(*GetFailedPin(), *(BranchNode->GetElsePin()));

	// Handling the branch condition
	DstFunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());


	// Final.
	BreakAllNodeLinks();
}

void UBaseUI_K2Node_GetHandlerByName::PostReconstructNode()
{  // Open asset
	Super::PostReconstructNode();
	RefreshOutputPinType();
}

void UBaseUI_K2Node_GetHandlerByName::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	if (Pin && Pin->PinName == GetPropertyHelper::InputPinName_HandlerName)
	{
		RefreshOutputPinType();
	}
}

UClass* UBaseUI_K2Node_GetHandlerByName::GetHandlerObjectType()
{
	UEdGraphPin* HandlerNamePin = GetHandlerNamePin();
	const FName HandlerName(HandlerNamePin->DefaultValue);
	const FFunctionHandlerInfo* FoundedHandlerInfoPtr = UMainMenuGameInstanceConfig::Get()->HandlerClassDict.Find(HandlerName);
	if (UNLIKELY(!FoundedHandlerInfoPtr))
	{
		UE_LOG(LogTemp, Error, TEXT("Function:[%s] INVALID handler name: [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *HandlerNamePin->DefaultValue);
		return nullptr;
	}
	return (*FoundedHandlerInfoPtr).HandlerClass;
}

void UBaseUI_K2Node_GetHandlerByName::RefreshOutputPinType()
{
	UClass* OutputType = GetHandlerObjectType();
	SetReturnTypeForStruct(OutputType);
}

void UBaseUI_K2Node_GetHandlerByName::SetReturnTypeForStruct(UClass* NewRowStruct)
{
	UClass* OldRowStruct = Cast<UClass>(GetHandlerObjPtrPin()->PinType.PinSubCategoryObject.Get());
	if (NewRowStruct != OldRowStruct)
	{
		UEdGraphPin* ResultPin = GetHandlerObjPtrPin();

		if (ResultPin->SubPins.Num() > 0)
		{
			GetSchema()->RecombinePin(ResultPin);
		}

		// NOTE: purposefully not disconnecting the ResultPin (even though it changed type)... we want the user to see the old
		//       connections, and incompatible connections will produce an error (plus, some super-struct connections may still be valid)
		ResultPin->PinType.PinSubCategoryObject = NewRowStruct;
		ResultPin->PinType.PinCategory = (NewRowStruct == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Object;
	}
}

void UBaseUI_K2Node_GetHandlerByName::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UBaseUI_K2Node_GetHandlerByName::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);
}

UK2Node_CallFunction* UBaseUI_K2Node_GetHandlerByName::GetK2NodeDstCallFunction(FKismetCompilerContext& CompilerContext,
	UEdGraph* SourceGraph)
{
	UK2Node_CallFunction* DstCallFunction = nullptr;
	FName FunctionName = GET_FUNCTION_NAME_CHECKED(UBaseUI_Static, FindHandlerByName);
	DstCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	DstCallFunction->FunctionReference.SetExternalMember(FunctionName, UBaseUI_Static::StaticClass());
	DstCallFunction->AllocateDefaultPins();

	return DstCallFunction;
}

void UBaseUI_K2Node_GetHandlerByName::SetPinToolTip(UEdGraphPin& MutablePin, const FText& PinDescription) const
{
	MutablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutablePin.PinToolTip += TEXT(" ");
		MutablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutablePin).ToString();
	}
	MutablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

void UBaseUI_K2Node_GetHandlerByName::PropagateValuePinType()
{
	UEdGraphPin* ValuePin = GetHandlerObjPtrPin();
	if (ValuePin->LinkedTo.Num() > 0 && ValuePin->LinkedTo[0]->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
	{
		UEdGraphPin* ValueLinkedPin = ValuePin->LinkedTo[0];
		ValuePin->PinType = ValueLinkedPin->PinType;
	}
	else
	{
		ValuePin->ResetDefaultValue();
		ValuePin->PinType.ContainerType = EPinContainerType::None;
		ValuePin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	}
}
