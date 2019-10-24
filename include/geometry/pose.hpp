#ifndef POSE_H_OKAKMWHG
#define POSE_H_OKAKMWHG

#include <Eigen/Dense>
#include <mutex>

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
	// Subtraction
	Pose &operator-=(const Pose&);
	Pose &operator-=(const Eigen::Vector3d&);
	Pose &operator-=(const Eigen::Quaterniond&);

	Pose operator-(const Pose&) const;
	Pose operator-(const Eigen::Vector3d&) const;
	Pose operator-(const Eigen::Quaterniond&) const;

	// Addition
	// Pose &operator+=(const Pose&);
	// Pose &operator+=(const Eigen::Vector3d&);
	// Pose &operator+=(const Eigen::Quaterniond&);

	// Pose operator+(const Pose&) const;
	// Pose operator+(const Eigen::Vector3d&) const;
	// Pose operator+(const Eigen::Quaterniond&) const;

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
