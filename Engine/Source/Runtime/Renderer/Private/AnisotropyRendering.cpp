// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnisotropyRendering.h"
#include "PrimitiveSceneProxy.h"
#include "MeshPassProcessor.inl"
#include "ScenePrivate.h"
#include "DeferredShadingRenderer.h"

DECLARE_GPU_STAT_NAMED(RenderAnisotropyPass, TEXT("Render Anisotropy Pass"));

static int32 GAnisotropicMaterials = 0;
static FAutoConsoleVariableRef CVarAnisotropicMaterials(
	TEXT("r.AnisotropicMaterials"),
	GAnisotropicMaterials,
	TEXT("Whether anisotropic BRDF is used for material with anisotropy."),
	ECVF_Scalability | ECVF_RenderThreadSafe
	);

bool SupportsAnisotropicMaterials(ERHIFeatureLevel::Type FeatureLevel, EShaderPlatform ShaderPlatform)
{
	return GAnisotropicMaterials
		&& FeatureLevel >= ERHIFeatureLevel::SM5
		&& FDataDrivenShaderPlatformInfo::GetSupportsAnisotropicMaterials(ShaderPlatform);
}

static bool IsAnisotropyPassCompatible(const EShaderPlatform Platform, FMaterialShaderParameters MaterialParameters)
{
	return 
		FDataDrivenShaderPlatformInfo::GetSupportsAnisotropicMaterials(Platform) &&
		MaterialParameters.bHasAnisotropyConnected &&
		!IsTranslucentBlendMode(MaterialParameters.BlendMode) && 
		MaterialParameters.ShadingModels.HasAnyShadingModel({ MSM_DefaultLit, MSM_ClearCoat, MSM_ToonHair });
}

class FAnisotropyVS : public FMeshMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FAnisotropyVS, MeshMaterial);
// public: 
// 	using ShaderMetaType = FMeshMaterialShaderType; 
// 	using ShaderMapType = FMeshMaterialShaderMap; 
//
// 	static ShaderMetaType StaticType; 

	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		// Compile if supported by the hardware.
		const bool bIsFeatureSupported = IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);

		return 
			bIsFeatureSupported && 
			IsAnisotropyPassCompatible(Parameters.Platform, Parameters.MaterialParameters) &&
			FMeshMaterialShader::ShouldCompilePermutation(Parameters);
	}

	FAnisotropyVS() = default;
	FAnisotropyVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMeshMaterialShader(Initializer)
	{}
};

class FAnisotropyPS : public FMeshMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FAnisotropyPS, MeshMaterial);

	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		return FAnisotropyVS::ShouldCompilePermutation(Parameters);
	}

	static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}

	FAnisotropyPS() = default;
	FAnisotropyPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMeshMaterialShader(Initializer)
	{}
};

