#include "DartHelper.h"
using namespace dart::dynamics;
using namespace dart::simulation;
namespace VPC{
namespace DartHelper{
dart::dynamics::BodyNode* MakeFreeJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& body_pos,
	double mass)
{
	ShapePtr shape = std::shared_ptr<BoxShape>(new BoxShape(size));

	dart::dynamics::Inertia inertia;
	inertia.setMass(mass);
	inertia.setMoment(shape->computeInertia(mass));

	BodyNode* bn;
	FreeJoint::Properties props;
	props.mName = name + "_joint";
	props.mT_ParentBodyToJoint.setIdentity();
	props.mT_ParentBodyToJoint.translation() = body_pos;

	bn = skel->createJointAndBodyNodePair<FreeJoint>(
		parent,props,BodyNode::AspectProperties(name)).second;
	bn->createShapeNodeWith<VisualAspect,CollisionAspect, DynamicsAspect>(shape);
	bn->setInertia(inertia);

	return bn;
}


dart::dynamics::BodyNode* MakeBallJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass)
{
	ShapePtr shape = std::shared_ptr<BoxShape>(new BoxShape(size));

	dart::dynamics::Inertia inertia;
	inertia.setMass(mass);
	inertia.setMoment(shape->computeInertia(mass));

	BodyNode* bn;
	BallJoint::Properties props;
	props.mName = name + "_joint";
	if(parent!=nullptr)
	{
		props.mT_ParentBodyToJoint.setIdentity();
		props.mT_ParentBodyToJoint.translation() = joint_pos-parent->getCOM();
	}

	props.mT_ChildBodyToJoint.setIdentity();
	props.mT_ChildBodyToJoint.translation() = joint_pos-body_pos;
	
	bn = skel->createJointAndBodyNodePair<BallJoint>(
		parent,props,BodyNode::AspectProperties(name)).second;
	bn->createShapeNodeWith<VisualAspect,CollisionAspect, DynamicsAspect>(shape);
	
	bn->setInertia(inertia);

	return bn;
}
dart::dynamics::BodyNode* MakePrismaticJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass)
{
	ShapePtr shape = std::shared_ptr<BoxShape>(new BoxShape(size));	

	dart::dynamics::Inertia inertia;
	inertia.setMass(mass);
	inertia.setMoment(shape->computeInertia(mass));

	BodyNode* bn;
	PrismaticJoint::Properties props;
	props.mName = name + "_joint";
	props.mAxis = Eigen::Vector3d::UnitX();

	if(parent!=nullptr)
	{
		const auto sn = parent->getShapeNodesWith<DynamicsAspect>();
		const auto box = std::dynamic_pointer_cast<const BoxShape>(sn[0]->getShape());
	}	

	props.mT_ParentBodyToJoint.setIdentity();
	props.mT_ParentBodyToJoint.translation() = body_pos;

	bn = skel->createJointAndBodyNodePair<PrismaticJoint>(
		parent,props,BodyNode::AspectProperties(name)).second;
	bn->createShapeNodeWith<VisualAspect,CollisionAspect, DynamicsAspect>(shape);
	
	bn->setInertia(inertia);

	return bn;
}
dart::dynamics::BodyNode* MakeRevoluteJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass)
{
	ShapePtr shape = std::shared_ptr<BoxShape>(new BoxShape(size));

	dart::dynamics::Inertia inertia;
	inertia.setMass(mass);
	inertia.setMoment(shape->computeInertia(mass));

	BodyNode* bn;
	RevoluteJoint::Properties props;
	props.mName = name + "_joint";
	if(parent!=nullptr)
	{
		props.mT_ParentBodyToJoint.setIdentity();
		props.mT_ParentBodyToJoint.translation() = joint_pos-parent->getCOM();
	}

	props.mT_ChildBodyToJoint.setIdentity();
	props.mT_ChildBodyToJoint.translation() = joint_pos-body_pos;
	props.mAxis = Eigen::Vector3d::UnitZ();
	bn = skel->createJointAndBodyNodePair<RevoluteJoint>(
		parent,props,BodyNode::AspectProperties(name)).second;
	bn->createShapeNodeWith<VisualAspect,CollisionAspect, DynamicsAspect>(shape);
	
	bn->setInertia(inertia);

	return bn;
}
dart::dynamics::BodyNode* MakeWeldJointBody(
	const std::string& name,
	const dart::dynamics::SkeletonPtr& skel,
	dart::dynamics::BodyNode* const parent,
	const Eigen::Vector3d& size,
	const Eigen::Vector3d& joint_pos,
	const Eigen::Vector3d& body_pos,
	double mass)
{
	ShapePtr shape = std::shared_ptr<BoxShape>(new BoxShape(size));

	dart::dynamics::Inertia inertia;
	inertia.setMass(mass);
	inertia.setMoment(shape->computeInertia(mass));

	BodyNode* bn;
	WeldJoint::Properties props;
	props.mName = name + "_joint";
	if(parent!=nullptr)
	{
		props.mT_ParentBodyToJoint.setIdentity();
		props.mT_ParentBodyToJoint.translation() = joint_pos-parent->getCOM();
	}

	props.mT_ChildBodyToJoint.setIdentity();
	props.mT_ChildBodyToJoint.translation() = joint_pos-body_pos;
	bn = skel->createJointAndBodyNodePair<WeldJoint>(
		parent,props,BodyNode::AspectProperties(name)).second;
	bn->createShapeNodeWith<VisualAspect,CollisionAspect, DynamicsAspect>(shape);
	
	bn->setInertia(inertia);	

	return bn;
}




}
}