#pragma once

#include <CoreMinimal.h>
#include "DongSerializeStatic.h"
#include "TextureUtilities.h"
#include "SaveGameObjectHeaderBase.generated.h"


/**
 * 文件头为何用UObject，而不是结构体
 *		文件头所需特性：
 *		1. 可以通过派生来添加新的数据类型。
 *		2. 可以使用统一的父类来进行管理。（蓝图办不到这点）
 *		3. 数据可被蓝图访问。
 * 自定义新的文件头：
 *		0. 从本类派生新的子类
 *		1. 添加成员变量（新的数据类型），并选择添加UPROPERTY宏
 *		2. 按需覆盖Init函数
 *		3. 按需添加额外的Init函数
 *		4. （根据成员变量的数据类型，可能为必须）覆盖Serialize函数，并内部调用父类实现（必须，父类的成员变量的序列化是必要的）。
 *			a. 没有UPROPERTY的变量，必须手动添加序列化代码
 *			b. UTexture2D 需要手动序列化，代码可参考本类的实现。
 *			c. 容器类型：TArray TMap TSet类型，也需要手动添加序列化代码。
 *			d. 没有奇怪的需求的话，a和c都只用调用UE提供的序列化函数【Ar << 变量名;】即可。
 *
 */
UCLASS(BlueprintType, meta=(ToolTip="存储 存档的文件头相关信息。"))
class BASEUI_MODULE_API USaveGameObjectHeaderBase : public UObject
{
	GENERATED_BODY()
public:
	void Init(const UClass* SaveGameObjectClass)
	{
		SaveGameClassName = SaveGameObjectClass->GetPathName();
	}
	
	uint64 HeaderLength = 0;
	FGuid GameVersion;  // 
	FGuid SaveDataVersion;
	FEngineVersion SavedEngineVersion = FEngineVersion::Current();
	FString SaveGameClassName = USaveGameObjectHeaderBase::StaticClass()->GetPathName();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip="存档文件名，含扩展名"))
	FString SaveSlotNameStr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip="和存档关联的地图名称"))
	FName PlayingMapIdentifier;
		
	UPROPERTY(EditAnywhere, SkipSerialization, BlueprintReadWrite, Category="SaveData|BaseInfo", meta=(Tooltip="Runtime 使用。显示存档缩略图"))
	UTexture2D* MapThumbnail = nullptr;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip="用户自行添加的注释"))
	FText UserComment;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip="本存档的Slot序号"))
	int32 SlotIndex = INDEX_NONE;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip="存档保存的时间"))
	FDateTime SaveTime;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="SaveData|BaseInfo", meta=(Tooltip="保存和读取时使用；显示存档缩略图Texture2D的数据数组。"))
	TArray<uint8> PNG_ImageArray;

	virtual void Serialize(FArchive& Ar) override
	{
		// 记录【文件头长度】的保存地址
		const int64 OldPos = Ar.Tell();
		// 确保记录文件头长度的空间（8Byte），初始化为0 。待文件头全部写入完毕后再计算文件头长度。
		Ar << HeaderLength;

		// 【非反射系统变量】的保存和恢复
		// 底层信息，版本等，
		Ar << GameVersion;
		Ar << SaveDataVersion;
		Ar << SavedEngineVersion;
		Ar << SaveGameClassName;

		// 【反射系统变量】的保存和恢复
		// 通过反射系统来恢复或保存 加入到反射系统的变量
		FDongSerializePropertyParams SerializeParams(this, this->GetClass());
		SerializeParams.SetMode_SaveGame_SearchSuper();
		SerializeParams.SetStopCondition_Type1(USaveGameObjectHeaderBase::StaticClass(), true);
		UDongSerializeStatic::SerializePropertyWithFixedSeq(Ar, SerializeParams);
		
		// 反序列化（读取）的时候：
		if (Ar.IsLoading())
		{
			// 图像的解压和恢复
			Ar << PNG_ImageArray;
			if (PNG_ImageArray.Num())
			{
				MapThumbnail = UTextureUtilities::Import_CompressedRawDataArray_To_Texture2D(PNG_ImageArray);
			}
		}
		// 序列化（保存）的时候
		else
		{
			// 图像的压缩保存
			// 将UTexture的图像信息压缩并保存到Byte数组中。
			if (IsValid(MapThumbnail))
			{
				FTextureImportExportParams Params;
				Params.CompressFormat = ETextureCompressFormat::JPEG;
				UTextureUtilities::Export_Texture2D_To_RawDataArray_Compressed(MapThumbnail, Params, PNG_ImageArray);

				MapThumbnail->RemoveFromRoot();
				MapThumbnail->MarkPendingKill();
			}
			// 不论MapThumbnail的状态，保存的时候都保存PNG_ImageArray。
			// 而不是只在MapThumbnail合法的时候保存
			Ar << PNG_ImageArray;

			// 保存文件头的长度，方便【只 读取文件头】功能
			const int64 NewPos = Ar.Tell();
			HeaderLength = NewPos - OldPos + 1;
			Ar.Seek(OldPos);  // 回到开头
			Ar << HeaderLength;  // 写入最终文件头的长度。
			Ar.Seek(NewPos);  // 返回末尾，结束本Object的序列化。
		}
	}
};