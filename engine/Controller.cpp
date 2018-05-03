#include "Controller.h"
#include "World.h"
#include "Character.h"
namespace VPC {
Controller::
Controller(const std::shared_ptr<VPC::World>& world)
	:mWorld(world)
{
	auto& skel =mWorld->GetCharacter()->GetSkeleton();
	mKv = Eigen::VectorXd::Zero(skel->getNumDofs());
	mKp = Eigen::VectorXd::Zero(skel->getNumDofs());


	int start_index = skel->getBodyNode(1)->getParentJoint()->getIndexInSkeleton(0);
	// mKp.block<3,1>(6,0) = Eigen::Vector3d::Constant(50);
	// mKp.block<3,1>(9,0) = Eigen::Vector3d::Constant(50);

	// mKp.block<3,1>(12,0) = Eigen::Vector3d::Constant(10);
	// mKp.block<3,1>(15,0) = Eigen::Vector3d::Constant(10);

	// mKp.block<3,1>(18,0) = Eigen::Vector3d::Constant(10);
	// mKp.block<3,1>(21,0) = Eigen::Vector3d::Constant(10);
	mKp = Eigen::VectorXd::Constant(skel->getNumDofs(),300);
	mKv = Eigen::VectorXd::Constant(skel->getNumDofs(),30);
	// mKp.tail(skel->getNumDofs()-start_index) = Eigen::VectorXd::Constant(skel->getNumDofs()-start_index,300);
	// mKv.tail(skel->getNumDofs()-start_index) = Eigen::VectorXd::Constant(skel->getNumDofs()-start_index,30);
}
Eigen::VectorXd
Controller::
ComputeTorque(const Eigen::VectorXd& p,const Eigen::VectorXd& v)
{
	auto& skel =mWorld->GetCharacter()->GetSkeleton();
	Eigen::VectorXd p_diff = skel->getPositionDifferences(p,skel->getPositions());
	Eigen::VectorXd v_diff = v - skel->getVelocities();
	Eigen::VectorXd tau = mKp.cwiseProduct(p_diff) + mKv.cwiseProduct(v_diff);
	return tau;
}

};
