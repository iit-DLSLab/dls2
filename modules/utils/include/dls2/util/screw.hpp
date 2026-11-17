
#ifndef SCREW_HPP_O9FQ1FIE
#define SCREW_HPP_O9FQ1FIE

#include <Eigen/Dense>

/// Wrapper class for screw messages
///
/// A screw is encapsulated as a 6x1 vector. The first 3 components are linear,
/// the final 3 components are angular

namespace dls
{
	namespace utils{
	class Screw
	{
	public:
		/// Default constructor
		///
		/// Assigns memory, but does not zero it
		Screw();

		Screw(const Eigen::Vector3d&, const Eigen::Vector3d&);

		/// Sets the linear component of the screw
		///
		void setLinear(const Eigen::Vector3d&);

		/// Sets the angular component of the screw
		///
		void setAngular(const Eigen::Vector3d&);

		/// Returns the linear component of the screw
		///
		Eigen::Vector3d getLinear() const;

		/// Returns the angular component of the screw
		///
		Eigen::Vector3d getAngular() const;

		/// Returns a reference to the internal representation of the screw
		///
		/// Note that this is a returned reference. Do not use this reference after
		/// the Screw instance which owns it has gone out of scope
		Eigen::Matrix<double, 6, 1> &data();

		/// Returns a value by index
		double& operator[](int index);

		/// Returns a value by index for const object
		const double& operator[](int index) const;

		/// Set linear and angular velocity
		/// @param[in] linear linear velocity
		/// @param[in] angular angular velocity
		void set(const std::array<double,3>& linear, const std::array<double,3>& angular);

	private:

		/// Data storage
		///
		Eigen::Matrix<double, 6, 1> eigen_vec;
	};
	}
} // end namespace dls
	#endif /* end of include guard: SCREW_HPP_O9FQ1FIE */