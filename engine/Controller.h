#ifndef __VPC_CONTROLLER_H__
#define __VPC_CONTROLLER_H__
#include "StdIncludes.h"
#include "EigenIncludes.h"
namespace VPC {
class World;
class Controller
{
private:
	std::shared_ptr<VPC::World> mWorld;
	Eigen::VectorXd 			mKp,mKv;

public:
	Controller(const std::shared_ptr<VPC::World>& world);
	Eigen::VectorXd ComputeTorque(const Eigen::VectorXd& p,const Eigen::VectorXd& v);
};
};

#endif