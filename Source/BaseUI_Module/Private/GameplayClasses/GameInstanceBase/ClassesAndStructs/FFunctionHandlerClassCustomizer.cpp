// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayClasses/GameInstanceBase/ClassesAndStructs/FFunctionHandlerClassCustomizer.h"
#include "GameplayClasses/GameInstanceBase/MainMenuGameInstanceConfig.h"

#include "IDetailGroup.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/STextComboBox.h"


TMap<FString, TArray<TSharedPtr<FString>>> FFunctionHandlerPropertyCustomizer::Map_HandlerNamePurpose_To_ValidDependentNameList;
TMap<FString, TSharedPtr<STextComboBox>> FFunctionHandlerPropertyCustomizer::Map_HandlerNamePurpose_To_ComboBoxPtr;
// TArray<TSharedPtr<STextComboBox>> FFunctionHandlerPropertyCustomizer::ComboBoxPtrList;
int FFunctionHandlerPropertyCustomizer::CreationCount = 0;


TSharedRef<IPropertyTypeCustomization> FFunctionHandlerPropertyCustomizer::MakeInstance()
{
	TSharedRef<IPropertyTypeCustomization> ObjectPtr = MakeShareable(new FFunctionHandlerPropertyCustomizer);
	TSharedRef<FFunctionHandlerPropertyCustomizer> SelfObjectRef = StaticCastSharedRef<FFunctionHandlerPropertyCustomizer>(ObjectPtr);
	SelfObjectRef->Handler_OnConfigChanged();

	SelfObjectRef->SelfNameString = FString::FromInt(CreationCount);
	SelfObjectRef->SelfID = CreationCount;
	// UE_LOG(LogTemp, Display, TEXT("Function:[%s] Customizator ID: [%d] Created!!"), ANSI_TO_TCHAR(__FUNCTION__), CreationCount);
	CreationCount++;
	return SelfObjectRef;
}

void FFunctionHandlerPropertyCustomizer::Handle_OnHandlerClassChanged()
{
	MakeNewDependentHandlerSelectionWidget(SelfPropertyHandle.ToSharedRef(), *SelfChildBuilder, *SelfCustomizationUtils);
	if (ParentDetailBuilderPtr)
	{
		ParentDetailBuilderPtr->ForceRefreshDetails();
	}
}

void FFunctionHandlerPropertyCustomizer::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                           IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	// UE_LOG(LogTemp, Display, TEXT("Function:[%s] Got children property [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *PropertyHandle->GetProperty()->GetName());
	SelfPropertyHandle = PropertyHandle;
	SelfChildBuilder = &ChildBuilder;
	SelfCustomizationUtils = &CustomizationUtils;
	TSharedPtr<IPropertyHandle> PropertyHandle_Map_Purpose_To_HandlerName = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FFunctionHandlerInfo, Map_Purpose_To_HandlerName));
		
	TSharedPtr<IPropertyHandle> PropertyHandle_HandlerClass = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FFunctionHandlerInfo, HandlerClass));

	
	ChildBuilder.AddProperty(PropertyHandle_HandlerClass.ToSharedRef());
	PropertyHandle_HandlerClass->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this, &FFunctionHandlerPropertyCustomizer::Handle_OnHandlerClassChanged));

	MakeNewDependentHandlerSelectionWidget(PropertyHandle, ChildBuilder, CustomizationUtils);
}

