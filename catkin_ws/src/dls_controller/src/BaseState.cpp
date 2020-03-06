#include <dls_controller/BaseState.h>

BaseState::BaseState() :
		utime_(0.), position_W_(Eigen::Vector3d::Zero()), velocity_B_(
				Eigen::Vector3d::Zero()), acceleration_B_(
				Eigen::Vector3d::Zero()), orientation_W_(
				Eigen::Quaterniond::Identity()), rot_rate_B_(
				Eigen::Vector3d::Zero()), rot_acceleration_B_(
				Eigen::Vector3d::Zero()), rpy_W_(Eigen::Vector3d::Zero()), unwrapped_rpy_W_(
				Eigen::Vector3d::Zero())
{

}


BaseState::~BaseState()
{

}


const Eigen::Vector3d& BaseState::getPosition_W() const
{
	return position_W_;
}


const Eigen::Vector3d BaseState::getVelocity_W() const
{
	return Eigen::Affine3d(orientation_W_) * velocity_B_;
}


const Eigen::Vector3d& BaseState::getVelocity_B() const
{
	return velocity_B_;
}


const uint64_t &BaseState::getMicroTime() const
{
	return utime_;
}


const Eigen::Quaterniond BaseState::getOrientation_H() const
{
	return Eigen::Quaterniond(getRotBaseToHF());
}


const Eigen::Vector3d BaseState::getVelocity_H() const
{
	// put back the velocity in the base frame into the horizontal frame
	return getRotBaseToHF() * velocity_B_;
}


const Eigen::Vector3d& BaseState::getAcceleration_B() const
{
	return acceleration_B_;
}


const Eigen::Vector3d& BaseState::getRotAcceleration_B() const
{
	return rot_acceleration_B_;
}


void BaseState::setPosition_W(const Eigen::Vector3d& pos)
{
	position_W_ = pos;
}


void BaseState::setPosition_W(const double& x, const double& y, const double& z)
{
	position_W_ << x, y, z;
}


void BaseState::setVelocity_B(const Eigen::Vector3d& vel)
{
	velocity_B_ = vel;
}


void BaseState::setVelocity_B(const double& x, const double& y,	const double& z)
{
	velocity_B_ << x, y, z;
}


void BaseState::setAcceleration_W(const Eigen::Vector3d &acc)
{
	acceleration_B_ = (Eigen::Affine3d(orientation_W_)).inverse() * acc;
}


void BaseState::setAcceleration_W(const double &x, const double &y,	const double &z)
{
	acceleration_B_ << x, y, z;
	acceleration_B_ =
			(Eigen::Affine3d(orientation_W_)).inverse()	* acceleration_B_;
}


void BaseState::setAcceleration_I(const Eigen::Vector3d &acc,
								  const Eigen::Affine3d &bTi)
{
	acceleration_B_ = bTi * acc;
}


void BaseState::setRotationRate_W(const Eigen::Vector3d &rate)
{
	rot_rate_B_ = (Eigen::Affine3d(orientation_W_)).inverse() * rate;
}


void BaseState::setRotAcceleration_W(const Eigen::Vector3d &rotacc)
{
	rot_acceleration_B_ = (Eigen::Affine3d(orientation_W_)).inverse() * rotacc;
}


void BaseState::setAcceleration_I(const double &x,
								  const double &y,
								  const double &z,
								  const Eigen::Affine3d &bTi)
{
	acceleration_B_ << x, y, z;
	acceleration_B_ = bTi * acceleration_B_;
}


void BaseState::setVelocity_W(const Eigen::Vector3d& vel)
{
	velocity_B_ = (Eigen::Affine3d(orientation_W_)).inverse() * vel;
}


void BaseState::setVelocity_W(const double& x,
							  const double& y,
							  const double& z)
{
	velocity_B_ << x, y, z;
	velocity_B_ = (Eigen::Affine3d(orientation_W_)).inverse() * velocity_B_;
}


void BaseState::setVelocity_H(const Eigen::Vector3d &vel)
{
	velocity_B_ = getRotBaseToHF().transpose() * vel;
}


void BaseState::setVelocity_H(const double &x,
							  const double &y,
							  const double &z)
{
	setVelocity_H(Eigen::Vector3d(x, y, z));
}


void BaseState::setAcceleration_B(const Eigen::Vector3d& acc)
{
	acceleration_B_ = acc;
}


void BaseState::setAcceleration_B(const double& x,
								  const double& y,
								  const double& z)
{
	acceleration_B_ << x, y, z;
}


const Eigen::Vector3d& BaseState::getRPY_W() const
{
	return rpy_W_;
}


const Eigen::Vector3d& BaseState::getUnwrappedRPY_W() const
{
	return unwrapped_rpy_W_;
}


