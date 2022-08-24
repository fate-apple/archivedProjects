#include "SecondBasePassMeshProcessor.h"
#include "MeshPassProcessor.h"

#include "BasePassRendering.h"
#include "ScenePrivate.h"

#include "VelocityRendering.h"
#include "SceneUtils.h"
#include "Materials/Material.h"
#include "PostProcess/SceneRenderTargets.h"
#include "MaterialShaderType.h"
#include "MaterialShader.h"
#include "MeshMaterialShader.h"
#include "ShaderBaseClasses.h"
#include "SceneRendering.h"
#include "DeferredShadingRenderer.h"
#include "ScenePrivate.h"
#include "ScreenSpaceRayTracing.h"
#include "PostProcess/PostProcessMotionBlur.h"
#include "UnrealEngine.h"
#if WITH_EDITOR
#include "Misc/CoreMisc.h"
#include "Interfaces/ITargetPlatform.h"
#include "Interfaces/ITargetPlatformManagerModule.h"
#endif
#include "VisualizeTexture.h"
#include "MeshPassProcessor.inl"
#include "DebugProbeRendering.h"
#include "RendererModule.h"



class FToon1VS : public FMeshMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FToon1VS, MeshMaterial);

	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		// Compile for default material.
		const bool bIsDefault = Parameters.MaterialParameters.bIsSpecialEngineMaterial;

		// Compile for masked materials.
		const bool bIsMasked = !Parameters.MaterialParameters.bWritesEveryPixel;

		// Compile for opaque and two-sided materials.
		const bool bIsOpaqueAndTwoSided = (Parameters.MaterialParameters.bIsTwoSided && !IsTranslucentBlendMode(Parameters.MaterialParameters.BlendMode));

		// Compile for materials which modify meshes.
		const bool bMayModifyMeshes = Parameters.MaterialParameters.bMaterialMayModifyMeshPosition;

		const bool bHasPlatformSupport = PlatformSupportsVelocityRendering(Parameters.Platform);

		/**
		 * Any material with a vertex factory incompatible with base pass velocity generation must generate
		 * permutations for this shader. Shaders which don't fall into this set are considered "simple" enough
		 * to swap against the default material. This massively simplifies the calculations.
		 */
		const bool bIsSeparateVelocityPassRequired = (bIsDefault || bIsMasked || bIsOpaqueAndTwoSided || bMayModifyMeshes) &&
			FVelocityRendering::IsSeparateVelocityPassRequiredByVertexFactory(Parameters.Platform, Parameters.VertexFactoryType->SupportsStaticLighting());

		// The material may explicitly override and request that it be rendered into the velocity pass.
		const bool bIsSeparateVelocityPassRequiredByMaterial = Parameters.MaterialParameters.bIsTranslucencyWritingVelocity;

		return bHasPlatformSupport && (bIsSeparateVelocityPassRequired || bIsSeparateVelocityPassRequiredByMaterial);
	}

	FToon1VS() = default;
	FToon1VS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMeshMaterialShader(Initializer)
	{}
};

class FToon1PS : public FMeshMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FToon1PS, MeshMaterial);

	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		return FToon1VS::ShouldCompilePermutation(Parameters);
	}

	static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetRenderTargetOutputFormat(0, PF_A16B16G16R16);

		// We support velocity on thin trnaslucent only with masking, and only if the material is only made of thin translucent shading model.
		OutEnvironment.SetDefine(TEXT("VELOCITY_THIN_TRANSLUCENT_MODE"), Parameters.MaterialParameters.ShadingModels.HasOnlyShadingModel(MSM_ThinTranslucent));
	}

	FToon1PS() = default;
	FToon1PS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMeshMaterialShader(Initializer)
	{}
};

