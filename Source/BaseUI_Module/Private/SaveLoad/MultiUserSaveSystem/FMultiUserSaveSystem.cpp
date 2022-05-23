// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoad/MultiUserSaveSystem/FMultiUserSaveSystem.h"


int32 FMultiUserSaveSystem::CurrentOperatingSlotIndex = INDEX_NONE;
ESaveGameType FMultiUserSaveSystem::CurrentSaveType = ESaveGameType::NormalSave;
TMap<int32, FName> FMultiUserSaveSystem::Map_UserIndex_To_SaveFolderName;

