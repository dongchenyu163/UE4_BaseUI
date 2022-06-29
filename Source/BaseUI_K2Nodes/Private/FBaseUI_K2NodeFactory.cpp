// Fill out your copyright notice in the Description page of Project Settings.


#include "FBaseUI_K2NodeFactory.h"
#include "NodeWidgets/SDongGraphPinNameList.h"
#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceConfig.h"
#include "Nodes/BaseUI_K2Node_GetHandlerByName.h"

TSharedPtr<SGraphPin> FBaseUI_K2NodeFactory::CreatePin(UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Name)
	{
		UObject* Outer = InPin->GetOuter();

		/*if (Outer->IsA(UK2Node_SetDataTableRow::StaticClass()))
		//{
		//	const UK2Node_SetDataTableRow* SetDataTableRowNode = CastChecked<UK2Node_SetDataTableRow>(Outer);
		//	// Create drop down combo boxes for DataTable and CurveTable RowName pins
		//	const UEdGraphPin* DataTablePin = SetDataTableRowNode->GetDataTablePin();
		//	if (DataTablePin)
		//	{
		//		if (DataTablePin->DefaultObject != nullptr && DataTablePin->LinkedTo.Num() == 0)
		//		{
		//			if (auto DataTable = Cast<UDataTable>(DataTablePin->DefaultObject))
		//			{
		//				return SNew(SGraphPinDataTableRowName, InPin, DataTable);
		//			}
		//		}
		//	}
		//}
		//else */
		if (Outer->IsA(UBaseUI_K2Node_GetHandlerByName::StaticClass()))
		{
			const UBaseUI_K2Node_GetHandlerByName* ArraySortNode = CastChecked<UBaseUI_K2Node_GetHandlerByName>(Outer);
			UEdGraphPin* ArrayPin = ArraySortNode->GetHandlerNamePin();
			if (ArrayPin)
			{
				TArray<FName> PropertyNames = UMainMenuGameInstanceConfig::Get()->HandlerNameList;
				return SNew(SDongGraphPinNameList, ArrayPin, PropertyNames);
			}
		}
	}
	return nullptr;
}
