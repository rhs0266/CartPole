#include "World.h"
#include "DartHelper.h"
#include "Character.h"
using namespace dart;
using namespace dart::simulation;
using namespace dart::dynamics;
namespace VPC {

World::
World(int num_sim_step)
	:mNumSimulationStep(num_sim_step)
{
	
}

void
World::
Initialize()
{
	mWorld = std::make_shared<dart::simulation::World>();
	mWorld->setGravity(Eigen::Vector3d(0,-9.81,0));
	mCharacter = std::make_shared<VPC::Character>("cartpole");
	mCharacter->Initialize();
	mGround = Skeleton::create("ground");
	DartHelper::MakeWeldJointBody(
		"Ground",
		mGround,
		nullptr,
		Eigen::Vector3d(100.0,0.2,100.0),
		Eigen::Vector3d(0,0,0),
		Eigen::Vector3d(0,-0.1,0),
		10);
	
	// Eigen::VectorXd p = mCharacter->GetSkeleton()->getPositions();
	// p[1] = 0.01;
	// mCharacter->GetSkeleton()->setPositions(p);
	mWorld->addSkeleton(mCharacter->GetSkeleton());
	//mWorld->addSkeleton(mGround);
}
void
World::
TimeStepping()
{
	for(int i =0;i<mNumSimulationStep;i++)
		mWorld->step();
}

void
World::
Action(float v)
{
	mCharacter->GetSkeleton()->getBodyNode(0)->addExtForce(Eigen::Vector3d(v, 0, 0), Eigen::Vector3d(0, 0, 0));
}
}
