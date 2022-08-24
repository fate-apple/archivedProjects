#pragma once
#include "Eigen/Eigen/Core"
#include "Eigen/Eigen/Sparse"
#include "Eigen/Eigen/Geometry"


	using   PDScalar = float;
	template < int Rows, int Cols, int Options = (Eigen::ColMajor) >
	using MatrixT = Eigen::Matrix<PDScalar, Rows, Cols, Options>;
	using Vec3 = MatrixT<3,1>;
	using Vec4 = MatrixT<4,1>;
	using Vec7 = MatrixT<7,1>;
	using Vec = MatrixT<Eigen::Dynamic,1>;
	using Mat = MatrixT<Eigen::Dynamic,Eigen::Dynamic>;
	using Mat3 = MatrixT<3,3>;
	template<int Options = Eigen::ColMajor>
	using SparseMatrixT = Eigen::SparseMatrix<PDScalar, Options>;	// A typedef of the sparse matrix of Eigen.
	typedef SparseMatrixT<> SparseMatrix;
	typedef MatrixT<Eigen::Dynamic, 3> Vec3s;

	typedef Eigen::Quaternion<PDScalar> Quat;
	template < int Rows, int Cols>
	using ArrayQ = Eigen::Array<Quat, Rows, Cols>;
	typedef ArrayQ<Eigen::Dynamic, 1> Quats;

	typedef SparseMatrixT<Eigen::RowMajor> SparseMatrixRM; //M_star
	
	//typedef Matri


// Data produced by a clothing simulation
struct FRopeSimulData
{
	void Reset()
	{
		Positions.Reset();
		//Normals.Reset();
	}

	// Positions of the simulation mesh particles
	TArray<Chaos::TVector<PDScalar,3>> Positions;

	// Normals at the simulation mesh particles
	//TArray<FVector> Normals;

	// Transform applied per position/normal element when loaded
	//FTransform Transform;

	// Transform relative to the component to update clothing root transform when not ticking clothing but rendering a component
	//FTransform ComponentRelativeTransform;
};