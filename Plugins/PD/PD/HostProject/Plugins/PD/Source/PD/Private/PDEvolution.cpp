#include "PDEvolution.h"

#include "PDRopeSimulationRope.h"

TAutoConsoleVariable<int32> CVarChaosPDEvolutionMinParallelBatchSize(TEXT("p.Chaos.PDEvolution.MinParallelBatchSize"), 30, TEXT(""), ECVF_Cheat);

using namespace Chaos;

template<class T, int d>
void TPDEvolution<T, d>::AddGroups(int32 NumGroups)
{
	// Add elements
	const uint32 Offset = TArrayCollection::Size();
	TArrayCollection::AddElementsHelper(NumGroups);

	// Set defaults
	for (uint32 GroupId = Offset; GroupId < TArrayCollection::Size(); ++GroupId)
	{
	}
}
template<class T, int d>
TPDEvolution<T, d>::TPDEvolution(TPBDParticles<T, d>&& InParticles, TCosseratEdges<T,d>&& InEdges, int32 NumIterations ,T Damping)
: MParticles(MoveTemp(InParticles))
, MParticlesActiveView(MParticles)
,MEdges(MoveTemp(InEdges))
,MEdgesActiveView(MEdges)
, MNumIterations(NumIterations)
, MGravity(TVector<T, d>((T)0., (T)0., (T)-980.665))
, MDamping(Damping)
, MConstraintInitsActiveView(MConstraintInits)
, MConstraintRulesActiveView(MConstraintRules)
,MTime(0)
{
	TArrayCollection::AddArray(&MGroupVelocityFields);
	TArrayCollection::AddArray(&MGroupForceRules);
	TArrayCollection::AddArray(&MGroupGravityForces);
	
	MParticles.AddArray(&MParticleGroupIds);
};

template<class T, int d>
int32 TPDEvolution<T, d>::AddParticleRange(int32 NumParticles, uint32 GroupId, bool bActivate)
{
	if (NumParticles)
	{
		const int32 Offset = (int32)MParticles.Size();

		MParticles.AddParticles(NumParticles);
		//FlatPos.resize(MParticles.size());
		//FlatQuat.resize(MParticles.size());
		Sx.resize(MParticles.Size()*3);
		

		// Initialize the new particles' group ids
		for (int32 i = Offset; i < (int32)MParticles.Size(); ++i)
		{
			MParticleGroupIds[i] = GroupId;
		}

		// Resize the group parameter arrays
		const uint32 GroupSize = TArrayCollection::Size();
		if (GroupId >= GroupSize)
		{
			AddGroups(GroupId + 1 - GroupSize);
		}

		// Add range
		MParticlesActiveView.AddRange(NumParticles, bActivate);

		return Offset;
	}
	return INDEX_NONE;
}

template<class T, int d>
int32 TPDEvolution<T, d>::AddEdgeRange(int32 NumEdges, uint32 GroupId, bool bActivate)
{
	if (NumEdges)
	{
		const int32 Offset = (int32)MEdges.Size();

		MEdges.AddEdges(NumEdges);
		Su.resize(MEdges.Size()*4);
		// for (int32 i = Offset; i < (int32)MEdges.Size(); ++i)
		// {
		// 	MEdgeGroupIds[i] = GroupId;
		// }

		// Resize the group parameter arrays
		const uint32 GroupSize = TArrayCollection::Size();
		if (GroupId >= GroupSize)
		{
			AddGroups(GroupId + 1 - GroupSize);
		}

		// Add range
		MEdgesActiveView.AddRange(NumEdges, bActivate);

		return Offset;
	}
	return INDEX_NONE;
}

template<class T, int d>
int32 TPDEvolution<T, d>::AddConstraintInitRange(int32 NumConstraints, bool bActivate)
{
	// Add new constraint init functions
	MConstraintInits.AddDefaulted(NumConstraints);

	// Add range
	return MConstraintInitsActiveView.AddRange(NumConstraints, bActivate);
}
template<class T, int d>
int32 TPDEvolution<T, d>::AddConstraintRuleRange(int32 NumConstraints, bool bActivate)
{
	// Add new constraint rule functions
	MConstraintRules.AddDefaulted(NumConstraints);

	// Add range
	return MConstraintRulesActiveView.AddRange(NumConstraints, bActivate);
}

