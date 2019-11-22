/*******************************************************************************
*                                                       ,----,                 *
*                                                     .'   .' \                *
*                                                   ,----,'    |               *
*               ________  ___       ________        |    :  .  ;               *
*              |\   ___ \|\  \     |\   ____\       ;    |.'  /                *
*              \ \  \_|\ \ \  \    \ \  \___|_      `----'/  ;                 *
*               \ \  \ \\ \ \  \    \ \_____  \       /  ;  /                  *
*                \ \  \_\\ \ \  \____\|____|\  \     ;  /  /-,                 *
*                 \ \_______\ \_______\____\_\  \   /  /  /.`|                 *
*                  \|_______|\|_______|\_________\./__;      :                 *
*                                     \|_________||   :    .'                  *
*                                                 ;   | .'                     *
*                                                 `---'                        *
********************************************************************************
* Author:            Hendrik de Bruin                                          *
* Maintainer:        Hendrik de Bruin                                          *
* author email:      hendrik.debruin@iit.it                                    *
*******************************************************************************/
#ifndef POSE_H_OKAKMWHG
#define POSE_H_OKAKMWHG

// =============================================================================
// Includes
// =============================================================================
#include "todo.h"

#include <Eigen/Dense>
#include <mutex>

// fastrtps
#include "msg/pose.h"

TODO("Write test cases to check the math in the Pose class")

namespace dls
{
// =============================================================================
// Class Interface
// =============================================================================
/// Pose class
///
/// This is a thread-safe class that encapsulates a pose. A pose is stored as a
/// vector representing a translation from the origin, as well as a quaternion
/// representing a rotation about the origin
class Pose
{
public:
	/// Typedef for transformation matrices
	///
	using transformation_matrix_t = Eigen::Matrix<double, 4, 4>;

	// ============================= Constructors ==============================
	/// Default constructor
	///
	/// Creates a pose with zero rotation at the origin
	Pose();

	/// Creates a pose at the given point with zero rotation
	///
	Pose(const Eigen::Vector3d&);

	/// Creates a pose at the origin with the given quaternion as orientation
	///
	Pose(const Eigen::Quaterniond&);

	/// Creates a pose at the origin with the orientation as specified by the
	/// given angle axis
	Pose(const Eigen::AngleAxisd&);

	/// Creates a pose at the given point and orientation
	///
	Pose(const Eigen::Vector3d&, const Eigen::Quaterniond&);

	/// Creates a pose at the given point and orientation
	///
	Pose(const Eigen::Vector3d&, const Eigen::AngleAxisd&);

	/// Creates a pose from a transformation matrix
	///
	Pose(const transformation_matrix_t);

	/// Copy Constructor
	///
	Pose(const Pose&);

	// ============================= FastRTPS util =============================
	/// Converting constructor
	///
	TODO("read up if this should be reference or const or whatever")
	Pose(PoseMsg);

	/// Conversion to PoseMsg
	///
	TODO("this has not been tested yet")
	operator PoseMsg() const;

	// ============================== Conversions ==============================
	/// Returns the position of the pose
	///
	/// This returns a copy of the position. This call is thread-safe, but makes
	/// no guarantees that another thread will not change the original pose.
	Eigen::Vector3d toPosition() const;

	/// Returns a quaternion representing the orientation of the pose
	///
	/// This returns a copy of the orientation. This call is thread-safe, but
	/// makes no guarantees that another thread will not change the original
	/// pose.
	Eigen::Quaterniond toQuaternion() const;

	/// Returns an angle-axis representation of the orientation of the pose
	///
	/// This returns a copy of the orientation. This call is thread-safe, but
	/// makes no guarantees that another thread will not change the original
	/// pose.
	Eigen::AngleAxisd toAngleAxis() const;

	/// Returns a vector of Roll Pith Yaw angles (in ZYX) convention representing the orientation of the pose
	///
	/// This returns a copy of the position. This call is thread-safe, but makes
	/// no guarantees that another thread will not change the original pose.
	Eigen::Vector3d toRpy() const;

	/// Returns an Matrix representation of the orientation of the pose.
	/// The matrix maps vectors from the fixed frame to the rotated frame.
	///
	/// This returns a copy of the orientation. This call is thread-safe, but
	/// makes no guarantees that another thread will not change the original
	/// pose.
	Eigen::Matrix3d toRotationMatrix() const;

	/// Returns a transformation matrix representation of the current pose
	///
	/// This returns a copy of the position. This call is thread-safe, but makes
	/// no guarantees that another thread will not change the original pose.
	transformation_matrix_t toTransformationMatrix() const;

	// ============================== Arithmetic ===============================
	/// Assignment overload
	///
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
	/// Set the position of this pose, leaving the orientation is left untouched
	///
	/// @param [in] vec The position
	void set(const Eigen::Vector3d &vec);

	/// Set the orientation of this pose, leaving the position untouched
	///
	/// @param [in] q The quaternion representing the orientation
	void set(const Eigen::Quaterniond &q);

	/// Set the orientation of this pose, leaving the position untouched
	///
	/// @param [in] aa The angle-axis representing the orientation
	void set(const Eigen::AngleAxisd &aa);

	/// Set both the orientation and position of this pose
	///
	/// @param [in] vec The position
	/// @param [in] q The quaternion representing the orientation
	void set(const Eigen::Vector3d &vec, const Eigen::Quaterniond &q);

	/// Set both the orientation and position of this pose
	///
	/// @param [in] vec The position
	/// @param [in] a The angle-axis representing the orientation
	void set(const Eigen::Vector3d &vec, const Eigen::AngleAxisd &aa);

private:
	// BEGIN critical section
		/// Mutex protecting position and quaternion
		///
		mutable std::mutex pose_mutex;

		/// Position
		///
		Eigen::Vector3d position;

		/// Orientation
		///
		Eigen::Quaternion<double> quaternion;
	// END critical section
};
} // end namespace dls

#endif /* end of include guard: POSE_H_OKAKMWHG */
