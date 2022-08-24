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
	using Vec8 = MatrixT<8,1>;
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
	TArray<Quat> Quats;

	// Normals at the simulation mesh particles
	//TArray<FVector> Normals;

	// Transform applied per position/normal element when loaded
	//FTransform Transform;

	// Transform relative to the component to update clothing root transform when not ticking clothing but rendering a component
	//FTransform ComponentRelativeTransform;
};


inline std::vector<std::vector<float>> EigenMatrix2StdVector(const SparseMatrix& Matrix)
{
	std::vector<std::vector<float>>debug1(Matrix.rows(),std::vector<float>(Matrix.cols(),0));
	for(int i=0;i<Matrix.rows();i++)
	{
		for( int j=0;j<Matrix.cols();j++)
		{
			debug1[i][j] = Matrix.coeff(i,j);
		}
	}
	return debug1;
}
inline std::vector<std::vector<float>> EigenMatrix2StdVector(const Vec& Matrix)
{
	std::vector<std::vector<float>>debug1(Matrix.rows(),std::vector<float>(Matrix.cols(),0));
	for(int i=0;i<Matrix.rows();i++)
	{
		for( int j=0;j<Matrix.cols();j++)
		{
			debug1[i][j] = Matrix.coeff(i,j);
		}
	}
	return debug1;
}
inline void NormalizeQuatVec( Vec& Su)
{
	for(int i=0;i<Su.rows();i+=4)
	{
		
		PDScalar s = 1/sqrt(Su[i]*Su[i]+Su[i+1]*Su[i+1]+Su[i+2]*Su[i+2]+Su[i+3]*Su[i+3]);
		Su[i] *=s;
		Su[i+1] *=s;
		Su[i+1] *=s;
		Su[i+1] *=s;
	}
	return ;
}

inline PDScalar EulerZFromQuat(const Quat& q)
{
	PDScalar t3 = 2*(q.w()*q.z()+q.x()*q.y());
	PDScalar t4 = 1-2*(q.y()*q.y()+q.z()*q.z());
	PDScalar EulerZ = atan2f(t3,t4);
	return EulerZ;
}

inline FQuat FQuatFromQuat(const Quat& q)
{
	return FQuat(q.x(),q.y(),q.z(),q.w());
}