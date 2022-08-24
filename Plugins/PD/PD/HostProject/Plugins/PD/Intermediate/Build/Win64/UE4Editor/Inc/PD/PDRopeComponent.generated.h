// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FVector;
class USceneComponent;
class AActor;
#ifdef PD_PDRopeComponent_generated_h
#error "PDRopeComponent.generated.h already included, missing '#pragma once' in PDRopeComponent.h"
#endif
#define PD_PDRopeComponent_generated_h

#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_SPARSE_DATA
#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execSetPauseSimulation); \
	DECLARE_FUNCTION(execGetCableParticleLocationsV); \
	DECLARE_FUNCTION(execGetAttachedComponentV); \
	DECLARE_FUNCTION(execGetAttachedActorV); \
	DECLARE_FUNCTION(execSetAttachEndToV); \
	DECLARE_FUNCTION(execSetAttachEndToComponentV);


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execSetPauseSimulation); \
	DECLARE_FUNCTION(execGetCableParticleLocationsV); \
	DECLARE_FUNCTION(execGetAttachedComponentV); \
	DECLARE_FUNCTION(execGetAttachedActorV); \
	DECLARE_FUNCTION(execSetAttachEndToV); \
	DECLARE_FUNCTION(execSetAttachEndToComponentV);


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUPDRopeComponent(); \
	friend struct Z_Construct_UClass_UPDRopeComponent_Statics; \
public: \
	DECLARE_CLASS(UPDRopeComponent, UMeshComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/PD"), NO_API) \
	DECLARE_SERIALIZER(UPDRopeComponent)


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_INCLASS \
private: \
	static void StaticRegisterNativesUPDRopeComponent(); \
	friend struct Z_Construct_UClass_UPDRopeComponent_Statics; \
public: \
	DECLARE_CLASS(UPDRopeComponent, UMeshComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/PD"), NO_API) \
	DECLARE_SERIALIZER(UPDRopeComponent)


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UPDRopeComponent(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UPDRopeComponent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPDRopeComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPDRopeComponent); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPDRopeComponent(UPDRopeComponent&&); \
	NO_API UPDRopeComponent(const UPDRopeComponent&); \
public:


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UPDRopeComponent(UPDRopeComponent&&); \
	NO_API UPDRopeComponent(const UPDRopeComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UPDRopeComponent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UPDRopeComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UPDRopeComponent)


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_PRIVATE_PROPERTY_OFFSET
#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_54_PROLOG
#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_SPARSE_DATA \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_RPC_WRAPPERS \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_INCLASS \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_SPARSE_DATA \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h_57_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class PDRopeComponent."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> PD_API UClass* StaticClass<class UPDRopeComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_PD_Source_PD_Public_PDRopeComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
