#include "ToonOutlineRendering.h"

// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrimitiveSceneProxy.h"
#include "MeshPassProcessor.inl"
#include "ScenePrivate.h"
#include "DeferredShadingRenderer.h"

DECLARE_GPU_STAT_NAMED(RenderToonOutlinePass, TEXT("Render ToonOutline Pass"));



IMPLEMENT_SHADER_TYPE(, FToonOutlineVS, TEXT("/Engine/Private/ToonDataPassShader.usf"), TEXT("MainVertexShader"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FToonOutlinePS, TEXT("/Engine/Private/ToonDataPassShader.usf"), TEXT("MainPixelShader"), SF_Pixel);
IMPLEMENT_SHADERPIPELINE_TYPE_VSPS(ToonOutlinePipeline, FToonOutlineVS, FToonOutlinePS, true);

DECLARE_CYCLE_STAT(TEXT("ToonOutlinePass"), STAT_CLP_ToonOutlinePass, STATGROUP_ParallelCommandListMarkers);


FToonOutlineMeshProcessor::FToonOutlineMeshProcessor(
	const FScene* Scene, 
	ERHIFeatureLevel::Type InFeatureLevel,
	const FSceneView* InViewIfDynamicMeshCommand,
	const FMeshPassProcessorRenderState& InPassDrawRenderState, 
	FMeshPassDrawListContext* InDrawListContext
	)
	: FMeshPassProcessor(Scene, InFeatureLevel, InViewIfDynamicMeshCommand, InDrawListContext)
	, PassDrawRenderState(InPassDrawRenderState)
{
}


FMeshPassProcessor* CreateToonOutlinePassProcessor(const FScene* Scene, const FSceneView* InViewIfDynamicMeshCommand, FMeshPassDrawListContext* InDrawListContext)
{
	const ERHIFeatureLevel::Type FeatureLevel = Scene ? Scene->GetFeatureLevel() : (InViewIfDynamicMeshCommand ? InViewIfDynamicMeshCommand->GetFeatureLevel() : GMaxRHIFeatureLevel);
	FMeshPassProcessorRenderState PassDrawRenderState;
	PassDrawRenderState.SetBlendState(TStaticBlendStateWriteMask<CW_RGBA>::GetRHI());
	PassDrawRenderState.SetDepthStencilAccess(Scene->DefaultBasePassDepthStencilAccess);
	PassDrawRenderState.SetDepthStencilState(TStaticDepthStencilState<true, CF_DepthNearOrEqual>::GetRHI());
 
	return new(FMemStack::Get()) FToonOutlineMeshProcessor(Scene, FeatureLevel, InViewIfDynamicMeshCommand, PassDrawRenderState, InDrawListContext);
}




bool GetBorderGlowPassShaders(
	const FMaterial& Material,
	FVertexFactoryType* VertexFactoryType,
	TShaderRef<FToonOutlineVS>& VertexShader,
	TShaderRef<FToonOutlinePS>& PixelShader)
{
	FMaterialShaderTypes ShaderTypes;
	ShaderTypes.PipelineType = &ToonOutlinePipeline;
	FMaterialShaders Shaders;
	ShaderTypes.AddShaderType<FToonOutlineVS>();
	ShaderTypes.AddShaderType<FToonOutlinePS>();
	if (!Material.TryGetShaders(ShaderTypes, VertexFactoryType, Shaders))
	{
		return false;
	}
 
	Shaders.TryGetVertexShader(VertexShader);
	Shaders.TryGetPixelShader(PixelShader);
	return true;
}

bool FToonOutlineMeshProcessor::Process(
	const FMeshBatch& MeshBatch,
	uint64 BatchElementMask,
	int32 StaticMeshId,
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
	const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
	const FMaterial& RESTRICT MaterialResource,
	ERasterizerFillMode MeshFillMode,
	ERasterizerCullMode MeshCullMode)
{
	const FVertexFactory* VertexFactory = MeshBatch.VertexFactory;
 
	TMeshProcessorShaders<
		FToonOutlineVS,
		FToonOutlinePS> BorderGlowPassShader;
 
	if (!GetBorderGlowPassShaders(MaterialResource, VertexFactory->GetType(), BorderGlowPassShader.VertexShader, BorderGlowPassShader.PixelShader))
		return false;
	
	FMeshMaterialShaderElementData ShaderElementData;
	ShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, StaticMeshId, false);
 
	const FMeshDrawCommandSortKey SortKey = CalculateMeshStaticSortKey(BorderGlowPassShader.VertexShader, BorderGlowPassShader.PixelShader);
	
 
	BuildMeshDrawCommands(
		MeshBatch,
		BatchElementMask,
		PrimitiveSceneProxy,
		MaterialRenderProxy,
		MaterialResource,
		PassDrawRenderState,
		BorderGlowPassShader,
		MeshFillMode,
		MeshCullMode,
		SortKey,
		EMeshPassFeatures::Default,
		ShaderElementData);
	
	return true;
}


