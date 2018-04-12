#ifndef __VPC_DART_HELPER_H__
#define __VPC_DART_HELPER_H__
#include "dart/dart.hpp"
#include "StdIncludes.h"
namespace VPC {
namespace DartHelper {

dart::dynamics::BodyNode* MakeFreeJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& body_pos,
	double mass);


dart::dynamics::BodyNode* MakeBallJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass);

dart::dynamics::BodyNode* MakePrismaticJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass);

dart::dynamics::BodyNode* MakeRevoluteJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass);

dart::dynamics::BodyNode* MakeWeldJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass);

}
}

#endif