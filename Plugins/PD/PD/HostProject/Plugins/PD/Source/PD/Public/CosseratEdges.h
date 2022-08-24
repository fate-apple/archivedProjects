// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "PDTypes.h"
#include "Chaos/ArrayCollection.h"
#include "Chaos/ArrayCollectionArray.h"


namespace Chaos
{
	template<class T, int d>
	class TCosseratEdges :  public TArrayCollection
	{
	public:
		TCosseratEdges()
		{
			AddArray(&MQ);
			AddArray(&MSu);
			AddArray(&MW);
			AddArray(&MSw);
			AddArray(&MTorque);
			AddArray(&MJ);
			AddArray(&MJinv);
		}
		TCosseratEdges(const TCosseratEdges<T, d>& Other) = delete;
		TCosseratEdges(TCosseratEdges<T, d>&& Other)
		: TArrayCollection(), MQ(MoveTemp(Other.MQ)), MSu(MoveTemp(Other.MSu)), MW(MoveTemp(Other.MW)), MSw(MoveTemp(Other.MSw)), MTorque(MoveTemp(Other.MTorque)), MJ(MoveTemp(Other.MJ)), MJinv(MoveTemp(Other.MJinv))
		{
			AddEdges(Other.Size());
			AddArray(&MQ);
			AddArray(&MSu);
			AddArray(&MW);
			AddArray(&MSw);
			AddArray(&MTorque);
			AddArray(&MJ);
			AddArray(&MJinv);
			Other.MSize = 0;
		}
		virtual ~TCosseratEdges()
		{}
		Quat& Q(const int32 index) { return MQ[index]; }
		const Quat& Q(const int32 index) const { return MQ[index]; }
		Quat& Su(const int32 index) { return MSu[index]; }
		const Quat& Su(const int32 index) const { return MSu[index]; }
		
		Vec3& Sw(const int32 index) { return MSw[index]; }
		const Vec3& Sw(const int32 index) const { return MSw[index]; }
		Vec3& W(const int32 index) { return MW[index]; }
		const Vec3& W(const int32 index) const { return MW[index]; }
		Vec3& Torque(const int32 index) { return MTorque[index]; }
		const Vec3& Torque(const int32 index) const { return MTorque[index]; }
		
		Mat3& J(const int32 index) { return MJ[index]; }
		const Mat3& J(const int32 index) const { return MJ[index]; }
		Mat3& Jinv(const int32 index) { return MJinv[index]; }
		const Mat3& Jinv(const int32 index) const { return MJinv[index]; }

		void AddEdges(const int32 Num)
		{
			AddElementsHelper(Num);
			//IncrementDirtyValidation();
		}
	private:
		//change per iteration
		TArrayCollectionArray<Quat> MQ;
		TArrayCollectionArray<Quat> MSu; //new Q
		TArrayCollectionArray<Vec3> MW;	
		TArrayCollectionArray<Vec3> MSw;	//new W
		TArrayCollectionArray<Vec3> MTorque;
		
		TArrayCollectionArray<Mat3> MJ;	//<j1,j2,j3>
		TArrayCollectionArray<Mat3> MJinv;
	};
}
