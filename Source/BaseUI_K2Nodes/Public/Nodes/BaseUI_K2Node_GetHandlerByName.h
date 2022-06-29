// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "BaseUI_K2Node_GetHandlerByName.generated.h"

/**
 * 
 */
UCLASS()
class BASEUI_K2NODES_API UBaseUI_K2Node_GetHandlerByName : public UK2Node
{
	GENERATED_BODY()
	
public:
	UBaseUI_K2Node_GetHandlerByName(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	{
		NodeTooltip = NSLOCTEXT("UBaseUI_K2Node_GetHandlerByName", "NodeTooltip", "Get UObject property value by name");
	}

	//  ~ Begin UEdGraphNode Interface.
	// Basic information functions
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return NSLOCTEXT("K2Node", "UBaseUI_K2Node_GetHandlerByName", "Get Handler Object By Name"); }
	virtual FText GetTooltipText() const override { return NodeTooltip; }
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override { static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon"); OutColor = GetNodeTitleColor(); return Icon; }

	// Main node construction functions.
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	// Update node function.
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual void PostReconstructNode() override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override { return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities); }
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	//~ End UK2Node Interface

	/** Get the then output pin */
	UEdGraphPin* GetThenPin() const { return Pins[1]; }
	/** Get the Failed output pin */
	UEdGraphPin* GetFailedPin() const { return Pins[2]; }
	/** Get the Data Table input pin */
	// UEdGraphPin* GetObjectPin() const { return Pins[3]; }
	/** Get the spawn transform input pin */
	UEdGraphPin* GetHandlerNamePin() const { return Pins[3]; }
	/** Get the result output pin */
	UEdGraphPin* GetHandlerObjPtrPin() const { return Pins[4]; }
	/** Get K2Node Call Function */
	UK2Node_CallFunction* GetK2NodeDstCallFunction(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph);
	
	UClass* GetHandlerObjectType();
	void RefreshOutputPinType();
	void SetReturnTypeForStruct(UClass* NewRowStruct);
private:
	/**
	 * Takes the specified "MutatablePin" and sets its 'PinToolTip' field (according
	 * to the specified description)
	 *
	 * @param   MutablePin	The pin you want to set tool-tip text on
	 * @param   PinDescription	A string describing the pin's purpose
	 */
	void SetPinToolTip(UEdGraphPin& MutablePin, const FText& PinDescription) const;

	/** Propagates Value pin type to the between the input and output pins */
	void PropagateValuePinType();

	/** Tooltip text for this node. */
	FText NodeTooltip;
};
