/*
 * computeJacobians.h
 *
 *  Created on: Apr 9, 2014
 *      Author: mfocchi
 */

#ifndef IIT_DOG_COMPUTE_JACOBIANS_H_
#define IIT_DOG_COMPUTE_JACOBIANS_H_

#include <Eigen/Dense>
#include <doglib/base/transforms.hpp>
#include <doglib/base/declarations.hpp>
#include <doglib/base/leg_data_map.hpp>
#include <doglib/base/feet_jacobians.hpp>
#include <doglib/base/forward_kinematics.hpp>
#include <doglib/base/inertia_properties.hpp>
#include <doglib/base/inverse_dynamics.hpp>

namespace dls {
namespace dog {

void computeJcb(Eigen::Matrix<double, Eigen::Dynamic, 6> & Jcb,
                const Eigen::Matrix3d R,
                const dog::LegDataMap<bool> & stance_legs,
                const dog::LegDataMap<Eigen::Vector3d> & feet);

void computeJcq(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount> & Jcq,
                const Eigen::Matrix3d R,
                const dog::LegDataMap<bool> & stance_legs,
                const dog::LegDataMap<dog::FootJac> &jacs);

void computeStanceJacobian(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> & Jc,
                           const Eigen::Matrix3d R,
                           const dog::LegDataMap<bool> & stance_legs,
                           const dog::LegDataMap<Eigen::Vector3d> & feet,
                           const dog::LegDataMap<dog::FootJac> &jacs);

void computeStanceJacobianVirtual(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> & Jc,
                                  const Eigen::Matrix3d R,
                                  const dog::LegDataMap<bool> & stance_legs,
                                  const dog::LegDataMap<Eigen::Vector3d> & feet,
                                  const dog::LegDataMap<dog::FootJac> &jacs);

void computeStanceJacobian(Eigen::Matrix<double, Eigen::Dynamic, dog::jointsCount+6> & Jc,
                           Eigen::Matrix<double, dog::jointsCount+6, dog::jointsCount+6> & Nc,
                           const Eigen::Matrix3d R,
                           const dog::LegDataMap<bool> & stance_legs,
                           const dog::LegDataMap<Eigen::Vector3d> & feet,
                           const dog::LegDataMap<dog::FootJac> &jacs);

void computeJcdQd(Eigen::Matrix<double, Eigen::Dynamic, 1> & JcdQd,
                  const dog::LegDataMap<bool> & stance_legs,
                  const dls::rbd::VelocityVector &  baseVel,
                  const dls::rbd::VelocityVector &  gB,
                  const Eigen::Matrix3d & R,
                  const dog::JointState & q,
                  const dog::JointState & qd,
                  dog::InverseDynamicsBase& id,
                  dog::MotionTransformsBase& mt,
                  dog::ForwardKinematicsBase &fwd_kin);


dls::rbd::Vector6D revoluteGJac(const dls::rbd::Vector3d & p,
                                const dls::rbd::Vector3d & origin,
                                const dls::rbd::Vector3d & axis);

Eigen::Matrix<double, 6, dog::jointsCount> getWholeBodyCOMJacobian(const dog::JointState& q,
                                                                   const dog::InertiaPropertiesBase &in,
                                                                   dog::HomogeneousTransformsBase& ht);

Eigen::Matrix<double, 6,  dog::jointsCount + 6> getWholeBodyCOMJacobianFB(const Eigen::Matrix3d & rotationMx,
        const dog::JointState& q,
        InertiaPropertiesBase &in,
        dog::HomogeneousTransformsBase& ht);


dls::rbd::Vector6D getWholeBodyCOMVel(const dog::JointState & q,
                                      const dog::JointState & qd,
                                      const dog::InertiaPropertiesBase &inertiaProps,
                                      dog::HomogeneousTransformsBase& ht);


dls::rbd::Vector6D getWholeBodyCOMVelFB(const dls::rbd::VelocityVector & baseVel,
                                        const Eigen::Matrix3d & rotationMx,
                                        const dog::JointState & q,
                                        const dog::JointState & qd,
                                        InertiaPropertiesBase &inertiaProps,
                                        dog::HomogeneousTransformsBase& ht);

/**
 * @brief motionVectorTransform
 * compose a Plucker coordinate transform X from its component parts rotationMx
 * and position, and decompose it into those parts, respectively.
 * X is a coordinate transform corresponding to a shift of origin by an amount
 * specified by position, followed by a rotation about the new origin as
 * specified by rotationMx.  For example, plux(rx(1),[2 3 4]) makes
 * the same transform as rotx(1)*xlt([2 3 4]). if b_X_a then rotationMx is from
 * A to B b_X_a : position is the vector from A to B rotation the R that maps
 * vectors in A into vectors in B
 * @param position is a 3D vector
 * @param rotationMx is a 3x3 rotational coordinate transform
 * @return
 */
dls::rbd::Matrix66d motionVectorTransform(const dls::rbd::Vector3d & position,
                                          const Eigen::Matrix3d & rotationMx);

/**
 * @brief forceVectorTransform
 * @param position
 * @param rotationMx
 * @return
 * @note this is the transposed of motionVectorTransform()
 * @sa motionVectorTransform()
 */
dls::rbd::Matrix66d forceVectorTransform(const dls::rbd::Vector3d & position,
                                         const Eigen::Matrix3d & rotationMx);

void computeBaseTwist(const dog::LegDataMap<Eigen::Vector3d> & feet,
                      const dog::LegDataMap<Eigen::Vector3d> & feet_vel,
                      const Eigen::Vector3d omega,
                      const dog::LegDataMap<bool> & stance_legs,
                      dls::rbd::VelocityVector & baseVel);

int compute_stance_legs(const dog::LegDataMap<bool> & stance_legs);

Eigen::Vector3d getCoMFromBase(const dog::JointState & q,
                               const Eigen::Vector3d & base_orient,
                               const Eigen::Vector3d & base_pos,
                               InertiaPropertiesBase &in);

Eigen::Vector3d getBaseFromCoM(const dog::JointState & q,
                               const Eigen::Vector3d & base_orient,
                               const Eigen::Vector3d & CoM,
                               InertiaPropertiesBase &in);

} // namespace dog
} // namespace iit

#endif /* COMPUTEJACOBIANS_H_ */
