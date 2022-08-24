#include "PDEvolution.h"

#include "PDAnimationConstraints.h"
#include "PDAnimationQuatConstraints.h"
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
, MGravity(TVector<T, d>((T)0., (T)0., (T)-9.80665))
, MDamping(Damping)
, MConstraintInitsActiveView(MConstraintInits)
, MConstraintRulesActiveView(MConstraintRules)
,MTime(0)
,HardConstraintWeight(0)
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
		MGroupGravityForces[GroupId].SetAcceleration(MGravity);

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
					//MParticles.F(Index)  =Gravity*0;
					
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
				}
				else  // Process kinematic particles
					{
						MKinematicUpdate(MParticles, Dt, MTime, Index);
					}
			//change to m while following PD step.copy p to flat vector
			MParticles.P(Index) *=0.01;
			Sx[Index*3] = MParticles.P(Index)[0];
			Sx[Index*3+1] = MParticles.P(Index)[1];
			Sx[Index*3+2] = MParticles.P(Index)[2];
		},RangeSize < MinParallelBatchSize);
}

template<class T, int d>
void TPDEvolution<T, d>::SetAnimationPos(TArray<TVector<int32,1> >& MConstraints,TArray<Vec3>& AnimationPos,
	const int32 Offset,
	const int32 Range,
	const int32 MinParallelBatchSize)
{
	for(int i=Offset;i<Offset+Range;i++){
			if (MParticles.InvM(i) == (T)0.)  // Process dynamic particles
			{
				//can not use PhysicsParallelFor
				MConstraints.Add(TVector<int32,1>{i});
				
				AnimationPos.Add(Vec3( MParticles.P(i)[0],MParticles.P(i)[1],MParticles.P(i)[2]));
			}
		}
	
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
	//w = sqrt(1.0 - x*x - y*y - z*z);
	w=0;

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
	//w = sqrt(1.0 - x*x - y*y - z*z);
	w=0;
	
	return Quat(w,x,y,z); //coeff store in (x,y,z,w) way. what a mass!
}

Quat QuatScale(const Quat& quat,PDScalar scale)
{
	auto m = quat.toRotationMatrix();
	m*= scale;
	return Quat(m);
}
Quat QuatPlus(const Quat& quat1,const Quat& quat2)
{
	return Quat(quat1.w()+quat2.w(),quat1.x()+quat2.x(),quat1.y()+quat2.y(),quat1.z()+quat2.z());
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
			Vec3 w;
			Quat q;
			w = MEdges.W(Index).cross(MEdges.J(Index)*MEdges.W(Index));
			w = MEdges.W(Index)+Dt* MEdges.Jinv(Index)*(MEdges.Torque(Index)-w);
			MEdges.Sw(Index) = w;
			w*= 0.5*Dt;
			//Pos2Quat(MEdges.Sw(Index),MEdges.Su(Index));
			//first order
			 Quat DQ =  MEdges.Q(Index) * MEdges.Su(Index);
			// MEdges.Su(Index) =QuatPlus( MEdges.Q(Index) ,DQ).normalized();
			double l = w.norm(); // magnitude
		
			if (l > 1e-6)
			{
				w *= sin(l) / l;
				DQ =  Quat(cos(l), w.x(), w.y(), w.z());
			} else
			{
				DQ =  Quat(1.0, w.x(), w.y(), w.z());
			}
			q = (DQ* MEdges.Q(Index)).normalized();
			

			//TODO: may change implemention and use same data structure
			MEdges.Su(Index)= q;
			Su[Index*4] =q.w();
			Su[Index*4+1] = q.x();
			Su[Index*4+2] = q.y();
			Su[Index*4+3] = q.z();
			//Su[Index*4] << MEdges.Su(Index).w(),MEdges.Su(Index).x(),MEdges.Su(Index).y(),MEdges.Su(Index).z();
		},RangeSize < MinParallelBatchSize);
}

	template<class T, int d>
