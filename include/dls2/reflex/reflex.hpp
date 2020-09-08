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
*******************************************************************************/
#ifndef REFLEX_HPP_ATZLXOSD
#define REFLEX_HPP_ATZLXOSD

#include <Eigen/Dense>

namespace dls
{
	/// Class representing a reflex reaction of the robot
	///
	/// Derived classes implement some kind of behaviour of the robot that
	/// should only occur under certain circumstances. For instance, a derived
	/// class may implement a reflex where the robot raises its leg higher to
	/// avoid an obstacle.
	///
	/// Derived classes should have no knowledge of when to apply this reflex.
	/// This must be done by the gait generator or controller that wishes to add
	/// this reflex. The reflex is informed that it needs to run its
	/// calculations by means of a predicate function that is passed into its
	/// constructor. The function can be decided by whoever adds this reflex.
	/// If the predicate returns `true`, the reflex runs its computations. Else,
	/// the reflex returns a vector of zeroes.
	class Reflex
	{
		/// Constructor
		///
		/// @param njoints the number of active joints
		/// @param predicate a predicate function that determines whether this
		/// reflex is run. The predicate returns `true` to indicate that it
		/// should run
		Reflex(size_t njoints, bool(*predicate)());

		virtual ~Reflex() = default;

		/// Calculates the reflex
		///
		/// Only calculates if the predicate is true. Returns zeroes otherwise
		Eigen::VectorXd run();

	protected:
		/// Pure Virtual callculation function
		///
		/// This function is called only when the predicate returns `true`. It
		/// should compute the aditional torques that the reflex requires. The
		/// size of the torques should be equal to the size of the robot active
		/// joint space
		virtual Eigen::VectorXd calculateTorques() = 0;

	private:
		/// Number of joints of the robot
		///
		size_t njoints;

		/// Predicate function
		///
		/// Returns `true` if calculations should be run, else `false`
		bool (*predicate)();

		/// Zero vector
		///
		/// Stored here so as to avoid reconstructing it on each iteration of
		/// the epoch
		Eigen::VectorXd zeroes;
	};
} // end namespace dls
#endif /* end of include guard: REFLEX_HPP_ATZLXOSD */
