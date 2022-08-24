
#include "CoreMinimal.h"
#include "RHI.h"
#include "RendererInterface.h"
#include "MeshPassProcessor.h"


class FScene;

static bool IsToonOutlinePassCompatible(const EShaderPlatform Platform, FMaterialShaderParameters MaterialParameters)
{
	return 
		!IsTranslucentBlendMode(MaterialParameters.BlendMode) && 
		MaterialParameters.ShadingModels.HasAnyShadingModel({ MSM_ToonStandard,  MSM_ToonHair, MSM_ToonSkin});
}

class FToonOutlineVS : public FMeshMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FToonOutlineVS, MeshMaterial);

	// decided which material should compile with this pipeline
	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		// Compile if supported by the hardware.
		const bool bIsFeatureSupported = IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);

		return 
			bIsFeatureSupported && FMeshMaterialShader::ShouldCompilePermutation(Parameters)
		&&IsToonOutlinePassCompatible(Parameters.Platform, Parameters.MaterialParameters);
	}

	FToonOutlineVS() = default;
	FToonOutlineVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMeshMaterialShader(Initializer)
	{
		//BorderGlowSize.Bind(Initializer.ParameterMap, TEXT("BorderGlowSize"));
	}


private:
	//LAYOUT_FIELD(FShaderParameter, BorderGlowSize);
};

class FToonOutlinePS : public FMeshMaterialShader
{
public:
	DECLARE_SHADER_TYPE(FToonOutlinePS, MeshMaterial);

	static bool ShouldCompilePermutation(const FMeshMaterialShaderPermutationParameters& Parameters)
	{
		return FToonOutlineVS::ShouldCompilePermutation(Parameters);
	}

	static void ModifyCompilationEnvironment(const FMaterialShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FMaterialShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}

	FToonOutlinePS() = default;
	FToonOutlinePS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMeshMaterialShader(Initializer)
	{
		BorderGlowColor.Bind(Initializer.ParameterMap, TEXT("BorderGlowColor"));

	}
	
private:
	LAYOUT_FIELD(FShaderParameter, BorderGlowColor);
};

class FToonOutlineMeshProcessor : public FMeshPassProcessor
{
public:
	FToonOutlineMeshProcessor(
			const FScene* Scene,
			ERHIFeatureLevel::Type InFeatureLevel,
			const FSceneView* InViewIfDynamicMeshCommand,
			const FMeshPassProcessorRenderState& InPassDrawRenderState,
			FMeshPassDrawListContext* InDrawListContext
			);

	FMeshPassProcessorRenderState PassDrawRenderState;

	virtual void AddMeshBatch(
		const FMeshBatch& RESTRICT MeshBatch,
		uint64 BatchElementMask,
		const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
		int32 StaticMeshId = -1
		) override final;
	
protected:
	bool TryAddMeshBatch(
		const FMeshBatch& RESTRICT MeshBatch,
		uint64 BatchElementMask,
		const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
		int32 StaticMeshId,
		const FMaterialRenderProxy& MaterialRenderProxy,
		const FMaterial& Material);

	bool Process(
		const FMeshBatch& MeshBatch,
		uint64 BatchElementMask,
		int32 StaticMeshId,
		const FPrimitiveSceneProxy* RESTRICT PrimitiveSceneProxy,
		const FMaterialRenderProxy& RESTRICT MaterialRenderProxy,
		const FMaterial& RESTRICT MaterialResource,
		ERasterizerFillMode MeshFillMode,
		ERasterizerCullMode MeshCullMode
		);
};


