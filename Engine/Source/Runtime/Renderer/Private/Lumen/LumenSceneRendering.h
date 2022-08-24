// Copyright Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	LumenSceneRendering.h
=============================================================================*/

#pragma once

#include "RHIDefinitions.h"
#include "SceneView.h"
#include "SceneRendering.h"
#include "ScenePrivate.h"
#include "LumenSceneData.h"

class FLumenSceneData;
class FLumenCardScene;

inline bool DoesPlatformSupportLumenGI(EShaderPlatform Platform, bool bSkipProjectCheck = false)
{
	extern int32 GLumenSupported;
	return (bSkipProjectCheck || GLumenSupported)
		&& FDataDrivenShaderPlatformInfo::GetSupportsLumenGI(Platform)
		&& !IsForwardShadingEnabled(Platform);
}

//FCardPageRenderData 中包含 Nanite 和 Non-Nanite Mesh 的 Instance 信息
class FCardPageRenderData
{
public:
	int32 PrimitiveGroupIndex = -1;

	// CardData
	const int32 CardIndex = -1;
	const int32 PageTableIndex = -1;
	bool bDistantScene = false;
	FVector4f CardUVRect;
	//apture Atlas 和 Card Atlas 的区域.本质上就是数据存储在物理纹理上的位置和大小，这也是地址转换的关键数据
	//CardCaptureAtlasRect，表示捕获的材质属性在 Capture Altas 所在的物理纹理的区域，捕获时根据这个区域写入到对应的材质属性 Capture Altas 上。
	//这个区域是在 FLumenSurfaceCacheAllocator::Allocate 函数中根据 FLumenPageTableEntry 进行子分配得到，并且分配的区域大小与 FLumenPageTableEntry 的 SubAllocationSize 大小一致
	FIntRect CardCaptureAtlasRect;
	//SurfaceCacheAtlasRect，表示 Card 对应在 Card Altas 所在的物理纹理的区域，
	//在 Capture To Card Atlas 过程中以及后续的 Lumen Scene Direct Lighting 时会根据这个区域写入到 Card Altas 上。这个区域实际上就是 ReallocVirtualSurface 中分配的FLumenPageTableEntry 的 PhysicalAtlasRect 成员
	//FLumenPageTableEntry 的 SubAllocationSize 表示这个区域的大小，因此 PhysicalAtlasRect 也是根据 SubAllocationSize 计算而来。
	FIntRect SurfaceCacheAtlasRect;

	FLumenCardOBB CardWorldOBB;

	FViewMatrices ViewMatrices;
	FMatrix ProjectionMatrixUnadjustedForRHI;

	int32 StartMeshDrawCommandIndex = 0;
	int32 NumMeshDrawCommands = 0;

	//Nanite Instance 数据
	TArray<uint32, SceneRenderingAllocator> NaniteInstanceIds;
	TArray<FNaniteCommandInfo, SceneRenderingAllocator> NaniteCommandInfos;
	float NaniteLODScaleFactor = 1.0f;

	bool bResampleLastLighting = false;

	// Non-Nanite mesh inclusive instance ranges to draw
	//Non-Nanite Instance信息
	TArray<uint32, SceneRenderingAllocator> InstanceRuns;

	FCardPageRenderData(const FViewInfo& InMainView,
		const FLumenCard& InLumenCard,
		FVector4f InCardUVRect,
		FIntRect InCardCaptureAtlasRect,
		FIntRect InSurfaceCacheAtlasRect,
		int32 InPrimitiveGroupIndex,
		int32 InCardIndex,
		int32 InCardPageIndex,
		bool bResampleLastLighting);

	void UpdateViewMatrices(const FViewInfo& MainView);

	void PatchView(FRHICommandList& RHICmdList, const FScene* Scene, FViewInfo* View) const;
};

struct FResampledCardCaptureAtlas
{
	FIntPoint Size;
	FRDGTextureRef DirectLighting = nullptr;
	FRDGTextureRef IndirectLighting = nullptr;
	FRDGTextureRef NumFramesAccumulated = nullptr;
};

FMeshPassProcessor* CreateLumenCardNaniteMeshProcessor(
	const FScene* Scene,
	const FSceneView* InViewIfDynamicMeshCommand,
	FMeshPassDrawListContext* InDrawListContext);

namespace Lumen
{
	inline bool HasPrimitiveNaniteMeshBatches(const FPrimitiveSceneProxy* Proxy)
	{
		return Proxy && Proxy->ShouldRenderInMainPass() && Proxy->AffectsDynamicIndirectLighting();
	}
};

extern void SetupLumenCardSceneParameters(FRDGBuilder& GraphBuilder, const FScene* Scene, FLumenCardScene& OutParameters);
extern void UpdateLumenMeshCards(FScene& Scene, const FDistanceFieldSceneData& DistanceFieldSceneData, FLumenSceneData& LumenSceneData, FRDGBuilder& GraphBuilder);

BEGIN_SHADER_PARAMETER_STRUCT(FLumenReflectionCompositeParameters, )
	SHADER_PARAMETER(float, MaxRoughnessToTrace)
	SHADER_PARAMETER(float, InvRoughnessFadeLength)
END_SHADER_PARAMETER_STRUCT()

BEGIN_SHADER_PARAMETER_STRUCT(FLumenScreenSpaceBentNormalParameters, )
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float3>, ScreenBentNormal)
	SHADER_PARAMETER(uint32, UseScreenBentNormal)
END_SHADER_PARAMETER_STRUCT()