void FFunctionHandlerPropertyCustomizer::MakeNewDependentHandlerSelectionWidget(
	TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils
	)
{
	auto DetailsNormalFont = CustomizationUtils.GetRegularFont();
	auto DetailsBoldFont = CustomizationUtils.GetRegularFont();

	TSharedPtr<IPropertyHandle> PropertyHandle_Map_Purpose_To_HandlerName = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FFunctionHandlerInfo, Map_Purpose_To_HandlerName));
		
	TSharedPtr<IPropertyHandle> PropertyHandle_HandlerClass = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FFunctionHandlerInfo, HandlerClass));
	
	// 获取Property 变量值的指针
	void* _TempPtr;
	PropertyHandle_Map_Purpose_To_HandlerName->GetValueData(_TempPtr);
	Map_Purpose_To_HandlerName_Ptr = (TMap<FString, FName>*)_TempPtr;

	PropertyHandle->GetValueData(_TempPtr);
	FFunctionHandlerInfo* DataPtr = (FFunctionHandlerInfo*) _TempPtr;


	// 拿到正在Customized的Handler名称
	const TMap<FName, FFunctionHandlerInfo>& HandlerClassDict = UMainMenuGameInstanceConfig::Get()->HandlerClassDict;
	FName CustomizationHandlerName;
	for (TMap<FName, FFunctionHandlerInfo>::TConstIterator Pair = HandlerClassDict.CreateConstIterator(); Pair; ++Pair)
	{
		const FFunctionHandlerInfo& HandlerInfoRef = Pair.Value();
		const FFunctionHandlerInfo* HandlerInfoPtr = &HandlerInfoRef;
		// UE_LOG(LogTemp, Display, TEXT("Function:[%s] Input Property Address: [%016x]; Test property Addr: [%016x]"), ANSI_TO_TCHAR(__FUNCTION__), DataPtr, HandlerInfoPtr);
		if (UNLIKELY(HandlerInfoPtr == DataPtr))
		{
			CustomizationHandlerName = Pair->Key;
			break;
		}
	}

	SelfNameString = CustomizationHandlerName.ToString() + "_" + FString::FromInt(CreationCount);
	UE_LOG(LogTemp, Display, TEXT("Function:[%s] Customizator name: [%s] Customized!!"), ANSI_TO_TCHAR(__FUNCTION__), *SelfNameString);

	const FName GroupID("Dependency_" + FString::FromInt(SelfID) + "_" + FString::FromInt(SelfID_Group));
	// const FText GroupDisplayText = FText::FromString("Dependent Handler List " + FString::FromInt(SelfID) + "_" + FString::FromInt(SelfID_Group));
	const FText GroupDisplayText = FText::FromString("Dependent Handler List");
	IDetailGroup& DependentGroup = ChildBuilder.AddGroup(GroupID, GroupDisplayText);
	SelfID_Group++;

	// 寻找当前Handler的 目的->依赖信息字典
	HandlerDependentMap* Map_Purpose_To_DependentMap_Ptr = nullptr;

	// 没有的话就说明本Handler无需依赖
	if (! FFunctionHandlerDef::Get_Map_Purpose_To_Dependent(DataPtr->HandlerClass, Map_Purpose_To_DependentMap_Ptr))
	{
		for (auto Ptr : SelfComboBoxPtrList)
		{
			for (auto ComboBoxPtrIt = Map_HandlerNamePurpose_To_ComboBoxPtr.CreateConstIterator(); ComboBoxPtrIt; ++ComboBoxPtrIt)
			{
				if (ComboBoxPtrIt->Key.Contains(CustomizationHandlerName.ToString()))
				{
					Map_HandlerNamePurpose_To_ComboBoxPtr.Remove(ComboBoxPtrIt->Key);
				}
			}
		}
		
		auto CustomRowName = FText::FromString("");
		DependentGroup.AddWidgetRow()
			.WholeRowContent()
			[
				SNew(STextBlock)
				.Text(FText::FromString("No dependency!"))
			];
		return;
	}
	const auto Map_Purpose_To_DependentMap = *Map_Purpose_To_DependentMap_Ptr;

	// 生成说明
	DependentGroup.AddWidgetRow()
	.NameContent()
	[
		SNew(SBox)
	   .HAlign(HAlign_Fill)
	   .VAlign(VAlign_Fill)
	   .MinDesiredWidth(250)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Handler Purpose:"))
			.Font(DetailsBoldFont)
		]
	]
	.ValueContent()
	[
		SNew(SBox)
	   .HAlign(HAlign_Fill)
	   .VAlign(VAlign_Fill)
	   .MinDesiredWidth(250)
	    [
			SNew(STextBlock)
			.Text(FText::FromString("Select a handler:"))
			.Font(DetailsBoldFont)
		]
	];

	// 正式生成用于选择依赖项的控件
	for(auto Pair = (Map_Purpose_To_DependentMap).CreateConstIterator(); Pair; ++Pair)
	{
		// 生成键值
		const FString ValidDependentList_KeyName = CustomizationHandlerName.ToString() + "::" + Pair.Key();
		const FString& PurposeString = Pair.Key();

		TSharedPtr<STextComboBox> ComboBoxPtr;
		SAssignNew(ComboBoxPtr, STextComboBox)
			.OnSelectionChanged_Lambda(
			[this, PurposeString]
			(TSharedPtr<FString> InNewChoice, ESelectInfo::Type InSelectionType)
				{
					if (InNewChoice.IsValid())
					{
						auto HandlerNamePtr = (*Map_Purpose_To_HandlerName_Ptr).Find(PurposeString);
						if (HandlerNamePtr == nullptr)
						{
							(*Map_Purpose_To_HandlerName_Ptr).Add(PurposeString);
						}
						(*Map_Purpose_To_HandlerName_Ptr)[PurposeString] = FName(*InNewChoice);
					}
				}
			)
			.OptionsSource(&Map_HandlerNamePurpose_To_ValidDependentNameList[ValidDependentList_KeyName])
			.InitiallySelectedItem(Map_HandlerNamePurpose_To_ValidDependentNameList[ValidDependentList_KeyName][0])
			.ToolTipText(Pair.Value()->Tooltip)
			.Font(DetailsNormalFont);
		Map_HandlerNamePurpose_To_ComboBoxPtr.Add(ValidDependentList_KeyName, ComboBoxPtr);
		SelfComboBoxPtrList.Add(ComboBoxPtr);
			
		auto CustomRowName = FText::FromString(Pair.Key());
		DependentGroup.AddWidgetRow()
		    .NameContent()
			[
				SNew(SBox)
			   .HAlign(HAlign_Fill)
			   .VAlign(VAlign_Fill)
			   .MinDesiredWidth(250)
				[
					SNew(STextBlock)
					.Text(FText::FromString(Pair.Key()))
					.Font(DetailsNormalFont)
					.ToolTipText(Pair.Value()->Tooltip)
				]
			]
			.ValueContent()
			[
				SNew(SBox)
			   .HAlign(HAlign_Fill)
			   .VAlign(VAlign_Fill)
			   .MinDesiredWidth(250)
				[
					ComboBoxPtr.ToSharedRef()
				]
			];
	}
}

