// Copyright Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	LumenMeshCards.h
=============================================================================*/

#pragma once

#include "TextureLayout3d.h"
#include "RenderResource.h"
#include "MeshCardRepresentation.h"
#include "LumenSparseSpanArray.h"

class FLumenCard;
class FPrimitiveSceneInfo;

namespace Lumen
{
	constexpr uint32 NumAxisAlignedDirections = 6;

	void UpdateCardSceneBuffer(FRDGBuilder& GraphBuilder, const FSceneViewFamily& ViewFamily, FScene* Scene);
};

// Lumen Card 的集合，与 FLumenPrimitiveGroup 一一对应，PrimitiveGroupIndex 成员指向 FLumenPrimitiveGroup 的索引，同样在 FLumenPrimitiveGroup 中也包含一个指向 FLumenMeshCards 的索引 MeshCardsIndex。
// 由于 MeshCards 的 Card 数据在 FLumenSceneData 中是连续存储的，因此只需保存 FLumenSceneData 中 Cards 数组的起始索引和数量即可访问所有 Card。
class FLumenMeshCards
{
public:
	void Initialize(
		const FMatrix& InLocalToWorld, 
		const FBox& InLocalBounds,
		int32 InPrimitiveGroupIndex,
		uint32 InFirstCardIndex,
		uint32 InNumCards,
		bool InFarField,
		bool InHeightfield,
		bool InEmissiveLightSource)
	{
		PrimitiveGroupIndex = InPrimitiveGroupIndex;

		LocalBounds = InLocalBounds;
		SetTransform(InLocalToWorld);
		FirstCardIndex = InFirstCardIndex;
		NumCards = InNumCards;
		bFarField = InFarField;
		bHeightfield = InHeightfield;
		bEmissiveLightSource = InEmissiveLightSource;
	}

	void UpdateLookup(const TSparseSpanArray<FLumenCard>& Cards);

	void SetTransform(const FMatrix& InLocalToWorld);

	FBox GetWorldSpaceBounds() const
	{
		const FBox WorldSpaceBounds = LocalBounds.TransformBy(LocalToWorld);
		return WorldSpaceBounds;
	}

	FMatrix LocalToWorld;
	FVector3f LocalToWorldScale;
	FMatrix WorldToLocalRotation;
	FBox LocalBounds;

	//指向 FLumenPrimitiveGroup 的索引
	int32 PrimitiveGroupIndex = -1;
	bool bFarField = false;
	bool bHeightfield = false;
	bool bEmissiveLightSource = false;

	uint32 FirstCardIndex = 0;
	uint32 NumCards = 0;
	uint32 CardLookup[Lumen::NumAxisAlignedDirections];
};

namespace LumenMeshCards
{
	float GetCardMinSurfaceArea(bool bEmissiveLightSource);
};