//construct static F##MeshMaterial##ShaderType
IMPLEMENT_SHADER_TYPE(, FAnisotropyVS, TEXT("/Engine/Private/AnisotropyPassShader.usf"), TEXT("MainVertexShader"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FAnisotropyPS, TEXT("/Engine/Private/AnisotropyPassShader.usf"), TEXT("MainPixelShader"), SF_Pixel);
IMPLEMENT_SHADERPIPELINE_TYPE_VSPS(AnisotropyPipeline, FAnisotropyVS, FAnisotropyPS, true);

DECLARE_CYCLE_STAT(TEXT("AnisotropyPass"), STAT_CLP_AnisotropyPass, STATGROUP_ParallelCommandListMarkers);

FAnisotropyMeshProcessor::FAnisotropyMeshProcessor(
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

FMeshPassProcessor* CreateAnisotropyPassProcessor(const FScene* Scene, const FSceneView* InViewIfDynamicMeshCommand, FMeshPassDrawListContext* InDrawListContext)
{
	const ERHIFeatureLevel::Type FeatureLevel = Scene ? Scene->GetFeatureLevel() : (InViewIfDynamicMeshCommand ? InViewIfDynamicMeshCommand->GetFeatureLevel() : GMaxRHIFeatureLevel);

	FMeshPassProcessorRenderState AnisotropyPassState;

	AnisotropyPassState.SetBlendState(TStaticBlendState<>::GetRHI());
	AnisotropyPassState.SetDepthStencilState(TStaticDepthStencilState<false, CF_Equal>::GetRHI());

	return new(FMemStack::Get()) FAnisotropyMeshProcessor(Scene, FeatureLevel, InViewIfDynamicMeshCommand, AnisotropyPassState, InDrawListContext);
}

FRegisterPassProcessorCreateFunction RegisterAnisotropyPass(
	&CreateAnisotropyPassProcessor,
	EShadingPath::Deferred, 
	EMeshPass::AnisotropyPass, 
	EMeshPassFlags::CachedMeshCommands | EMeshPassFlags::MainView
	);

bool GetAnisotropyPassShaders(
	const FMaterial& Material,
	FVertexFactoryType* VertexFactoryType,
	ERHIFeatureLevel::Type FeatureLevel,
	TShaderRef<FAnisotropyVS>& VertexShader,
	TShaderRef<FAnisotropyPS>& PixelShader
	)
{
	FMaterialShaderTypes ShaderTypes;
	ShaderTypes.PipelineType = &AnisotropyPipeline;
	ShaderTypes.AddShaderType<FAnisotropyVS>();
	ShaderTypes.AddShaderType<FAnisotropyPS>();

	FMaterialShaders Shaders;
	if (!Material.TryGetShaders(ShaderTypes, VertexFactoryType, Shaders))
	{
		return false;
	}

	Shaders.TryGetVertexShader(VertexShader);
	Shaders.TryGetPixelShader(PixelShader);
	check(VertexShader.IsValid() && PixelShader.IsValid());

	return true;
}

void FAnisotropyMeshProcessor::AddMeshBatch(
	const FMeshBatch& RESTRICT MeshBatch, 
	uint64 BatchElementMask, 
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, 
	int32 StaticMeshId /* = -1 */ 
	)
{
	if (SupportsAnisotropicMaterials(FeatureLevel, GShaderPlatformForFeatureLevel[FeatureLevel]) && MeshBatch.bUseForMaterial)
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

bool FAnisotropyMeshProcessor::TryAddMeshBatch(
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
	if (Material.MaterialUsesAnisotropy_RenderThread() && bIsNotTranslucent && Material.GetShadingModels().HasAnyShadingModel({ MSM_DefaultLit, MSM_ClearCoat }))
	{
		const FMeshDrawingPolicyOverrideSettings OverrideSettings = ComputeMeshOverrideSettings(MeshBatch);
		const ERasterizerFillMode MeshFillMode = ComputeMeshFillMode(MeshBatch, Material, OverrideSettings);
		const ERasterizerCullMode MeshCullMode = ComputeMeshCullMode(MeshBatch, Material, OverrideSettings);

		bResult = Process(MeshBatch, BatchElementMask, StaticMeshId, PrimitiveSceneProxy, MaterialRenderProxy, Material, MeshFillMode, MeshCullMode);
	}

	return bResult;
}

bool FAnisotropyMeshProcessor::Process(
	const FMeshBatch& MeshBatch, 
	uint64 BatchElementMask, 
	int32 StaticMeshId, 
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
	const FMaterialRenderProxy& RESTRICT MaterialRenderProxy, 
	const FMaterial& RESTRICT MaterialResource,
	ERasterizerFillMode MeshFillMode, 
	ERasterizerCullMode MeshCullMode 
	)
{
	const FVertexFactory* VertexFactory = MeshBatch.VertexFactory;

	TMeshProcessorShaders<
		FAnisotropyVS,
		FAnisotropyPS> AnisotropyPassShaders;

	if (!GetAnisotropyPassShaders(
		MaterialResource,
		VertexFactory->GetType(),
		FeatureLevel,
		AnisotropyPassShaders.VertexShader,
		AnisotropyPassShaders.PixelShader))
	{
		return false;
	}

	FMeshMaterialShaderElementData ShaderElementData;
	ShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, StaticMeshId, false);

	const FMeshDrawCommandSortKey SortKey = CalculateMeshStaticSortKey(AnisotropyPassShaders.VertexShader, AnisotropyPassShaders.PixelShader);

	BuildMeshDrawCommands(
		MeshBatch,
		BatchElementMask,
		PrimitiveSceneProxy,
		MaterialRenderProxy,
		MaterialResource,
		PassDrawRenderState,
		AnisotropyPassShaders,
		MeshFillMode,
		MeshCullMode,
		SortKey,
		EMeshPassFeatures::Default,
		ShaderElementData
		);

	return true;
}

bool ShouldRenderAnisotropyPass(const FViewInfo& View)
{
	if (!SupportsAnisotropicMaterials(View.FeatureLevel, View.GetShaderPlatform()))
	{
		return false;
	}

	if (IsAnyForwardShadingEnabled(GetFeatureLevelShaderPlatform(View.FeatureLevel)))
	{
		return false;
	}

	if (View.ShouldRenderView() && View.ParallelMeshDrawCommandPasses[EMeshPass::AnisotropyPass].HasAnyDraw())
	{
		return true;
	}

	return false;
}

bool ShouldRenderAnisotropyPass(const TArray<FViewInfo>& Views)
{
	for (const FViewInfo& View : Views)
	{
		if (ShouldRenderAnisotropyPass(View))
		{
			return true;
		}
	}

	return false;
}

BEGIN_SHADER_PARAMETER_STRUCT(FAnisotropyPassParameters, )
	SHADER_PARAMETER_STRUCT_INCLUDE(FViewShaderParameters, View)
	//init by ParallelMeshPass.BuildRenderingCommands() -> TaskContext.InstanceCullingResult.GetDrawParameters(OutInstanceCullingDrawParams);
	SHADER_PARAMETER_STRUCT_INCLUDE(FInstanceCullingDrawParams, InstanceCullingDrawParams)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

void FDeferredShadingSceneRenderer::RenderAnisotropyPass(
	FRDGBuilder& GraphBuilder, 
	FSceneTextures& SceneTextures,
	bool bDoParallelPass
)
{
	RDG_CSV_STAT_EXCLUSIVE_SCOPE(GraphBuilder, RenderAnisotropyPass);
	SCOPED_NAMED_EVENT(FDeferredShadingSceneRenderer_RenderAnisotropyPass, FColor::Emerald);
	SCOPE_CYCLE_COUNTER(STAT_AnisotropyPassDrawTime);
	//FRDGGPUStatScopeGuard
	RDG_GPU_STAT_SCOPE(GraphBuilder, RenderAnisotropyPass);

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		FViewInfo& View = Views[ViewIndex];

		if (View.ShouldRenderView())
		{
			FParallelMeshDrawCommandPass& ParallelMeshPass = View.ParallelMeshDrawCommandPasses[EMeshPass::AnisotropyPass];

			if (!ParallelMeshPass.HasAnyDraw())
			{
				continue;
			}

			View.BeginRenderView();

			auto* PassParameters = GraphBuilder.AllocParameters<FAnisotropyPassParameters>();
			PassParameters->View = View.GetShaderParameters();
			PassParameters->RenderTargets.DepthStencil = FDepthStencilBinding(SceneTextures.Depth.Target, ERenderTargetLoadAction::ELoad, FExclusiveDepthStencil::DepthRead_StencilNop);

			ParallelMeshPass.BuildRenderingCommands(GraphBuilder, Scene->GPUScene, PassParameters->InstanceCullingDrawParams);
			if (bDoParallelPass)
			{
				AddClearRenderTargetPass(GraphBuilder, SceneTextures.GBufferF);

				PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneTextures.GBufferF, ERenderTargetLoadAction::ELoad);

				GraphBuilder.AddPass(
					RDG_EVENT_NAME("AnisotropyPassParallel"),
					PassParameters,
					ERDGPassFlags::Raster | ERDGPassFlags::SkipRenderPass,
					[this, &View, &ParallelMeshPass, PassParameters](FRHICommandListImmediate& RHICmdList)
				{
					FRDGParallelCommandListSet ParallelCommandListSet(RHICmdList, GET_STATID(STAT_CLP_AnisotropyPass), *this, View, FParallelCommandListBindings(PassParameters));

					ParallelMeshPass.DispatchDraw(&ParallelCommandListSet, RHICmdList, &PassParameters->InstanceCullingDrawParams);
				});
			}
			else
			{
				PassParameters->RenderTargets[0] = FRenderTargetBinding(SceneTextures.GBufferF, ERenderTargetLoadAction::EClear);

				GraphBuilder.AddPass(
					RDG_EVENT_NAME("AnisotropyPass"),
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
