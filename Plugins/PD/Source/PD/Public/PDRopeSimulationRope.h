#pragma once
#include "CRProperty.h"
#include "PDRopeComponent.h"


namespace Chaos
{
	class FRopeSimulationSolver;
	class FRopeSimulationRope final
	{
	public:
		//FRopeSimulationRope();
		FRopeSimulationRope(UPDRopeComponent* InMesh, uint32 InGroupId,bool bUseLODIndexOverride,int32 LODIndexOverride);
		~FRopeSimulationRope();
		void SetMesh(UPDRopeComponent* InMesh);
		void Add(FRopeSimulationSolver* Solver);
		void PreUpdate(FRopeSimulationSolver* Solver);
		void Update(FRopeSimulationSolver* Solver);
		//void Enable(FRopeSimulationSolver* Solver, bool bEnable);
		int32 GetOffset(const FRopeSimulationSolver* Solver, int32 InLODIndex) const;
		int32 GetNumParticles(int32 InLODIndex) const;
		TConstArrayView<TVector<float, 3>> GetParticlePositions(const FRopeSimulationSolver* Solver) const;
		TConstArrayView<Quat> GetEdgeQuats(const FRopeSimulationSolver* Solver) const;
		void SetAnimationPos(FRopeSimulationSolver* Solver, TConstArrayView<TVector<float, 3>> InAnimationPos);
		void SetParticleM(FRopeSimulationSolver* Solver, int Offset, PDScalar InM);
		uint32 GetGroupId() const { return GroupId; }

	private:
		// LOD data
		struct FLODData
		{
			// Input mesh
			const int32 NumParticles;
			const TConstArrayView<uint32> Indices;
			// Per Solver data
			struct FSolverData
			{
				int32 PosOffset;
				int32 QuatOffset;
				TArray<TVector<int32, 3>> Constraints;
			};
			TMap<FRopeSimulationSolver*, FSolverData> SolverData;

			// Stats
			int32 NumKinenamicParticles;
			int32 NumDynammicParticles;

			//FLODData(int32 InNumParticles, const TConstArrayView<uint32>& InIndices, const TArray<TConstArrayView<float>>& InWeightMaps);
			FLODData(int32 InNumParticles);

			void Add(FRopeSimulationSolver* Solver, FRopeSimulationRope* Cloth, int32 LODIndex);
			void Remove(FRopeSimulationSolver* Solver);

			void Update(FRopeSimulationSolver* Solver, FRopeSimulationRope* Cloth);

			void Enable(FRopeSimulationSolver* Solver, bool bEnable) const;

			void ResetStartPose(FRopeSimulationSolver* Solver) const;


		};

		UPDRopeComponent* Mesh;
		uint32 GroupId;
		bool bUseLODIndexOverride;
		int32 LODIndexOverride;
		
		
		TArray<FLODData> LODData;
		TMap<FRopeSimulationSolver*, int32> LODIndices;

	public:
	 CRProperty Property;
	};
}