double BaseState::getUwrappedRoll_W() const
{
	return unwrapped_rpy_W_(0);
}


double BaseState::getUwrappedPitch_W() const
{
	return unwrapped_rpy_W_(1);
}


double BaseState::getUwrappedYaw_W() const
{
	return unwrapped_rpy_W_(2);
}


const Eigen::Vector3d& BaseState::getRotationRate_B() const
{
	return rot_rate_B_;
}


const Eigen::Quaterniond& BaseState::getOrientation_W() const
{
	return orientation_W_;
}


double BaseState::getRoll_W() const
{
	return rpy_W_(0);
}


double BaseState::getPitch_W() const
{
	return rpy_W_(1);
}


double BaseState::getYaw_W() const
{
	return rpy_W_(2);
}


void BaseState::setMicroTime(const uint64_t &utime)
{
	this->utime_ = utime;
}


void BaseState::setRotationRate_B(const Eigen::Vector3d &angRate)
{
	rot_rate_B_ = angRate;
}


void BaseState::setRotationRate_B(const double& x, const double& y,	const double& z)
{
	rot_rate_B_ << x, y, z;
}


void BaseState::setRotAcceleration_B(const Eigen::Vector3d &rate)
{
	rot_acceleration_B_ = rate;
}


void BaseState::setRotAcceleration_B(const double &x, const double &y, const double &z)
{
	rot_acceleration_B_ << x, y, z;
}


void BaseState::setOrientation_W(const Eigen::Quaterniond& orient)
{
	orientation_W_ = orient;
	double q0 = orient.w();
	double q1 = orient.x();
	double q2 = orient.y();
	double q3 = orient.z();

	// Euler angles (convention RPY)
	Eigen::Vector3d rpy_W = Eigen::Vector3d::Zero();
	// rpy_W is the new local wrapped state
	// rpy_W_ is my private old wrapped state
	rpy_W << atan2(2.0 * (q0 * q1 + q2 * q3), 1.0 - 2.0 * (q1 * q1 + q2 * q2)), asin(
			2.0 * (q0 * q2 - q3 * q1)), atan2(2.0 * (q0 * q3 + q1 * q2),
			1.0 - 2.0 * (q2 * q2 + q3 * q3));

	// Unwrap the angle
	for (int i = 0; i < 3; i++) {
		// rpy_W(i) - rpy_W_(i) is our delta (new state - old state)
		if (rpy_W(i) - rpy_W_(i) < -M_PI) {
			unwrapped_rpy_W_(i) += rpy_W(i) - rpy_W_(i) + 2 * M_PI;
		} else if (rpy_W(i) - rpy_W_(i) > M_PI) {
			unwrapped_rpy_W_(i) += rpy_W(i) - rpy_W_(i) - 2 * M_PI;
		} else {
			unwrapped_rpy_W_(i) += rpy_W(i) - rpy_W_(i);
		}
	}
	// update the old private state into current state
	rpy_W_ = rpy_W;
}


void BaseState::resetState()
{
	position_W_ = Eigen::Vector3d::Zero();
	acceleration_B_ = Eigen::Vector3d::Zero();
	rpy_W_ = Eigen::Vector3d::Zero();
	unwrapped_rpy_W_ = Eigen::Vector3d::Zero();
	velocity_B_ = Eigen::Vector3d::Zero();
	rot_rate_B_ = Eigen::Vector3d::Zero();
	rot_acceleration_B_ = Eigen::Vector3d::Zero();
	orientation_W_ = Eigen::Quaterniond::Identity();
}

void BaseState::setRawRotationRate(const Eigen::Vector3d &rate, uint8_t id) {
    if (id==0 || id == 1)
        raw_rot_rate_[id] = rate;
}

void BaseState::setRawAcceleration(const Eigen::Vector3d &acc, uint8_t id) {
    if (id==0 || id == 1)
        raw_acceleration_[id] = acc;
}

void BaseState::setRawQ(const Eigen::Quaterniond &q, uint8_t id) {
    if (id==0 || id == 1)
        raw_q_[id] = q;
}

const Eigen::Vector3d& BaseState::getRawAcceleration(uint8_t id) const {
    if (id==0 || id == 1)
        return raw_acceleration_[id];
    return Eigen::Vector3d::Zero();
}

const Eigen::Vector3d& BaseState::getRawRotationRate(uint8_t id) const {
    if (id==0 || id == 1)
        return raw_rot_rate_[id];
    return Eigen::Vector3d::Zero();
}

const Eigen::Quaterniond& BaseState::getRawQ(uint8_t id) const {
    if (id==0 || id == 1)
        return raw_q_[id];
    return Eigen::Quaterniond::Identity();
}

