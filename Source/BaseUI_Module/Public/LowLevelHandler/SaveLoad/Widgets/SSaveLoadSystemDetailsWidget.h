// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailCustomization.h"
#include "PropertyEditor/Private/SDetailsView.h"
#include "Widgets/SCompoundWidget.h"


// class USaveLoadSystemConfig;

class FSaveLoadSystemDetailsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<class IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FSaveLoadSystemDetailsCustomization);
	}
	
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		auto ConfigCategory = &DetailBuilder.EditCategory("Basic Config", FText::GetEmpty(), ECategoryPriority::Important);
		auto SaveAreaCategory = &DetailBuilder.EditCategory("Save Area", FText::GetEmpty(), ECategoryPriority::Important);

		ConfigCategory->SetShowAdvanced(false);
		// auto ConfigRowSlot = ConfigCategory->AddCustomRow(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "ConfigCategoryRowFilterText", ""), false);
		ConfigCategory->AddCustomRow(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "ConfigCategoryRowFilterText", ""), false)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			  .AutoWidth()
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "ConfigCategorySlotNamingText", "指定存档Slot的名称（文件名）；可使用大括号来指定变量作为Slot的名称；可使用的变量名为：【Prefix】【UserID】【SlotID】【CustomSlotName】"))
			]
			// + SHorizontalBox::Slot()
			//   .HAlign(HAlign_Fill)
			//   .VAlign(VAlign_Fill)
			//   .AutoWidth()
			// [
			// 	SNew(SButton)
			// 	.Text(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "Import", "Import"))
			// 	.ToolTipText(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "ImportRulesTable_Tooltip", "Import Rules Table to the Config"))
			// 	.OnClicked_Lambda([this]()
			// 			 {
			// 				UE_LOG(LogTemp, Display, TEXT("Customized button clicked!"))
			// 				return (FReply::Handled());
			// 			}
			// 			)
			// ]
		];

		auto SaveAreaRowSlot = SaveAreaCategory->AddCustomRow(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "ConfigCategoryRowFilterText", ""), false)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .HAlign(HAlign_Fill)
			  .VAlign(VAlign_Fill)
			  .AutoWidth()
			[
				SNew(STextBlock)
				.Text(NSLOCTEXT("SaveLoadSystemDetailsCustomization", "SaveAreaRowSlotNamingText", "指定各个存档的存储区域相关的信息；可使用大括号来指定变量作为存储区域的名称；可使用的变量名为：【UserID】"))
			]
		];
		// SaveAreaRowSlot.
	}
};
/**
 * 
 */
class BASEUI_MODULE_API SSaveLoadSystemDetailsWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSaveLoadSystemDetailsWidget)
		{
		}

	SLATE_END_ARGS()
	
	virtual FDetailsViewArgs MakeDetailsViewArgs()
	{
		FDetailsViewArgs DetailsViewArgs;
		{
			DetailsViewArgs.bAllowSearch = false;
			DetailsViewArgs.bHideSelectionTip = true;
			DetailsViewArgs.bLockable = false;
			DetailsViewArgs.bSearchInitialKeyFocus = true;
			DetailsViewArgs.bUpdatesFromSelection = false;
			DetailsViewArgs.NotifyHook = nullptr;
			DetailsViewArgs.bShowOptions = true;
			DetailsViewArgs.bShowModifiedPropertiesOption = false;
			DetailsViewArgs.bShowScrollBar = false;
			DetailsViewArgs.bShowOptions = true;
		}
		return DetailsViewArgs;
	}
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};

