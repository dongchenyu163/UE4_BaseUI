// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
// #include "FBaseUI_K2NodeFactory.generated.h"

class SGraphPin;

/**
 * 
 */
class BASEUI_K2NODES_API FBaseUI_K2NodeFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override;
};