template<class T, int d>
template<bool bForceRule, bool bVelocityField, bool bDampVelocityRule>
void Chaos::TPDEvolution<T, d>::PreIterationUpdate(
	const T Dt,
	const int32 Offset,
	const int32 Range,
	const int32 MinParallelBatchSize)
{
	const uint32 ParticleGroupId = MParticleGroupIds[Offset];
	FVelocityField& VelocityField = MGroupVelocityFields[ParticleGroupId];
	const TFunction<void(TPBDParticles<T, d>&, const T, const int32)>& ForceRule = MGroupForceRules[ParticleGroupId];
	const TVector<T, d>& Gravity = MGroupGravityForces[ParticleGroupId].GetAcceleration();

	// if (bVelocityField)
	// {
	// 	VelocityField.UpdateForces(MParticles, Dt);  // Update force per surface element
	// }

	const int32 RangeSize = Range - Offset;
	PhysicsParallelFor(RangeSize,
		[this, &Offset, &ForceRule, &Gravity,/* &VelocityField,*/ Dt](int32 i)
		{
			const int32 Index = Offset + i;
			if (MParticles.InvM(Index) != (T)0.)  // Process dynamic particles
				{
					// Init forces with GravityForces
					MParticles.F(Index) = Gravity * MParticles.M(Index);  // F = M * G
					
					// Force Rule
					if (bForceRule)
					{
						ForceRule(MParticles, Dt, Index); // F += M * A
					}

					// Velocity Field
					// if (bVelocityField)
					// {
					// 	VelocityField.Apply(MParticles, Dt, Index);
					// }

					// Euler Step Velocity
					MParticles.V(Index) += MParticles.F(Index) * MParticles.InvM(Index) * Dt;

					// Euler Step
					MParticles.P(Index) = MParticles.X(Index) + MParticles.V(Index) * Dt;
				Sx[Index*3] = MParticles.P(Index)[0];
				Sx[Index*3+1] = MParticles.P(Index)[1];
				Sx[Index*3+2] = MParticles.P(Index)[2];
				
				}
				else  // Process kinematic particles
					{
						MKinematicUpdate(MParticles, Dt, MTime, Index);
					}
			//change to m while following PD step
			MParticles.P(Index) *=0.01;
		},RangeSize < MinParallelBatchSize);
}

void Quat2Pos(const Quat& quat,Vec3& pos)
{
	Quat q = quat.normalized();
	pos<<q.x(),q.y(),q.z();
}
void Pos2Quat(const Vec3& pos, Quat& quat)
{
	PDScalar w, x, y, z;
	x = pos.x();
	y =pos.y();
	z = pos.z();
	//Recover the real part
	w = sqrt(1.0 - x*x - y*y - z*z);

	quat.coeffs()<<x,y,z,w;
}
Vec3 Quat2Pos(const Quat& quat)
{
	Quat q = quat.normalized();
	return Vec3(q.x(),q.y(),q.z());
}
Quat Pos2Quat(const Vec3& pos)
{
	PDScalar w, x, y, z;
	x = pos.x();
	y =pos.y();
	z = pos.z();
	//Recover the real part
	w = sqrt(1.0 - x*x - y*y - z*z);

	return Quat(w,x,y,z); //coeff store in (x,y,z,w) way. what a mass!
}
template<class T, int d>
void Chaos::TPDEvolution<T, d>::PreIterationQuatUpdate(
	const T Dt,
	const int32 Offset,
	const int32 Range,
	const int32 MinParallelBatchSize)
{
	const uint32 ParticleGroupId = MParticleGroupIds[Offset];

	
	const int32 RangeSize = Range - Offset;
	PhysicsParallelFor(RangeSize,
		[this, &Offset, Dt](int32 i)
		{
			const int32 Index = Offset + i;
			MEdges.Sw(Index) = MEdges.W(Index).cross(MEdges.J(Index)*MEdges.W(Index));
			MEdges.Sw(Index) = MEdges.W(Index)+Dt* MEdges.Jinv(Index)*(MEdges.Torque(Index)-MEdges.Sw(Index));
			Pos2Quat(MEdges.Sw(Index),MEdges.Su(Index));
			MEdges.Su(Index) = Pos2Quat(Quat2Pos(MEdges.Q(Index)) + Dt*0.5 * Quat2Pos(MEdges.Q(Index) * MEdges.Su(Index)));

			//TODO: may change implemention and use same data structure
			Su[Index*4] = MEdges.Su(Index).w();
			Su[Index*4+1] = MEdges.Su(Index).x();
			Su[Index*4+2] = MEdges.Su(Index).y();
			Su[Index*4+3] = MEdges.Su(Index).z();
			//Su[Index*4] << MEdges.Su(Index).w(),MEdges.Su(Index).x(),MEdges.Su(Index).y(),MEdges.Su(Index).z();
		},RangeSize < MinParallelBatchSize);
}


	template<class T, int d>