void TPDEvolution<T, d>::AdvanceOneTimeStep(const T Dt)
{
	MTime +=Dt;

	//TODO
	MParticles.InvM(MParticles.Size()-1) = 0;
	//auto debug2 = EigenMatrix2StdVector(CRPosRhs);
	std::vector<std::vector<float>> debugSx,debugSu,debugPosLhs,debugPosRhs,debugQuatLhs,debugQuatRhs;
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
	TArray<Vec3> AnimationPos;
	TArray<TVector<int32, 1>> MConstraints;
	MParticlesActiveView.RangeFor(
		[this, Dt, MinParallelBatchSize,&MConstraints,&AnimationPos](TPBDParticles<T, d>& Particles, int32 Offset, int32 Range)
		{
			SetAnimationPos(MConstraints,AnimationPos, Offset, Range, MinParallelBatchSize);
		}, true);
	FPDAnimationConstraints AnimationConstraints(MoveTemp( MConstraints),MoveTemp(AnimationPos),HardConstraintWeight);
	//TODO
	TArray<Quat> AnimationQuat = TArray<Quat>{Quat(1,0,0,0)};
	MConstraints = TArray<TVector<int32, 1>>{TVector<int32, 1>{0}};
	FPDAnimationQuatConstraints AnimationQuatConstraints(MoveTemp( MConstraints),MoveTemp(AnimationQuat),HardConstraintWeight);
	
	debugSx = EigenMatrix2StdVector(Sx);
	debugSu  = EigenMatrix2StdVector(Su);
	{
		CRPosRhs.setZero();
		CRQuatRhs.setZero();
		CRPosLhs = M*((1/Dt)*(1/Dt));
		CRQuatLhs = J*((1/Dt)*(1/Dt));
		
		auto CRPosLhsInit = CRPosLhs;
		auto CRQuatLhsInit = CRQuatLhs;
	}
	auto debugj = EigenMatrix2StdVector(J);
	//  debugPosLhs = EigenMatrix2StdVector(CRPosLhs);
	//  debugPosRhs  = EigenMatrix2StdVector(CRPosRhs);
	// debugQuatLhs = EigenMatrix2StdVector(CRQuatLhs);
	// debugQuatRhs  = EigenMatrix2StdVector(CRQuatRhs);

	
	{
		for (int32 i = 0; i < MNumIterations; ++i)
		{
			bool bInitLhs = (i==0);
			CRPosRhs =M*((1/Dt)*(1/Dt))*Sx;
			CRQuatRhs =J*((1/Dt)*(1/Dt))*Su;
			AnimationConstraints.computeProjections(Sx,Su,CRPosLhs ,CRQuatLhs,CRPosRhs,CRQuatRhs, bInitLhs);
			AnimationQuatConstraints.computeProjections(Sx,Su,CRPosLhs ,CRQuatLhs,CRPosRhs,CRQuatRhs, bInitLhs);
			MConstraintRulesActiveView.RangeFor(
				[this, bInitLhs](TArray<TFunction<void(Vec& NewPos,Vec& NewQuat,SparseMatrix&CRPosLhs ,SparseMatrix&CRQuatLhs,Vec&CRPosRhs,Vec&CRQuatRhs,bool bInitLhs )>>& ConstraintRules,
					int32 Offset, int32 Range)
				{
					for (int32 ConstraintIndex = Offset; ConstraintIndex < Range; ++ConstraintIndex)
					{
						//TODO: detach projection and eigen struct?
						ConstraintRules[ConstraintIndex](Sx,Su,CRPosLhs,CRQuatLhs,CRPosRhs,CRQuatRhs,bInitLhs); //pi = projectConstrainSet
					}}, true);
			
			
			CRPosSolver.compute(CRPosLhs);						//can omit if no collision
			CRQuatSolver.compute(CRQuatLhs);
			Sx = CRPosSolver.solve(CRPosRhs);
			Su = CRQuatSolver.solve(CRQuatRhs);
			NormalizeQuatVec(Su);
			debugPosLhs = EigenMatrix2StdVector(CRPosLhs);
			debugPosRhs  = EigenMatrix2StdVector(CRPosRhs);
			debugQuatLhs = EigenMatrix2StdVector(CRQuatLhs);
			debugQuatRhs  = EigenMatrix2StdVector(CRQuatRhs);
			debugSx = EigenMatrix2StdVector(Sx);
			debugSu  = EigenMatrix2StdVector(Su);
			
		}
	}
	{
		MParticlesActiveView.ParallelFor(
			[Dt,this](TPBDParticles<T, d>& Particles, int32 Index)
			{
				if(Particles.InvM(Index)!=0)
				{
					Particles.P(Index)[0] = Sx[3*Index];
					Particles.P(Index)[1] = Sx[3*Index+1];
					Particles.P(Index)[2] = Sx[3*Index+2];
				}

				//end PD steps and change m to cm to return simulation data
				Particles.P(Index) *=100;
				Particles.V(Index) = (Particles.P(Index) - Particles.X(Index)) / Dt;
				Particles.X(Index) = Particles.P(Index);
			}, MinParallelBatchSize);
	}
	
	{
		
		MEdgesActiveView.ParallelFor(
			[Dt,this](TCosseratEdges<T, d>& Edges, int32 Index)
			{
				Edges.Su(Index)=  Quat( Su[4*Index],Su[4*Index+1],Su[4*Index+2],Su[4*Index+3]);
				Edges.Sw(Index) =Quat2Pos(Edges.Q(Index).conjugate()*Edges.Su(Index)) *2 / Dt;
				Edges.Q(Index) = Edges.Su(Index);
				Edges.W(Index) = Edges.Sw(Index);
			}, MinParallelBatchSize);
	}
	MEdges.W(MEdges.Size()-1) = Vec3(0,0,5);
	//MEdges.Torque(MEdges.Size()-1) = Vec3(0,0,1);
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
		//auto debugj = EigenMatrix2StdVector(J);
	
	}
	
}