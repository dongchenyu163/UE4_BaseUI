// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "FunctionHandlerInfo.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailCustomization.h"
#include "IDetailCustomNodeBuilder.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Widgets/SCompoundWidget.h"

class FDongPropertyRowBuilder : public IDetailCustomNodeBuilder
{
public:
	static TSharedRef<class IDetailCustomNodeBuilder> MakeInstance()
	{
		static TSharedRef<class IDetailCustomNodeBuilder> ObjectPtr = MakeShareable(new FDongPropertyRowBuilder);
		return ObjectPtr;
	}
	
	virtual void SetOnRebuildChildren(FSimpleDelegate InOnRegenerateChildren) override { OnRegenerateChildren = InOnRegenerateChildren; }
	virtual void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override;
	virtual void GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder) override;
	virtual void Tick(float DeltaTime) override {}
	virtual bool RequiresTick() const override { return false; }
	virtual bool InitiallyCollapsed() const override { return false; }
	virtual FName GetName() const override { return FName("FDongPropertyRowBuilder"); }

	FSimpleDelegate OnRegenerateChildren;
};

class BASEUI_MODULE_API FFunctionHandlerPropertyCustomizer : public IPropertyTypeCustomization
{
public:
	static TSharedRef<class IPropertyTypeCustomization> MakeInstance();
	void Handle_OnHandlerClassChanged();

	~FFunctionHandlerPropertyCustomizer()
	{

	}

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	                             IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		// PropertyHandle->
		// HeaderRow->
		// CustomizationUtils-> // 没啥用，只有字体什么的设置
		
		// HeaderRow
		// .NameContent()
		// [
		// 	SNew(STextBlock)
		// 	.Text(FText::FromString("CustomizeHeader Slot 1"))
		// ]
		// .ValueContent()
		// [
		// 	SNew(STextBlock)
		// 	.Text(FText::FromString("CustomizeHeader Slot 2"))
		// ];

		// UE_LOG(LogTemp, Display, TEXT("Function:[%s] Got property [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyHandle->GetProperty()->GetName());
	}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	void MakeNewDependentHandlerSelectionWidget(TSharedRef<IPropertyHandle> PropertyHandle,
														   IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	void Handler_OnConfigChanged();

	void Update_ValidHandlerName_ByClass(UClass* InClass, const ::FString& InCurrentHandlerPurposeString, TArray<TSharedPtr<FString>>& OutFilteredHandlerName);
	static TMap<FString, TArray<TSharedPtr<FString>>> Map_HandlerNamePurpose_To_ValidDependentNameList;
	static TMap<FString, TSharedPtr<STextComboBox>> Map_HandlerNamePurpose_To_ComboBoxPtr;
	// static TArray<TSharedPtr<STextComboBox>> ComboBoxPtrList;
	static int CreationCount;
	TArray<TSharedPtr<STextComboBox>> SelfComboBoxPtrList;

	FString SelfNameString;
	int SelfID;
	int SelfID_Group = 0;

	IDetailGroup* DependentGroupPtr = nullptr;
	TSharedPtr<IPropertyHandle> SelfPropertyHandle;
	IDetailChildrenBuilder* SelfChildBuilder;
	IPropertyTypeCustomizationUtils* SelfCustomizationUtils;
	
	IDetailLayoutBuilder* ParentDetailBuilderPtr;
	TMap<FString, FName>* Map_Purpose_To_HandlerName_Ptr;
};


/**
 * 
 */
class BASEUI_MODULE_API FFunctionHandlerClassCustomizer : public IDetailCustomization
{
public:
	static TSharedRef<class IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FFunctionHandlerClassCustomizer);
	}

	TSharedPtr<FFunctionHandlerPropertyCustomizer> FunctionHandlerPropertyCustomizer;
	TSharedRef<class IPropertyTypeCustomization> MakeFunctionHandlerPropertyCustomizerInstance()
	{
		auto ReturnDetailCustomizationRef = FFunctionHandlerPropertyCustomizer::MakeInstance();
		FunctionHandlerPropertyCustomizer = StaticCastSharedRef<FFunctionHandlerPropertyCustomizer>(ReturnDetailCustomizationRef);
		FunctionHandlerPropertyCustomizer->ParentDetailBuilderPtr = DetailBuilderPtr;
		return ReturnDetailCustomizationRef;
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		DetailBuilderPtr = &DetailBuilder;
		DetailBuilder.RegisterInstancedCustomPropertyTypeLayout(FFunctionHandlerInfo::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateSP(this, &FFunctionHandlerClassCustomizer::MakeFunctionHandlerPropertyCustomizerInstance));
	}
	
	void Handler_OnConfigChanged() const
	{
		FunctionHandlerPropertyCustomizer->Handler_OnConfigChanged();
	}
	
	TSharedRef<IPropertyTypeCustomization> Handle_FName_GetCustomized()
	{
		return FFunctionHandlerPropertyCustomizer::MakeInstance();
	}
	
	IDetailLayoutBuilder* DetailBuilderPtr;
};

