/*
 * computeJacobians.h
 *
 *  Created on: Apr 9, 2014
 *      Author: mfocchi
 */

#ifndef IIT_DOG_COMPUTE_JACOBIANS_H_
#define IIT_DOG_COMPUTE_JACOBIANS_H_

#include <Eigen/Dense>
#include <iit/commons/dog/transforms.h>
#include <iit/commons/dog/declarations.h>
#include <iit/commons/dog/leg_data_map.h>
#include <iit/commons/dog/feet_jacobians.h>
#include <iit/commons/dog/forward_kinematics.h>
#include <iit/commons/dog/inertia_properties.h>
#include <iit/commons/dog/inverse_dynamics.h>

namespace iit {
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
                  const iit::rbd::VelocityVector &  baseVel,
                  const iit::rbd::VelocityVector &  gB,
                  const Eigen::Matrix3d & R,
                  const dog::JointState & q,
                  const dog::JointState & qd,
                  dog::InverseDynamicsBase& id,
                  dog::MotionTransformsBase& mt,
                  dog::ForwardKinematics &fwd_kin);


iit::rbd::Vector6D revoluteGJac(const iit::rbd::Vector3d & p,
                                const iit::rbd::Vector3d & origin,
                                const iit::rbd::Vector3d & axis);

Eigen::Matrix<double, 6, dog::jointsCount> getWholeBodyCOMJacobian(const dog::JointState& q,
                                                                   const dog::InertiaPropertiesBase &in,
                                                                   dog::HomogeneousTransformsBase& ht);

Eigen::Matrix<double, 6,  dog::jointsCount + 6> getWholeBodyCOMJacobianFB(const Eigen::Matrix3d & rotationMx,
        const dog::JointState& q,
        InertiaPropertiesBase &in,
        dog::HomogeneousTransformsBase& ht);


iit::rbd::Vector6D getWholeBodyCOMVel(const dog::JointState & q,
                                      const dog::JointState & qd,
                                      const dog::InertiaPropertiesBase &inertiaProps,
                                      dog::HomogeneousTransformsBase& ht);


iit::rbd::Vector6D getWholeBodyCOMVelFB(const iit::rbd::VelocityVector & baseVel,
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
iit::rbd::Matrix66d motionVectorTransform(const iit::rbd::Vector3d & position,
                                          const Eigen::Matrix3d & rotationMx);

/**
 * @brief forceVectorTransform
 * @param position
 * @param rotationMx
 * @return
 * @note this is the transposed of motionVectorTransform()
 * @sa motionVectorTransform()
 */
iit::rbd::Matrix66d forceVectorTransform(const iit::rbd::Vector3d & position,
                                         const Eigen::Matrix3d & rotationMx);

void computeBaseTwist(const dog::LegDataMap<Eigen::Vector3d> & feet,
                      const dog::LegDataMap<Eigen::Vector3d> & feet_vel,
                      const Eigen::Vector3d omega,
                      const dog::LegDataMap<bool> & stance_legs,
                      iit::rbd::VelocityVector & baseVel);

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