void FToonOutlineMeshProcessor::AddMeshBatch(
	const FMeshBatch& RESTRICT MeshBatch, 
	uint64 BatchElementMask, 
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, 
	int32 StaticMeshId /* = -1 */ 
	)
{
	if (MeshBatch.bUseForMaterial)
	{
		const FMaterialRenderProxy* MaterialRenderProxy = MeshBatch.MaterialRenderProxy;
		while (MaterialRenderProxy)
		{
			const FMaterial* Material = MaterialRenderProxy->GetMaterialNoFallback(FeatureLevel);
			if (Material)
			{
				if (TryAddMeshBatch(MeshBatch, BatchElementMask, PrimitiveSceneProxy, StaticMeshId, *MaterialRenderProxy, *Material))
				{
					break;
				}
			}

			MaterialRenderProxy = MaterialRenderProxy->GetFallback(FeatureLevel);
		}
	}
}

bool FToonOutlineMeshProcessor::TryAddMeshBatch(
	const FMeshBatch& RESTRICT MeshBatch,
	uint64 BatchElementMask,
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
	int32 StaticMeshId,
	const FMaterialRenderProxy& MaterialRenderProxy,
	const FMaterial& Material)
{
	const EBlendMode BlendMode = Material.GetBlendMode();
	const bool bIsNotTranslucent = BlendMode == BLEND_Opaque || BlendMode == BLEND_Masked;

	bool bResult = true;
	if ( bIsNotTranslucent && Material.GetShadingModels().HasAnyShadingModel({ MSM_ToonStandard, MSM_ToonSkin, MSM_ToonHair}))
	{
		const FMeshDrawingPolicyOverrideSettings OverrideSettings = ComputeMeshOverrideSettings(MeshBatch);
		const ERasterizerFillMode MeshFillMode = ComputeMeshFillMode(MeshBatch, Material, OverrideSettings);
		//const ERasterizerCullMode MeshCullMode = ERasterizerCullMode::CM_CCW;
		const ERasterizerCullMode MeshCullMode = ComputeMeshCullMode(MeshBatch, Material, OverrideSettings);

		bResult = Process(MeshBatch, BatchElementMask, StaticMeshId, PrimitiveSceneProxy, MaterialRenderProxy, Material, MeshFillMode, MeshCullMode);
	}

	return bResult;
}

BEGIN_SHADER_PARAMETER_STRUCT(FToonOutlinePassParameters, )
	SHADER_PARAMETER_STRUCT_INCLUDE(FViewShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FInstanceCullingDrawParams, InstanceCullingDrawParams)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

void FDeferredShadingSceneRenderer::RenderToonDataPass(
	FRDGBuilder& GraphBuilder, 
	FSceneTextures& SceneTextures,
	bool bDoParallelPass
)
{
	RDG_CSV_STAT_EXCLUSIVE_SCOPE(GraphBuilder, RenderToonOutlinePass);
	SCOPED_NAMED_EVENT(FDeferredShadingSceneRenderer_RenderToonOutlinePass, FColor::Emerald);
	SCOPE_CYCLE_COUNTER(STAT_RenderToonOutlinePassDrawTime);
	RDG_GPU_STAT_SCOPE(GraphBuilder, RenderToonOutlinePass);

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		FViewInfo& View = Views[ViewIndex];

		if (View.ShouldRenderView())
		{
			FParallelMeshDrawCommandPass& ParallelMeshPass = View.ParallelMeshDrawCommandPasses[EMeshPass::ToonOutline];

			if (!ParallelMeshPass.HasAnyDraw())
			{
				continue;
			}

			View.BeginRenderView();

			auto* PassParameters = GraphBuilder.AllocParameters<FToonOutlinePassParameters>();
			PassParameters->View = View.GetShaderParameters();
			PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(SceneTextures.Depth.Target, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthWrite_StencilWrite);

			ParallelMeshPass.BuildRenderingCommands(GraphBuilder, Scene->GPUScene, PassParameters->InstanceCullingDrawParams);
			if (bDoParallelPass)
			{
				AddClearRenderTargetPass(GraphBuilder, SceneTextures.ToonDataTexture01);

				PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneTextures.ToonDataTexture01, ERenderTargetLoadAction::ELoad);

				GraphBuilder.AddPass(
					RDG_EVENT_NAME("ToonOulinePassParallel"),
					PassParameters,
					ERDGPassFlags::Raster | ERDGPassFlags::SkipRenderPass,
					[this, &View, &ParallelMeshPass, PassParameters](FRHICommandListImmediate& RHICmdList)
				{
					FRDGParallelCommandListSet ParallelCommandListSet(RHICmdList, GET_STATID(STAT_CLP_ToonOutlinePass), *this, View, FParallelCommandListBindings(PassParameters));

					ParallelMeshPass.DispatchDraw(&ParallelCommandListSet, RHICmdList, &PassParameters->InstanceCullingDrawParams);
				});
			}
			else
			{
				PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneTextures.ToonDataTexture01, ERenderTargetLoadAction::EClear);

				GraphBuilder.AddPass(
					RDG_EVENT_NAME("ToonOulinePassParallel"),
					PassParameters,
					ERDGPassFlags::Raster,
					[this, &View, &ParallelMeshPass, PassParameters](FRHICommandList& RHICmdList)
				{
					SetStereoViewport(RHICmdList, View);

					ParallelMeshPass.DispatchDraw(nullptr, RHICmdList, &PassParameters->InstanceCullingDrawParams);
				});
			}
		}
	}
}

FRegisterPassProcessorCreateFunction RegisterToonOutlinePass(
	&CreateToonOutlinePassProcessor,
	EShadingPath::Deferred, 
	EMeshPass::ToonOutline, 
	EMeshPassFlags::CachedMeshCommands | EMeshPassFlags::MainView);