void FFunctionHandlerPropertyCustomizer::Handler_OnConfigChanged()
{
	const auto HandlerClassDict = UMainMenuGameInstanceConfig::Get()->HandlerClassDict;

	// 更新各个Handler的各个依赖的可选择名称列表
	// 循环 每个被定义的Handler的名称
	for (auto Pair = HandlerClassDict.CreateConstIterator(); Pair; ++Pair)
	{
		const FName& Key = Pair.Key();

		// 寻找当前Handler的 目的->依赖信息字典
		HandlerDependentMap* Map_Purpose_To_DependentMap_Ptr = nullptr;
		
		if (! FFunctionHandlerDef::Get_Map_Purpose_To_Dependent(Pair.Value().HandlerClass, Map_Purpose_To_DependentMap_Ptr))
		{
			continue;
		}
		const auto Map_Purpose_To_DependentMap = *Map_Purpose_To_DependentMap_Ptr;


		// 循环 当前的Handler的 每个【依赖Handler的目的】
		for (auto PurposePair = Map_Purpose_To_DependentMap.CreateConstIterator(); PurposePair; ++PurposePair)
		{
			const FString HandlerName = Pair.Key().ToString();
			const FString Purpose = PurposePair.Key();
			const FString KeyName = HandlerName + "::" + Purpose;

			TArray<TSharedPtr<FString>> ValidHandlerNameList;
			Update_ValidHandlerName_ByClass(PurposePair.Value()->DependentClass, KeyName, ValidHandlerNameList);
			Map_HandlerNamePurpose_To_ValidDependentNameList.Add(KeyName, ValidHandlerNameList);
		}
	}
	
	UE_LOG(LogTemp, Display, TEXT("Function:[%s] Customizator name: [%s] Update!!"), ANSI_TO_TCHAR(__FUNCTION__), *SelfNameString);

	for (auto ComboBoxPtrIt = Map_HandlerNamePurpose_To_ComboBoxPtr.CreateConstIterator(); ComboBoxPtrIt; ++ComboBoxPtrIt)
	{
		const auto KeyName = ComboBoxPtrIt->Key;
		const auto ComboBoxPtr = ComboBoxPtrIt->Value;
		if (ComboBoxPtr.IsValid())
		{
			const FString ItemString = *ComboBoxPtr->GetSelectedItem().Get();
			ComboBoxPtr->RefreshOptions();
			const auto ValidDependentNamePtrList = Map_HandlerNamePurpose_To_ValidDependentNameList[KeyName];

			for (auto ValidDependentNamePtrIt = ValidDependentNamePtrList.CreateConstIterator(); ValidDependentNamePtrIt; ++ValidDependentNamePtrIt)
			{
				if ((*ValidDependentNamePtrIt->Get()) == ItemString)
				{
					ComboBoxPtr->SetSelectedItem(*ValidDependentNamePtrIt);
					break;
				}
			}
		}
	}
}

void FFunctionHandlerPropertyCustomizer::Update_ValidHandlerName_ByClass(UClass* InClass, const FString& InCurrentHandlerPurposeString, TArray<TSharedPtr<FString>>& OutFilteredHandlerName)
{
	const auto HandlerClassDict = UMainMenuGameInstanceConfig::Get()->HandlerClassDict;
	for (auto Pair = HandlerClassDict.CreateConstIterator(); Pair; ++Pair)
	{
		const auto SettedClass = Pair.Value().HandlerClass;
		if (UNLIKELY(Pair.Value().HandlerClass == InClass))
		{
			OutFilteredHandlerName.Add(MakeShareable(new FString(Pair.Key().ToString())));
		}
	}
}