IMPLEMENT_SHADER_TYPE(,FToon1VS, TEXT("/Engine/Private/Toon1.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(,FToon1PS, TEXT("/Engine/Private/Toon1.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADERPIPELINE_TYPE_VSPS(Toon1Pipeline, FToon1VS, FToon1PS, true);

FSecondBasePassMeshProcessor::FSecondBasePassMeshProcessor( 
	const FScene* InScene, 
	ERHIFeatureLevel::Type InFeatureLevel, 
	const FSceneView* InViewIfDynamicMeshCommand, 
	const FMeshPassProcessorRenderState& InDrawRenderState, 
	FMeshPassDrawListContext* InDrawListContext, 
	ETranslucencyPass::Type InTranslucencyPassType /* = ETranslucencyPass::TPT_MAX */ 
)
	: FMeshPassProcessor(InScene, InFeatureLevel, InViewIfDynamicMeshCommand, InDrawListContext)
	, PassDrawRenderState(InDrawRenderState)
{
	PassDrawRenderState.SetViewUniformBuffer(Scene->UniformBuffers.ViewUniformBuffer);
}

void FSecondBasePassMeshProcessor::AddMeshBatch(
	const FMeshBatch& RESTRICT MeshBatch, 
	uint64 BatchElementMask, 
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, 
	int32 StaticMeshId
)
{
	const FMaterialRenderProxy* FallBackMaterialRenderProxyPtr = nullptr;
	const FMaterial& Material = MeshBatch.MaterialRenderProxy->GetMaterialWithFallback(FeatureLevel, FallBackMaterialRenderProxyPtr);

	const EBlendMode BlendMode = Material.GetBlendMode();
	//const ERasterizerFillMode MeshFillMode = ComputeMeshFillMode(MeshBatch, Material);
	const ERasterizerFillMode MeshFillMode = ERasterizerFillMode::FM_Wireframe;
	//const ERasterizerCullMode MeshCullMode = ComputeMeshCullMode(MeshBatch, Material);
	const ERasterizerCullMode MeshCullMode = ERasterizerCullMode::CM_CCW;
	const bool bIsTranslucent = IsTranslucentBlendMode(BlendMode);

	if (
		(!PrimitiveSceneProxy || PrimitiveSceneProxy->ShouldRenderInMainPass())
		&& ShouldIncludeDomainInMeshPass(Material.GetMaterialDomain())
		)
	{
		if (BlendMode == BLEND_Opaque
			&& MeshBatch.VertexFactory->SupportsPositionOnlyStream()
			&& Material.WritesEveryPixel()
			)
		{
			const FMaterialRenderProxy& MaterialRenderProxy = FallBackMaterialRenderProxyPtr ? *FallBackMaterialRenderProxyPtr : *MeshBatch.MaterialRenderProxy;
			Process(MeshBatch, BatchElementMask, StaticMeshId, PrimitiveSceneProxy, MaterialRenderProxy, Material, MeshFillMode, MeshCullMode);
		}
	}
}


bool GetToon1PassShaders(
	const FMaterial& Material,
	FVertexFactoryType* VertexFactoryType,
	ERHIFeatureLevel::Type FeatureLevel,
	TShaderRef<FToon1VS>& VertexShader,
	TShaderRef<FToon1PS>& PixelShader)
{
	FMaterialShaderTypes ShaderTypes;

	// Don't use pipeline if we have hull/domain shaders
	ShaderTypes.PipelineType = &Toon1Pipeline;

	ShaderTypes.AddShaderType<FToon1VS>();
	ShaderTypes.AddShaderType<FToon1PS>();

	FMaterialShaders Shaders;
	if (!Material.TryGetShaders(ShaderTypes, VertexFactoryType, Shaders))
	{
		return false;
	}

	Shaders.TryGetVertexShader(VertexShader);
	Shaders.TryGetPixelShader(PixelShader);
	return true;
}

void FSecondBasePassMeshProcessor::Process(
	const FMeshBatch& MeshBatch, 
	uint64 BatchElementMask, 
	int32 staticMeshId, 
	const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy, 
	const FMaterialRenderProxy& RESTRICT MaterialRenderProxy, 
	const FMaterial& RESTRICT MaterialResource, 
	ERasterizerFillMode MeshFillMode, 
	ERasterizerCullMode MeshCullMode
)
{
	FUniformLightMapPolicy NoLightmapPolicy(LMP_NO_LIGHTMAP);
	typedef FUniformLightMapPolicy LightMapPolicyType;
	TMeshProcessorShaders<
		FToon1VS,
		FToon1PS> ToonShaders;

	const FVertexFactory* VertexFactory = MeshBatch.VertexFactory;
	const bool bRenderSkylight = false;
	const bool bRenderAtmosphericFog = false;
	// GetBasePassShaders<LightMapPolicyType>(
	// 	MaterialResource,
	// 	VertexFactory->GetType(),
	// 	NoLightmapPolicy,
	// 	FeatureLevel,
	// 	bRenderAtmosphericFog,
	// 	bRenderSkylight,
	// 	&SkyPassShaders.VertexShader,
	// 	&SkyPassShaders.PixelShader
	// 	);
	if (!GetToon1PassShaders(
		MaterialResource,
		VertexFactory->GetType(),
		FeatureLevel,
		ToonShaders.VertexShader,
		ToonShaders.PixelShader))
	{
		return ;
	}

	TBasePassShaderElementData<LightMapPolicyType> ShaderElementData(nullptr);
	ShaderElementData.InitializeMeshMaterialData(ViewIfDynamicMeshCommand, PrimitiveSceneProxy, MeshBatch, staticMeshId, false);

	const FMeshDrawCommandSortKey SortKey = CalculateMeshStaticSortKey(ToonShaders.VertexShader, ToonShaders.PixelShader);

	BuildMeshDrawCommands(
		MeshBatch,
		BatchElementMask,
		PrimitiveSceneProxy,
		MaterialRenderProxy,
		MaterialResource,
		PassDrawRenderState,
		ToonShaders,
		MeshFillMode,
		MeshCullMode,
		SortKey,
		EMeshPassFeatures::Default,
		ShaderElementData);
}

FMeshPassProcessor* CreateSecondBasePassProcessor(const FScene* Scene, const FSceneView* InViewIfDynamicMeshCommand, FMeshPassDrawListContext* InDrawListContext)
{
	FMeshPassProcessorRenderState DrawRenderState;

	FExclusiveDepthStencil::Type BasePassDepthStencilAccess_NoDepthWrite = FExclusiveDepthStencil::Type(Scene->DefaultBasePassDepthStencilAccess & ~FExclusiveDepthStencil::DepthWrite);
	SetupBasePassState(BasePassDepthStencilAccess_NoDepthWrite, false, DrawRenderState);
	const ERHIFeatureLevel::Type FeatureLevel = Scene ? Scene->GetFeatureLevel() : (InViewIfDynamicMeshCommand ? InViewIfDynamicMeshCommand->GetFeatureLevel() : GMaxRHIFeatureLevel);
	
	DrawRenderState.SetBlendState(TStaticBlendStateWriteMask<CW_RGBA>::GetRHI());
	DrawRenderState.SetDepthStencilAccess(Scene->DefaultBasePassDepthStencilAccess);
	DrawRenderState.SetDepthStencilState(TStaticDepthStencilState<true, CF_DepthNear>::GetRHI());
	return new(FMemStack::Get()) FSecondBasePassMeshProcessor(Scene , Scene->GetFeatureLevel(),InViewIfDynamicMeshCommand, DrawRenderState, InDrawListContext);
}
FRegisterPassProcessorCreateFunction RegisterSecondBasePass(&CreateSecondBasePassProcessor, EShadingPath::Deferred, EMeshPass::MultiBasePass, EMeshPassFlags::MainView);