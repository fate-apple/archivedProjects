// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PD/Public/PDRopeComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePDRopeComponent() {}
// Cross Module References
	PD_API UClass* Z_Construct_UClass_UPDRopeComponent_NoRegister();
	PD_API UClass* Z_Construct_UClass_UPDRopeComponent();
	ENGINE_API UClass* Z_Construct_UClass_UMeshComponent();
	UPackage* Z_Construct_UPackage__Script_PD();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FComponentReference();
// End Cross Module References
	DEFINE_FUNCTION(UPDRopeComponent::execSetPauseSimulation)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetPauseSimulation();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPDRopeComponent::execGetCableParticleLocationsV)
	{
		P_GET_TARRAY_REF(FVector,Z_Param_Out_Locations);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetCableParticleLocationsV(Z_Param_Out_Locations);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPDRopeComponent::execGetAttachedComponentV)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(USceneComponent**)Z_Param__Result=P_THIS->GetAttachedComponentV();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPDRopeComponent::execGetAttachedActorV)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(AActor**)Z_Param__Result=P_THIS->GetAttachedActorV();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPDRopeComponent::execSetAttachEndToV)
	{
		P_GET_OBJECT(AActor,Z_Param_Actor);
		P_GET_PROPERTY(FNameProperty,Z_Param_ComponentProperty);
		P_GET_PROPERTY(FNameProperty,Z_Param_SocketName);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetAttachEndToV(Z_Param_Actor,Z_Param_ComponentProperty,Z_Param_SocketName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPDRopeComponent::execSetAttachEndToComponentV)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_Component);
		P_GET_PROPERTY(FNameProperty,Z_Param_SocketName);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetAttachEndToComponentV(Z_Param_Component,Z_Param_SocketName);
		P_NATIVE_END;
	}
	void UPDRopeComponent::StaticRegisterNativesUPDRopeComponent()
	{
		UClass* Class = UPDRopeComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetAttachedActorV", &UPDRopeComponent::execGetAttachedActorV },
			{ "GetAttachedComponentV", &UPDRopeComponent::execGetAttachedComponentV },
			{ "GetCableParticleLocationsV", &UPDRopeComponent::execGetCableParticleLocationsV },
			{ "SetAttachEndToComponentV", &UPDRopeComponent::execSetAttachEndToComponentV },
			{ "SetAttachEndToV", &UPDRopeComponent::execSetAttachEndToV },
			{ "SetPauseSimulation", &UPDRopeComponent::execSetPauseSimulation },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics
	{
		struct PDRopeComponent_eventGetAttachedActorV_Parms
		{
			AActor* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventGetAttachedActorV_Parms, ReturnValue), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::Function_MetaDataParams[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Gets the Actor that the cable is attached to **/" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Gets the Actor that the cable is attached to *" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPDRopeComponent, nullptr, "GetAttachedActorV", nullptr, nullptr, sizeof(PDRopeComponent_eventGetAttachedActorV_Parms), Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics
	{
		struct PDRopeComponent_eventGetAttachedComponentV_Parms
		{
			USceneComponent* ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventGetAttachedComponentV_Parms, ReturnValue), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::NewProp_ReturnValue_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::NewProp_ReturnValue_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::Function_MetaDataParams[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Gets the specific USceneComponent that the cable is attached to **/" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Gets the specific USceneComponent that the cable is attached to *" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPDRopeComponent, nullptr, "GetAttachedComponentV", nullptr, nullptr, sizeof(PDRopeComponent_eventGetAttachedComponentV_Parms), Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics
	{
		struct PDRopeComponent_eventGetCableParticleLocationsV_Parms
		{
			TArray<FVector> Locations;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Locations_Inner;
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Locations;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::NewProp_Locations_Inner = { "Locations", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::NewProp_Locations = { "Locations", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventGetCableParticleLocationsV_Parms, Locations), EArrayPropertyFlags::None, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::NewProp_Locations_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::NewProp_Locations,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::Function_MetaDataParams[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Get array of locations of particles (in world space) making up the cable simulation. */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Get array of locations of particles (in world space) making up the cable simulation." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPDRopeComponent, nullptr, "GetCableParticleLocationsV", nullptr, nullptr, sizeof(PDRopeComponent_eventGetCableParticleLocationsV_Parms), Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54420401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics
	{
		struct PDRopeComponent_eventSetAttachEndToComponentV_Parms
		{
			USceneComponent* Component;
			FName SocketName;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_SocketName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventSetAttachEndToComponentV_Parms, Component), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_Component_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_Component_MetaData)) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_SocketName = { "SocketName", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventSetAttachEndToComponentV_Parms, SocketName), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_Component,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::NewProp_SocketName,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::Function_MetaDataParams[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Attaches the end of the cable to a specific Component **/" },
		{ "CPP_Default_SocketName", "None" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Attaches the end of the cable to a specific Component *" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPDRopeComponent, nullptr, "SetAttachEndToComponentV", nullptr, nullptr, sizeof(PDRopeComponent_eventSetAttachEndToComponentV_Parms), Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics
	{
		struct PDRopeComponent_eventSetAttachEndToV_Parms
		{
			AActor* Actor;
			FName ComponentProperty;
			FName SocketName;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Actor;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_ComponentProperty;
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_SocketName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::NewProp_Actor = { "Actor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventSetAttachEndToV_Parms, Actor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::NewProp_ComponentProperty = { "ComponentProperty", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventSetAttachEndToV_Parms, ComponentProperty), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::NewProp_SocketName = { "SocketName", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(PDRopeComponent_eventSetAttachEndToV_Parms, SocketName), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::NewProp_Actor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::NewProp_ComponentProperty,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::NewProp_SocketName,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::Function_MetaDataParams[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Attaches the end of the cable to a specific Component within an Actor **/" },
		{ "CPP_Default_SocketName", "None" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Attaches the end of the cable to a specific Component within an Actor *" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPDRopeComponent, nullptr, "SetAttachEndToV", nullptr, nullptr, sizeof(PDRopeComponent_eventSetAttachEndToV_Parms), Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation_Statics::Function_MetaDataParams[] = {
		{ "Category", "VerletRope" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPDRopeComponent, nullptr, "SetPauseSimulation", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UPDRopeComponent_NoRegister()
	{
		return UPDRopeComponent::StaticClass();
	}
	struct Z_Construct_UClass_UPDRopeComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bAttachStartV_MetaData[];
#endif
		static void NewProp_bAttachStartV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAttachStartV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bAttachEndV_MetaData[];
#endif
		static void NewProp_bAttachEndV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bAttachEndV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AttachEndToV_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_AttachEndToV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AttachEndToSocketNameV_MetaData[];
#endif
		static const UE4CodeGen_Private::FNamePropertyParams NewProp_AttachEndToSocketNameV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EndLocationV_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_EndLocationV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bUseSubsteppingV_MetaData[];
#endif
		static void NewProp_bUseSubsteppingV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUseSubsteppingV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bSkipCableUpdateWhenNotVisibleV_MetaData[];
#endif
		static void NewProp_bSkipCableUpdateWhenNotVisibleV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSkipCableUpdateWhenNotVisibleV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_MetaData[];
#endif
		static void NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RopeLengthV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RopeLengthV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_r0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_r0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_density_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_density;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_E_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_E;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EndM_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_EndM;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PoissonRatio_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PoissonRatio;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_pauseSimulation_MetaData[];
#endif
		static void NewProp_pauseSimulation_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_pauseSimulation;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableStiffnessV_MetaData[];
#endif
		static void NewProp_bEnableStiffnessV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableStiffnessV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SubstepTimeV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SubstepTimeV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_NumSegmentsV_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_NumSegmentsV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bEnableCollisionV_MetaData[];
#endif
		static void NewProp_bEnableCollisionV_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bEnableCollisionV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CollisionFrictionV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_CollisionFrictionV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RopeForceV_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_RopeForceV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RopeGravityScaleV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RopeGravityScaleV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RopeWidthV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RopeWidthV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_NumSidesV_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_NumSidesV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TileMaterialV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TileMaterialV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SolverIterationsV_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_SolverIterationsV;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPDRopeComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UMeshComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_PD,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UPDRopeComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UPDRopeComponent_GetAttachedActorV, "GetAttachedActorV" }, // 1509224630
		{ &Z_Construct_UFunction_UPDRopeComponent_GetAttachedComponentV, "GetAttachedComponentV" }, // 937812466
		{ &Z_Construct_UFunction_UPDRopeComponent_GetCableParticleLocationsV, "GetCableParticleLocationsV" }, // 86937324
		{ &Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToComponentV, "SetAttachEndToComponentV" }, // 1882181834
		{ &Z_Construct_UFunction_UPDRopeComponent_SetAttachEndToV, "SetAttachEndToV" }, // 1433423921
		{ &Z_Construct_UFunction_UPDRopeComponent_SetPauseSimulation, "SetPauseSimulation" }, // 2636074205
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Rendering" },
		{ "Comment", "/** Component that allows you to specify custom triangle mesh geometry */" },
		{ "HideCategories", "Object Physics Activation Components|Activation Mobility Trigger" },
		{ "IncludePath", "PDRopeComponent.h" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Component that allows you to specify custom triangle mesh geometry" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/**\n\x09 *\x09Should we fix the start to something, or leave it free.\n\x09 *\x09If false, component transform is just used for initial location of start of cable\n\x09 */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Should we fix the start to something, or leave it free.\nIf false, component transform is just used for initial location of start of cable" },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bAttachStartV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV = { "bAttachStartV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/**\n\x09 *\x09Should we fix the end to something (using AttachEndTo and EndLocation), or leave it free.\n\x09 *\x09If false, AttachEndTo and EndLocation are just used for initial location of end of cable\n\x09 */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Should we fix the end to something (using AttachEndTo and EndLocation), or leave it free.\nIf false, AttachEndTo and EndLocation are just used for initial location of end of cable" },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bAttachEndV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV = { "bAttachEndV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Actor or Component that the defines the end position of the cable */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Actor or Component that the defines the end position of the cable" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToV = { "AttachEndToV", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, AttachEndToV), Z_Construct_UScriptStruct_FComponentReference, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToSocketNameV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Socket name on the AttachEndTo component to attach to */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Socket name on the AttachEndTo component to attach to" },
	};
#endif
	const UE4CodeGen_Private::FNamePropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToSocketNameV = { "AttachEndToSocketNameV", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, AttachEndToSocketNameV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToSocketNameV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToSocketNameV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndLocationV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** End location of cable, relative to AttachEndTo (or AttachEndToSocketName) if specified, otherwise relative to cable component. */" },
		{ "MakeEditWidget", "TRUE" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "End location of cable, relative to AttachEndTo (or AttachEndToSocketName) if specified, otherwise relative to cable component." },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndLocationV = { "EndLocationV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, EndLocationV), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndLocationV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndLocationV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** When false, will still wait for SubstepTime to elapse before updating, but will only run the cable simulation once using all of accumulated simulation time */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "When false, will still wait for SubstepTime to elapse before updating, but will only run the cable simulation once using all of accumulated simulation time" },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bUseSubsteppingV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV = { "bUseSubsteppingV", nullptr, (EPropertyFlags)0x0010040000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bSkipCableUpdateWhenNotVisibleV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV = { "bSkipCableUpdateWhenNotVisibleV", nullptr, (EPropertyFlags)0x0010040000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bSkipCableUpdateWhenNotOwnerRecentlyRenderedV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV = { "bSkipCableUpdateWhenNotOwnerRecentlyRenderedV", nullptr, (EPropertyFlags)0x0010040000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeLengthV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "0.0" },
		{ "Comment", "/** Rest length of the Rope */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Rest length of the Rope" },
		{ "UIMax", "1000.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeLengthV = { "RopeLengthV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, RopeLengthV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeLengthV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeLengthV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_r0_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "UIMax", "100.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_r0 = { "r0", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, r0), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_r0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_r0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_density_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "UIMax", "100.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_density = { "density", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, density), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_density_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_density_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_E_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "UIMin", "0.0" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_E = { "E", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, E), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_E_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_E_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndM_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "UIMin", "1.0" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndM = { "EndM", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, EndM), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndM_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndM_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_PoissonRatio_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "UIMin", "0.0" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_PoissonRatio = { "PoissonRatio", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, PoissonRatio), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_PoissonRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_PoissonRatio_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->pauseSimulation = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation = { "pauseSimulation", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Add stiffness constraints to cable. */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Add stiffness constraints to cable." },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bEnableStiffnessV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV = { "bEnableStiffnessV", nullptr, (EPropertyFlags)0x0010040000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SubstepTimeV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/** Controls the simulation substep time for the cable */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Controls the simulation substep time for the cable" },
		{ "UIMax", "0.1" },
		{ "UIMin", "0.000001" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SubstepTimeV = { "SubstepTimeV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, SubstepTimeV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SubstepTimeV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SubstepTimeV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSegmentsV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMin", "1" },
		{ "Comment", "/** How many segments the cable has */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "How many segments the cable has" },
		{ "UIMax", "20" },
		{ "UIMin", "1" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSegmentsV = { "NumSegmentsV", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, NumSegmentsV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSegmentsV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSegmentsV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "Comment", "/**\n\x09 *\x09""EXPERIMENTAL. Perform sweeps for each cable particle, each substep, to avoid collisions with the world.\n\x09 *\x09Uses the Collision Preset on the component to determine what is collided with.\n\x09 *\x09This greatly increases the cost of the cable simulation.\n\x09 */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "EXPERIMENTAL. Perform sweeps for each cable particle, each substep, to avoid collisions with the world.\nUses the Collision Preset on the component to determine what is collided with.\nThis greatly increases the cost of the cable simulation." },
	};
#endif
	void Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV_SetBit(void* Obj)
	{
		((UPDRopeComponent*)Obj)->bEnableCollisionV = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV = { "bEnableCollisionV", nullptr, (EPropertyFlags)0x0010040000000005, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UPDRopeComponent), &Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV_SetBit, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_CollisionFrictionV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "Comment", "/** If collision is enabled, control how much sliding friction is applied when cable is in contact. */" },
		{ "EditCondition", "bEnableCollision" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "If collision is enabled, control how much sliding friction is applied when cable is in contact." },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_CollisionFrictionV = { "CollisionFrictionV", nullptr, (EPropertyFlags)0x0010040000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, CollisionFrictionV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_CollisionFrictionV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_CollisionFrictionV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeForceV_MetaData[] = {
		{ "Category", "VerletRope Forces" },
		{ "Comment", "/** Force vector (world space) applied to all particles in cable. */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Force vector (world space) applied to all particles in cable." },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeForceV = { "RopeForceV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, RopeForceV), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeForceV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeForceV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeGravityScaleV_MetaData[] = {
		{ "Category", "VerletRope Forces" },
		{ "Comment", "/** Scaling applied to world gravity affecting this cable. */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Scaling applied to world gravity affecting this cable." },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeGravityScaleV = { "RopeGravityScaleV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, RopeGravityScaleV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeGravityScaleV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeGravityScaleV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeWidthV_MetaData[] = {
		{ "Category", "VerletRope Rendering" },
		{ "ClampMin", "0.01" },
		{ "Comment", "/** How wide the cable geometry is */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "How wide the cable geometry is" },
		{ "UIMax", "50.0" },
		{ "UIMin", "0.01" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeWidthV = { "RopeWidthV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, RopeWidthV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeWidthV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeWidthV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSidesV_MetaData[] = {
		{ "Category", "VerletRope Rendering" },
		{ "ClampMax", "16" },
		{ "ClampMin", "1" },
		{ "Comment", "/** Number of sides of the cable geometry */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "Number of sides of the cable geometry" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSidesV = { "NumSidesV", nullptr, (EPropertyFlags)0x0010000000000015, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, NumSidesV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSidesV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSidesV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_TileMaterialV_MetaData[] = {
		{ "Category", "VerletRope Rendering" },
		{ "Comment", "/** How many times to repeat the material along the length of the cable */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "How many times to repeat the material along the length of the cable" },
		{ "UIMax", "8" },
		{ "UIMin", "0.1" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_TileMaterialV = { "TileMaterialV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, TileMaterialV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_TileMaterialV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_TileMaterialV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SolverIterationsV_MetaData[] = {
		{ "Category", "VerletRope" },
		{ "ClampMax", "16" },
		{ "ClampMin", "1" },
		{ "Comment", "/** The number of solver iterations controls how 'stiff' the cable is */" },
		{ "ModuleRelativePath", "Public/PDRopeComponent.h" },
		{ "ToolTip", "The number of solver iterations controls how 'stiff' the cable is" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SolverIterationsV = { "SolverIterationsV", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UPDRopeComponent, SolverIterationsV), METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SolverIterationsV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SolverIterationsV_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPDRopeComponent_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachStartV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bAttachEndV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_AttachEndToSocketNameV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndLocationV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bUseSubsteppingV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotVisibleV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bSkipCableUpdateWhenNotOwnerRecentlyRenderedV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeLengthV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_r0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_density,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_E,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_EndM,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_PoissonRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_pauseSimulation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableStiffnessV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SubstepTimeV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSegmentsV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_bEnableCollisionV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_CollisionFrictionV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeForceV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeGravityScaleV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_RopeWidthV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_NumSidesV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_TileMaterialV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPDRopeComponent_Statics::NewProp_SolverIterationsV,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPDRopeComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPDRopeComponent>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UPDRopeComponent_Statics::ClassParams = {
		&UPDRopeComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UPDRopeComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::PropPointers),
		0,
		0x00B010A4u,
		METADATA_PARAMS(Z_Construct_UClass_UPDRopeComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPDRopeComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPDRopeComponent()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UPDRopeComponent_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UPDRopeComponent, 4122925213);
	template<> PD_API UClass* StaticClass<UPDRopeComponent>()
	{
		return UPDRopeComponent::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UPDRopeComponent(Z_Construct_UClass_UPDRopeComponent, &UPDRopeComponent::StaticClass, TEXT("/Script/PD"), TEXT("UPDRopeComponent"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPDRopeComponent);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
