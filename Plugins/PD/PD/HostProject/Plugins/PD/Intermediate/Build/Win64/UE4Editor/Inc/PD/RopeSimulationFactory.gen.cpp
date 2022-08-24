// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PD/Public/RopeSimulationFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRopeSimulationFactory() {}
// Cross Module References
	PD_API UClass* Z_Construct_UClass_URopeSimulationFactory_NoRegister();
	PD_API UClass* Z_Construct_UClass_URopeSimulationFactory();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_PD();
// End Cross Module References
	void URopeSimulationFactory::StaticRegisterNativesURopeSimulationFactory()
	{
	}
	UClass* Z_Construct_UClass_URopeSimulationFactory_NoRegister()
	{
		return URopeSimulationFactory::StaticClass();
	}
	struct Z_Construct_UClass_URopeSimulationFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URopeSimulationFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_PD,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URopeSimulationFactory_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "RopeSimulationFactory.h" },
		{ "ModuleRelativePath", "Public/RopeSimulationFactory.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_URopeSimulationFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URopeSimulationFactory>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URopeSimulationFactory_Statics::ClassParams = {
		&URopeSimulationFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A1u,
		METADATA_PARAMS(Z_Construct_UClass_URopeSimulationFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URopeSimulationFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URopeSimulationFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URopeSimulationFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URopeSimulationFactory, 16063760);
	template<> PD_API UClass* StaticClass<URopeSimulationFactory>()
	{
		return URopeSimulationFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URopeSimulationFactory(Z_Construct_UClass_URopeSimulationFactory, &URopeSimulationFactory::StaticClass, TEXT("/Script/PD"), TEXT("URopeSimulationFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URopeSimulationFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
