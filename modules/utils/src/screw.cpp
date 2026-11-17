
#include "dls2/util/screw.hpp"

namespace dls
{
	namespace utils{
	// =============================================================================
	// Constructors
	// =============================================================================
	Screw::Screw():
		eigen_vec(Eigen::Matrix<double,6,1>::Zero())
	{ }

	Screw::Screw(const Eigen::Vector3d& lin, const Eigen::Vector3d& ang)
	{
		eigen_vec = Eigen::Matrix<double,6,1>::Zero();
		this->setLinear(lin);
		this->setLinear(ang);
	}

	// =============================================================================
	// Implementation
	// =============================================================================
	void Screw::setLinear(const Eigen::Vector3d &vec)
	{
		this->eigen_vec.block<3, 1>(0, 0) = vec;
	}

	void Screw::setAngular(const Eigen::Vector3d &vec)
	{
		this->eigen_vec.block<3, 1>(3, 0) = vec;
	}

	Eigen::Matrix<double, 6, 1> &Screw::data()
	{
		return this->eigen_vec;
	}

	Eigen::Vector3d Screw::getLinear() const
	{
		return this->eigen_vec.block<3, 1>(0, 0);
	}

	Eigen::Vector3d Screw::getAngular() const
	{
		return this->eigen_vec.block<3, 1>(3, 0);
	}

	double& Screw::operator[](int index)
	{
		return this->eigen_vec(index);
	}

	const double& Screw::operator[](int index) const
	{
		return this->eigen_vec(index);
	}

	void Screw::set(const std::array<double,3>& linear, const std::array<double,3>& angular)
	{
		this->eigen_vec(0) = linear[0];
		this->eigen_vec(1) = linear[1];
		this->eigen_vec(2) = linear[2];
		this->eigen_vec(3) = angular[0];
		this->eigen_vec(4) = angular[1];
		this->eigen_vec(5) = angular[2];
	}
	}
}