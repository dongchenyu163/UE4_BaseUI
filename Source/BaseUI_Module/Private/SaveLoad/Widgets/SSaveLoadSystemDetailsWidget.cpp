// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoad/Widgets/SSaveLoadSystemDetailsWidget.h"

#include "SlateOptMacros.h"
#include "SaveLoad/SaveLoadSystemGlobals.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSaveLoadSystemDetailsWidget::Construct(const FArguments& InArgs)
{
	// Create a property view
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// create details
	auto DetailsView = EditModule.CreateDetailView(MakeDetailsViewArgs());

		
	TSharedPtr<IDetailCustomization> DetailsCustomizer = FSaveLoadSystemDetailsCustomization::MakeInstance();
	if (DetailsCustomizer)
	{
		DetailsView->RegisterInstancedCustomPropertyLayout(
			USaveLoadSystemConfig::StaticClass(),
			FOnGetDetailCustomizationInstance::CreateLambda([DetailsCustomizer](){return DetailsCustomizer.ToSharedRef();})
			);
	}

	DetailsView->SetObject(USaveLoadSystemConfig::Get(), true);
		
	ChildSlot
	[
		// DetailsView
	SNew(STextBlock)
	.Text(FText::FromString("Test Widget"))
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
