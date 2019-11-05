#ifndef POSE_H_OKAKMWHG
#define POSE_H_OKAKMWHG

#include "todo.h"

#include <Eigen/Dense>
#include <mutex>

// fastrtps
#include "msg/pose.h"

TODO("Write test cases to check the math in the Pose class")
class Pose
{
public:
	using transformation_matrix_t = Eigen::Matrix<double, 4, 4>;

	// ============================= Constructors ==============================
	/// Default constructor
	///
	/// Creates a pose with zero rotation at the origin
	Pose();

	/// Creates a pose at the given point with zero rotation
	Pose(const Eigen::Vector3d&);

	/// Creates a pose at the origin with the given quaternion as orientation
	Pose(const Eigen::Quaterniond&);

	/// Creates a pose at the origin with the orientation as specified by the
	/// given angle axis
	Pose(const Eigen::AngleAxisd&);

	/// Creates a pose at the given point and orientation
	Pose(const Eigen::Vector3d&, const Eigen::Quaterniond&);

	/// Creates a pose at the given point and orientation
	Pose(const Eigen::Vector3d&, const Eigen::AngleAxisd&);

	/// Creates a pose from a transformation matrix
	Pose(const transformation_matrix_t);

	/// Copy Constructor
	Pose(const Pose&);

	// ============================= FastRTPS util =============================
	/// Converting constructor
	TODO("read up if this should be reference or const or whatever")
	Pose(PoseMsg);

	/// Conversion to PoseMsg
	TODO("this has not been tested yet")
	operator PoseMsg() const;

	// ============================== Conversions ==============================
	/// Returns the position of the pose
	Eigen::Vector3d toPosition() const;

	/// Returns a quaternion representing the orientation of the pose
	Eigen::Quaterniond toQuaternion() const;

	/// Returns an angle-axis representation of the orientation of the pose
	Eigen::AngleAxisd toAngleAxis() const;

	/// Returns a transformation matrix representation of the current pose
	transformation_matrix_t toTransformationMatrix() const;

	// ============================== Arithmetic ===============================
	/// Assignement overload
	Pose &operator=(const Pose &rhs);

	/// Computes a pose that represents the translation and rotation difference
	/// from a given pose to this pose
	Pose &operator-=(const Pose&);

	/// Computes the translation from the given position to this pose. Ignores
	/// rotations
	Pose &operator-=(const Eigen::Vector3d&);

	/// Computes the rotation from a given pose to this pose. Ignores
	/// translations
	Pose &operator-=(const Eigen::Quaterniond&);

	/// Computes a pose that represents the translation and rotation difference
	/// from a given pose to this pose
	Pose operator-(const Pose&) const;

	/// Computes the translation from the given position to this pose. Ignores
	/// rotations
	Pose operator-(const Eigen::Vector3d&) const;

	/// Computes the rotation from a given pose to this pose. Ignores
	/// translations
	Pose operator-(const Eigen::Quaterniond&) const;

	// ================================ setters ================================
	void set(const Eigen::Vector3d&);
	void set(const Eigen::Quaterniond&);
	void set(const Eigen::AngleAxisd&);
	void set(const Eigen::Vector3d&, const Eigen::Quaterniond&);
	void set(const Eigen::Vector3d&, const Eigen::AngleAxisd&);

private:
	// BEGIN critical section
		mutable std::mutex pose_mutex;
		Eigen::Vector3d position;
		Eigen::Quaternion<double> quaternion;
	// END critical section
};


#endif /* end of include guard: POSE_H_OKAKMWHG */
