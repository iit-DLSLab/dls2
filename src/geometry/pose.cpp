#include "geometry/pose.hpp"

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
// FastRTPS Util
// =============================================================================
Pose::Pose(PoseMsg msg) :
	pose_mutex(),
	position(msg.position().data()),
	quaternion(msg.quaternion().data())
{ }

Pose::operator PoseMsg() const
{
	PoseMsg p;

	TODO("DO NOT use hardcoded paths like this. Figure out how to access underlying data and send straight into std::array")
	std::lock_guard<std::mutex> lock(this->pose_mutex);
	p.position(std::array<double, 3>{this->position(0), this->position(1), this->position(2)});
	p.quaternion(std::array<double, 4>{this->quaternion.x(), this->quaternion.y(), this->quaternion.z(), this->quaternion.z()});

	return p;
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

Pose::transformation_matrix_t Pose::toTransformationMatrix() const
{
	Eigen::Matrix4d T;
	{
		std::lock_guard<std::mutex> lock(this->pose_mutex);
		T.block<3,3>(0,0) = this->quaternion.toRotationMatrix();
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
