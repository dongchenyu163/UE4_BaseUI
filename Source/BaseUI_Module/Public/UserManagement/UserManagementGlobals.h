#pragma once
#include "CoreMinimal.h"
#include "UserManagementGlobals.generated.h"


constexpr int64 USERID_START_NUMBER = 10000000;  
constexpr char USERINFO_SAVESLOT_NAME[] = "UserInfoSlot"; 



UCLASS(config = UserManagerConfig)
class UUserManagerConfig : public UDeveloperSettings
{
	GENERATED_BODY()

#pragma region ParentOverride
public:
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	virtual FName GetCategoryName() const override { return TEXT("Dong UI Framework Modules Config"); }  // Setting的左侧栏的大分类
	virtual FName GetSectionName() const override { return TEXT("UserManagerConfig"); }  // Setting的左侧栏的子分类

	// Setting的左侧栏的子分类
	virtual FText GetSectionText() const override
	{
		return NSLOCTEXT("UserManagerConfig", "SectionText", "User Manager Config");
	}

	virtual FText GetSectionDescription() const override
	{
		return NSLOCTEXT("UserManagerConfig", "SectionDescriptionText", "设置用户管理系统的相关参数");
	}
#pragma endregion ParentOverride


#pragma region PublicFunctions
public:
	UFUNCTION(BlueprintPure, Category="UserManager", DisplayName="GetUserManagerConfig")
	static UUserManagerConfig* Get() { return GetMutableDefault<UUserManagerConfig>(); }
	
	UFUNCTION(BlueprintPure, Category="UserManager")
	static bool IsInitialAutoCreateUser() { return Get()->bInitialAutoCreateUser; }
	
	UFUNCTION(BlueprintPure, Category="UserManager", meta=(ToolTip="获取用于存储【所有用户】的【基础信息】存档的名称，用以进行之后保存操作"))
	static FString GetSaveSlotName() { return Get()->UserInfoSaveSlotName; }

	UFUNCTION(BlueprintPure, Category="UserManager", meta=(ToolTip="获取用于 生成新用户ID 的起始数字"))
	static int64 GetStartUserID() { return Get()->UserID_StartNumber; }

	UFUNCTION(BlueprintPure, Category="UserManager")
	static FString GetDefaultUserName() { return Get()->DefaultUserName; }
#pragma endregion PublicFunctions

protected:
	
	UPROPERTY(BlueprintReadOnly, Config, Category="UserManager", DisplayName="User Info Slot Name", meta=(ToolTip="用来存储【所有用户】的【基础信息（FUserInfo结构体）】的存档的名称"))
	FString UserInfoSaveSlotName = "AllUserData";

	UPROPERTY(BlueprintReadOnly, Config, Category="UserManager", DisplayName="User ID Start From", meta=(ToolTip="新创建的用户的ID从这个数字开始累计。"))
	int64 UserID_StartNumber = 10000000;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="UserManager", meta=(ToolTip="UUserManagement初始化的时候自动创建一个默认用户（如果名为【DefaultUserName变量】的用户不存在的话）"))
	bool bInitialAutoCreateUser = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category="UserManager", meta=(EditCondition="bInitialAutoCreateUser"))
	FString DefaultUserName = "DefaultUserName";
public:

};
