
#include "dls2/util/pose.hpp"
#include "dls2/math/rotations.hpp" 
#include <algorithm>

namespace dls
{
	namespace utils{
	// =============================================================================
	// Constructors
	// =============================================================================
	Pose::Pose() :
		pose_mutex(),
		position(Eigen::Vector3d::Zero()),
		quaternion(Eigen::Quaternion<double>::Identity())
	{ }

	Pose::Pose(const Eigen::Vector3d &vec) :
		pose_mutex(),
		position(vec),
		quaternion(Eigen::Quaternion<double>::Identity())
	{ }

	Pose::Pose(const Eigen::AngleAxisd &aa) :
		pose_mutex(),
		position(Eigen::Vector3d::Zero()),
		quaternion(aa)
	{ }

	Pose::Pose(const Eigen::Vector3d &vec, const Eigen::Quaterniond &q) :
		pose_mutex(),
		position(vec),
		quaternion(q)
	{ }

	Pose::Pose(const Eigen::Vector3d &vec, const Eigen::AngleAxisd &aa) :
		pose_mutex(),
		position(vec),
		quaternion(aa)
	{ }

	// Copy Constructor
	Pose::Pose(const Pose &rhs) : Pose()
	{
		std::lock_guard<std::mutex> lock(rhs.pose_mutex);
		this->position = rhs.position;
		this->quaternion = rhs.quaternion;
	}

	// =============================================================================
	// Conversions
	// =============================================================================
	Eigen::Vector3d Pose::toPosition() const
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		return this->position;
	}

	Eigen::Quaterniond Pose::toQuaternion() const
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		return this->quaternion;
	}

	Eigen::AngleAxisd Pose::toAngleAxis() const
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		return Eigen::AngleAxisd(this->quaternion);
	}

	Eigen::Vector3d Pose::toRpy() const
	{
	std::lock_guard<std::mutex> lock(this->pose_mutex);
	return dls::math::quatToRPY(this->quaternion);
	}


	Eigen::Matrix3d Pose::toRotationMatrix() const
	{
	std::lock_guard<std::mutex> lock(this->pose_mutex);
	return dls::math::quatToRotMat(this->quaternion);
	}


	Pose::transformation_matrix_t Pose::toTransformationMatrix() const
	{
		Eigen::Matrix4d T;
		{
			std::lock_guard<std::mutex> lock(this->pose_mutex);
			T.block<3,3>(0,0) = dls::math::quatToRotMat(this->quaternion);//this->quaternion.toRotationMatrix();
			T.block<3, 1>(0, 3) = this->position;
			// Release mutex earlier for efficiency of other threads
		}
		T.block<1, 3>(3, 1) = Eigen::Matrix<double, 3, 1>::Zero();
		T(3, 3) = 1;

		return T;
	}

	// =============================================================================
	// Arithmetic
	// =============================================================================
	Pose &Pose::operator=(const Pose &rhs)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		{
			std::lock_guard<std::mutex> lock(rhs.pose_mutex);
			this->position = rhs.position;
			this->quaternion = rhs.quaternion;
		}
		return *this;
	}

	// ================================ Subtraction ================================
	Pose &Pose::operator-=(const Pose &rhs)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		{
			std::lock_guard<std::mutex> lock(rhs.pose_mutex);
			{
				this->position = this->position - rhs.position;
				this->quaternion  = rhs.quaternion * this->quaternion.inverse();
			}
		}
		return *this;
	}

	Pose &Pose::operator-=(const Eigen::Vector3d &rhs)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		{
			this->position = this->position - rhs;
		}
		return *this;
	}

	Pose &Pose::operator-=(const Eigen::Quaterniond &rhs)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		{
			this->quaternion = rhs*this->quaternion.inverse();
		}
		return *this;
	}

	Pose Pose::operator-(const Pose &rhs) const
	{
		Pose p(*this);
		return p -= rhs;
	}

	Pose Pose::operator-(const Eigen::Vector3d &rhs) const
	{
		Pose p(*this);
		return p -= rhs;
	}

	Pose Pose::operator-(const Eigen::Quaterniond &rhs) const
	{
		Pose p(*this);
		return p -= rhs;
	}

	// ================================= Addition ==================================
	// Pose &Pose::operator+=(const Pose &rhs)
	// {
	// 	std::lock_guard<std::mutex> lock(this->pose_mutex);
	// 	{
	// 		std::lock_guard<std::mutex> lock(rhs.pose_mutex);
	// 		{
	// 			this->position = this->position + rhs.position;
	// 			this->quaternion = this->
	// 		}
	// 	}
	// }

	// =============================================================================
	// Setters
	// =============================================================================
	void Pose::set(const Eigen::Vector3d &vec)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		this->position = vec;
	}

	void Pose::set(const Eigen::Quaterniond &q)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		this->quaternion = q.normalized();
	}

	void Pose::set(const Eigen::AngleAxisd &aa)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		this->quaternion = Eigen::Quaterniond(aa).normalized();
	}

	void Pose::set(const Eigen::Vector3d &vec, const Eigen::Quaterniond &q)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		this->position = vec;
		this->quaternion = q.normalized();
	}

	void Pose::set(const Eigen::Vector3d &vec, const Eigen::AngleAxisd &aa)
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		this->position = vec;
		this->quaternion = Eigen::Quaterniond(aa).normalized();
	}

	void Pose::set(const std::array<double,3>& pos, const std::array<double,4>& ori){
		position(0) = pos[0];
		position(1) = pos[1];
		position(2) = pos[2];
		quaternion.x() = ori[0];
		quaternion.y() = ori[1];
		quaternion.z() = ori[2];
		quaternion.w() = ori[3];
		quaternion = quaternion.normalized();
	}

	Eigen::Matrix<double, 7, 1> Pose::toVector() const
	{
		Eigen::Matrix<double, 7, 1> vec;
		{
			std::lock_guard<std::mutex> lock(this->pose_mutex);
			vec.block<3, 1>(0, 0) = this->position;
			vec.block<4, 1>(3, 0) = Eigen::Matrix<double,4,1>(this->quaternion.x(), this->quaternion.y(), this->quaternion.z(), this->quaternion.w());
		}
		return vec;
	}
	}
}