void TPDEvolution<T, d>::AdvanceOneTimeStep(const T Dt)
{
	MTime +=Dt;
	//CRPosLhs.setZero();
	//CRQuatLhs.setZero();
	CRPosRhs.setZero();
	CRQuatRhs.setZero();
	CRPosLhs = M*((1/Dt)*(1/Dt));
	CRQuatLhs = J*((1/Dt)*(1/Dt));

	const int32 MinParallelBatchSize = CVarChaosPDEvolutionMinParallelBatchSize.GetValueOnAnyThread();
	MParticlesActiveView.RangeFor(
		[this, Dt, MinParallelBatchSize](TPBDParticles<T, d>& Particles, int32 Offset, int32 Range)
		{
			PreIterationUpdate</*bForceRule =*/ false, /*bVelocityField =*/ false, /*bDampVelocityRule =*/ true>(Dt, Offset, Range, MinParallelBatchSize);
		}, true);

	MEdgesActiveView.RangeFor(
		[this, Dt, MinParallelBatchSize](TCosseratEdges<T, d>& Edges, int32 Offset, int32 Range)
		{
			PreIterationQuatUpdate(Dt, Offset, Range, MinParallelBatchSize);
		}, true);

	{
		for (int32 i = 0; i < MNumIterations; ++i)
		{
			MConstraintRulesActiveView.RangeFor(
				[this, Dt](TArray<TFunction<void(const TPBDParticles<T, d>&,const TCosseratEdges<T, d>&,const T,SparseMatrix& ,SparseMatrix&,Vec&,Vec& )>>& ConstraintRules,
					int32 Offset, int32 Range)
				{
					for (int32 ConstraintIndex = Offset; ConstraintIndex < Range; ++ConstraintIndex)
					{
						//TODO: detach projection and eigen struct?
						ConstraintRules[ConstraintIndex](MParticles,MEdges, Dt,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs); //pi = projectConstrainSet
					}
				}, true);
		}
	}
	//Compute real rhs and lhs for Eq.(13),use eigen for now;
	//TODO: better solver
	CRPosRhs+=M*((1/Dt)*(1/Dt))*Sx;
	CRQuatRhs+=J*((1/Dt)*(1/Dt))*Su;
	CRPosSolver.compute(CRPosLhs);						//can omit if no collision
	CRQuatSolver.compute(CRQuatLhs);
	Vec NewPos = CRPosSolver.solve(CRPosRhs);
	Vec NewQuat  = CRQuatSolver.solve(CRQuatRhs);

	
	{
		MParticlesActiveView.ParallelFor(
			[Dt,&NewPos](TPBDParticles<T, d>& Particles, int32 Index)
			{
				Particles.P(Index)[0] = NewPos[3*Index];
				Particles.P(Index)[1] = NewPos[3*Index+1];
				Particles.P(Index)[2] = NewPos[3*Index+2];

				//end PD steps and change m to cm to return simulation data
				Particles.P(Index) *=100;
				Particles.V(Index) = (Particles.P(Index) - Particles.X(Index)) / Dt;
				Particles.X(Index) = Particles.P(Index);
			}, MinParallelBatchSize);
	}
	
	{
		// Particle update, V = (P - X) / Dt; X = P;
		MEdgesActiveView.ParallelFor(
			[Dt,&NewQuat](TCosseratEdges<T, d>& Edges, int32 Index)
			{
				Edges.Su(Index)=  Quat( NewQuat[4*Index],NewQuat[4*Index+1],NewQuat[4*Index+2],NewQuat[4*Index+3]);
				Edges.Sw(Index) =Quat2Pos(Edges.Q(Index)*Edges.Su(Index)) *2 / Dt;
				Edges.Q(Index) = Edges.Su(Index);
				Edges.W(Index) = Edges.Sw(Index);
			}, MinParallelBatchSize);
	}
}
template<class T, int d>
void TPDEvolution<T, d>::UpdateMatrix()
{
	M.resize(MParticles.Size()*3,MParticles.Size()*3);
	M.setZero();
	for (uint32 Index = 0; Index < MParticles.Size(); Index++)
	{
		M.coeffRef(3*Index,3*Index)  = MParticles.M(Index);
		M.coeffRef(3*Index+1,3*Index+1)  = MParticles.M(Index);
		M.coeffRef(3*Index+2,3*Index+2)  = MParticles.M(Index);
	}
	J.resize(MEdges.Size()*4,MEdges.Size()*4);
	J.setZero();
	for (uint32 Index = 0; Index < MEdges.Size(); Index++)
	{
		J.coeffRef(4*Index,4*Index)  = 0;
		J.coeffRef(4*Index+1,4*Index+1)  = MEdges.J(Index).coeff(0,0);
		J.coeffRef(4*Index+2,4*Index+2)  = MEdges.J(Index).coeff(1,1);
		J.coeffRef(4*Index+3,4*Index+3)  = MEdges.J(Index).coeff(2,2);
	
	}
}