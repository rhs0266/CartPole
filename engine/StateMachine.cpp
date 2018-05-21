#include "StateMachine.h"
#include "World.h"
#include "Character.h"
namespace VPC {

StateMachine::
StateMachine(const std::shared_ptr<VPC::World>& world)
	:mState(0),mElapsedTime(0.0),mWorld(world)
{

}

void
StateMachine::
Initialize()
{
	/*auto& skel = mWorld->GetCharacter()->GetSkeleton();
	//Initialize Walking fsm
	Eigen::VectorXd p = skel->getPositions();
	p.setZero();
	Eigen::VectorXd p0=p;
	Eigen::VectorXd p1=p;
	Eigen::VectorXd p2=p;
	Eigen::VectorXd p3=p;

	//Swing Foot
	p0[6] = 0.5;
	p0[8] = -1.1;
	p0[10] = 0.1;
	//Stance Foot
	p1[9] = -0.05;
	p1[11] = 0.1;

	//Swing Foot
	p1[6] = -0.7;
	p1[8] = -0.05;
	p1[10] = 0.1;
	//Stance Foot
	p1[9] = -0.1;
	p1[11] = 0.1;

	//Swing Foot
	p2[7] = 0.5;
	p2[9] = -1.1;
	p2[11] = 0.1;
	//Stance Foot
	p2[8] = -0.05;
	p2[10] = 0.1;

	//Swing Foot
	p3[7] = -0.7;
	p3[9] = -0.05;
	p3[11] = 0.1;
	//Stance Foot
	p3[8] = -0.1;
	p3[10] = 0.1;

	State s0,s1,s2,s3;
	s0.target_positions = p0;
	s1.target_positions = p1;
	s2.target_positions = p2;
	s3.target_positions = p3;

	s0.c_d = 0.0;
	s0.c_v = 0.01;

	s1.c_d = 1.0;
	s1.c_v = 0.0;

	s2.c_d = 0.0;
	s2.c_v = 0.01;

	s3.c_d = 1.0;
	s3.c_v = 0.0;

	mStates.push_back(s0);
	mStates.push_back(s1);
	mStates.push_back(s2);
	mStates.push_back(s3);*/
}

void
StateMachine::
SetStatePose(Eigen::VectorXd action){
	auto& skel = mWorld->GetCharacter()->GetSkeleton();
	Eigen::VectorXd p = skel->getPositions();
	p.setZero();
	Eigen::VectorXd p0=p;
	Eigen::VectorXd p1=p;
	Eigen::VectorXd p2=p;
	Eigen::VectorXd p3=p;

	//Swing Foot
	p0[6] = action[0];
	p0[8] = action[1];
	p0[10] = action[2];
	//Stance Foot
	p0[7] = action[3];
	p0[9] = action[4];
	p0[11] = action[5];

	//Swing Foot
	p1[6] = action[6];
	p1[8] = action[7];
	p1[10] = action[8];
	//Stance Foot
	p1[7] = action[9];
	p1[9] = action[10];
	p1[11] = action[11];

	//Swing Foot
	p2[7] = action[12];
	p2[9] = action[13];
	p2[11] = action[14];
	//Stance Foot
	p2[6] = action[15];
	p2[8] = action[16];
	p2[10] = action[17];

	//Swing Foot
	p3[7] = action[18];
	p3[9] = action[19];
	p3[11] = action[20];
	//Stance Foot
	p3[6] = action[21];
	p3[8] = action[22];
	p3[10] = action[23];

	State s0,s1,s2,s3;
	s0.target_positions = p0;
	s1.target_positions = p1;
	s2.target_positions = p2;
	s3.target_positions = p3;

	mStates.clear();
	mStates.push_back(s0);
	mStates.push_back(s1);
	mStates.push_back(s2);
	mStates.push_back(s3);
}

void
StateMachine::
GetMotion(Eigen::VectorXd& p,Eigen::VectorXd& v)
{
	auto& skel = mWorld->GetCharacter()->GetSkeleton();
	p.resize(skel->getNumDofs());
	v.resize(skel->getNumDofs());
	p.setZero();
	v.setZero();

	mElapsedTime +=mWorld->GetWorld()->getTimeStep();
	if(mState == 1 ||mState == 3)
	{
		// std::cout<<mState<<std::endl;
		if(mWorld->GetWorld()->getLastCollisionResult().inCollision(skel->getBodyNode(GetSwingFootName())))
		{
//			std::cout<<"Contact ! "<<GetSwingFootName()<<std::endl;
			mElapsedTime = 0.0;
			mState++;
		}
	}
	else if(mElapsedTime>0.3){
		mElapsedTime = 0.0;
		mState++;
	}

	mState %=4;
	p = mStates[mState].target_positions;
//	p = AddBalanceControl(p);


}
/*double
StateMachine::
ComputeD()
{
	Eigen::Vector3d d_root = mWorld->GetCharacter()->GetSkeleton()->getCOM();
	auto ankle = mWorld->GetCharacter()->GetSkeleton()->getBodyNode(GetStanceFootName())->getParentJoint();

	Eigen::Isometry3d T;
	T.setIdentity();

	T = ankle->getParentBodyNode()->getTransform()*ankle->getTransformFromParentBodyNode();
	Eigen::Vector3d d_ankle = T.translation();

	return (d_root-d_ankle)[2];
}
double
StateMachine::
ComputeV()
{
	Eigen::Vector3d v = mWorld->GetCharacter()->GetSkeleton()->getCOMLinearVelocity();
	return v[2];
}
Eigen::VectorXd
StateMachine::
AddBalanceControl(const Eigen::VectorXd& p0)
{
	Eigen::VectorXd p = p0;

	double feedback = mStates[mState].c_d*ComputeD() + mStates[mState].c_v*ComputeV();
	// double feedback = -mStates[mState].c_d*ComputeD();

	// std::cout<<feedback<<std::endl;
	// feedback =0;
	for(int i =0;i<p.cols();i++)
	{
		p[i] -=feedback;
	}
	// p[0] += feedback;
	return p;
}*/
std::string
StateMachine::
GetStanceFootName()
{
	std::string stance_foot_name;
	if(mState == 0 || mState == 1)
		stance_foot_name = "FootL";
	else
		stance_foot_name = "FootR";
	return stance_foot_name;
}
std::string
StateMachine::
GetSwingFootName()
{
	std::string swing_foot_name;
	if(mState == 0 || mState == 1)
		swing_foot_name = "FootR";
	else
		swing_foot_name = "FootL";
	return swing_foot_name;
}
Eigen::VectorXd
StateMachine::
TorsoControl(const Eigen::VectorXd& tau)
{
	Eigen::VectorXd newtau = tau;
	std::string swing_foot = GetSwingFootName();
	std::string stance_foot_name = GetStanceFootName();

	int swing_foot_index = mWorld->GetCharacter()->GetSkeleton()->getBodyNode(swing_foot)->getParentJoint()->getIndexInSkeleton(0);
	int stance_foot_index = mWorld->GetCharacter()->GetSkeleton()->getBodyNode(stance_foot_name)->getParentJoint()->getIndexInSkeleton(0);
	newtau[stance_foot_index-4] = -newtau[0]-newtau[swing_foot_index-4];
	newtau.block<6,1>(0,0).setZero();
	return newtau;
}
};