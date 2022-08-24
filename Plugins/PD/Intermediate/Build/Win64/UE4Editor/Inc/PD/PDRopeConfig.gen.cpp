// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PD/Public/PDRopeConfig.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePDRopeConfig() {}
// Cross Module References
	PD_API UClass* Z_Construct_UClass_UPDRopeSharedSimConfig_NoRegister();
	PD_API UClass* Z_Construct_UClass_UPDRopeSharedSimConfig();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_PD();
// End Cross Module References
	void UPDRopeSharedSimConfig::StaticRegisterNativesUPDRopeSharedSimConfig()
	{
	}
	UClass* Z_Construct_UClass_UPDRopeSharedSimConfig_NoRegister()
	{
		return UPDRopeSharedSimConfig::StaticClass();
	}
	struct Z_Construct_UClass_UPDRopeSharedSimConfig_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_IterationCount_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_IterationCount;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_PD,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "PDRopeConfig.h" },
		{ "ModuleRelativePath", "Public/PDRopeConfig.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::NewProp_IterationCount_MetaData[] = {
		{ "Category", "Simulation" },
		{ "ClampMax", "100" },
		{ "ClampMin", "0" },
		{ "ModuleRelativePath", "Public/PDRopeConfig.h" },
		{ "UIMax", "10" },
		{ "UIMin", "1" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::NewProp_IterationCount = { "IterationCount", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeSharedSimConfig, IterationCount), METADATA_PARAMS(Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::NewProp_IterationCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::NewProp_IterationCount_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::NewProp_IterationCount,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPDRopeSharedSimConfig>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::ClassParams = {
		&UPDRopeSharedSimConfig::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::PropPointers),
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPDRopeSharedSimConfig()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UPDRopeSharedSimConfig_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPDRopeSharedSimConfig, 2080942256);
	template<> PD_API UClass* StaticClass<UPDRopeSharedSimConfig>()
	{
		return UPDRopeSharedSimConfig::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPDRopeSharedSimConfig(Z_Construct_UClass_UPDRopeSharedSimConfig, &UPDRopeSharedSimConfig::StaticClass, TEXT("/Script/PD"), TEXT("UPDRopeSharedSimConfig"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPDRopeSharedSimConfig);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
