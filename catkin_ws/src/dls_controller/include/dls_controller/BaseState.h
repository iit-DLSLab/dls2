#ifndef DLS_CONTROLLER__BASE_STATE__H
#define DLS_CONTROLLER__BASE_STATE__H

#include <Eigen/Dense>

/**
 * @brief The BaseState class
 * This class incorporate the full state of the robot base, including:
 * <ul>
 *    <li>A time, in microseconds</li>
 *    <li>Base position, expressed in a fixed-world frame of reference</li>
 *    <li>Base velocity, expressed in the base frame</li>
 *    <li>Base acceleration, expressed in the base frame</li>
 *    <li>Base orientation, expressed in a fixed-world frame of reference</li>
 *    <li>Base rotation rate, expressed in the base frame</li>
 * </ul>
 *
 * The class provides getter and setter methods for all these states.
 * When useful, it provides alternatives to set or get the desired physical
 * quantity in one of the following frames of reference: W (fixed-world frame),
 * B (base frame) and H (horizontal frame). Every getter/setter methods have one
 * of these three letters at the end of the signature, to clearly state what is
 * the reference frame adopted. Also convenient methods to set or get RPY angles
 * are provided. Both Eigen and base types are available for setter methods
 */
class BaseState
{
public:
	BaseState();
	~BaseState();

	/**
	 * @brief getPosition_W
	 * @return Absolute position in a fixed-world frame of reference
	 */
	const Eigen::Vector3d& getPosition_W() const;

	/**
	 * @brief getVelocity_W
	 * @return base velocity expressed in the fixed-world frame
	 */
	const Eigen::Vector3d getVelocity_W() const;

	/**
	 * @brief getVelocity_B
	 * @return base velocity expressed in the base frame
	 */
	const Eigen::Vector3d& getVelocity_B() const;

	/**
	 * @brief getVelocity_H
	 * @return base velocity expressed in the horizontal frame. Horizontal
	 * frame is gravity aligned but not yaw aligned
	 */
	const Eigen::Vector3d getVelocity_H() const;

	const Eigen::Vector3d& getAcceleration_B() const;

	const Eigen::Vector3d& getRPY_W() const;
	const Eigen::Vector3d& getUnwrappedRPY_W() const;
	const Eigen::Vector3d& getRotationRate_B() const;

	const Eigen::Quaterniond& getOrientation_W() const;
	const Eigen::Quaterniond getOrientation_H() const;

	/**
	 * @brief getRoll_W
	 * @return Roll of the robot in the range [-pi,pi]
	 */
    double getRoll_W() const;
    double getPitch_W() const;
    double getYaw_W() const;

    double getUwrappedRoll_W() const;
    double getUwrappedPitch_W() const;
    double getUwrappedYaw_W() const;

	const uint64_t& getMicroTime() const;

	const Eigen::Vector3d& getRotAcceleration_B() const;

	/* setters */
	void setPosition_W(const Eigen::Vector3d& pos);
	void setPosition_W(const double& x, const double& y, const double& z);

	void setVelocity_W(const Eigen::Vector3d& vel);
	void setVelocity_W(const double& x, const double& y, const double& z);

	void setVelocity_B(const Eigen::Vector3d& vel);
	void setVelocity_B(const double& x, const double& y, const double& z);

	void setVelocity_H(const Eigen::Vector3d& vel);
	void setVelocity_H(const double& x, const double& y, const double& z);

	void setAcceleration_W(const Eigen::Vector3d& acc);
	void setAcceleration_W(const double& x, const double& y, const double& z);

	void setAcceleration_I(const Eigen::Vector3d& acc,
			const Eigen::Affine3d& bTi);

	void setAcceleration_I(const double& x, const double& y, const double& z,
			const Eigen::Affine3d& bTi);

	void setAcceleration_B(const Eigen::Vector3d& acc);
	void setAcceleration_B(const double& x, const double& y, const double& z);

	void setRotationRate_W(const Eigen::Vector3d& rate);

	void setRotAcceleration_W(const Eigen::Vector3d& rotacc);

	void setRotationRate_B(const Eigen::Vector3d& rate);
	void setRotationRate_B(const double& x, const double& y, const double& z);

	void setRotAcceleration_B(const Eigen::Vector3d& rate);
	void setRotAcceleration_B(const double& x, const double& y,
			const double& z);

	void setOrientation_W(const Eigen::Quaterniond& orient);

	void resetState();

	void setMicroTime(const uint64_t& utime);

        //Hack for two IMU's
        void setRawRotationRate(const Eigen::Vector3d &rate, uint8_t id);
        void setRawAcceleration(const Eigen::Vector3d &acc, uint8_t id);
        void setRawQ(const Eigen::Quaterniond &q, uint8_t id);
        const Eigen::Vector3d& getRawAcceleration(uint8_t id) const;
        const Eigen::Vector3d& getRawRotationRate(uint8_t id) const;
        const Eigen::Quaterniond& getRawQ(uint8_t id) const;
        Eigen::Vector3d raw_acceleration_[2];
        Eigen::Vector3d raw_rot_rate_[2];
        Eigen::Quaterniond raw_q_[2];

	uint64_t utime_;
	Eigen::Vector3d position_W_;
	Eigen::Vector3d velocity_B_;
	Eigen::Vector3d acceleration_B_;

	Eigen::Quaterniond orientation_W_;
	Eigen::Vector3d rot_rate_B_;
	Eigen::Vector3d rot_acceleration_B_;

	Eigen::Vector3d rpy_W_;
	Eigen::Vector3d unwrapped_rpy_W_;




private:
	Eigen::Matrix3d inline getRotBaseToHF() const {
		Eigen::Matrix3d R;

		R << cos(rpy_W_(1)), sin(rpy_W_(0)) * sin(rpy_W_(1)), cos(rpy_W_(0))
				* sin(rpy_W_(1)), 0.0, cos(rpy_W_(0)), -sin(rpy_W_(0)), -sin(
				rpy_W_(1)), sin(rpy_W_(0)) * cos(rpy_W_(1)), cos(rpy_W_(0))
				* cos(rpy_W_(1));
		return R;
	}
};

#endif
