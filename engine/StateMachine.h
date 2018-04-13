#ifndef __VPC_STATE_MACHINE_H__
#define __VPC_STATE_MACHINE_H__
#include "StdIncludes.h"
#include "EigenIncludes.h"
namespace VPC {
class World;
struct State
{
	Eigen::VectorXd target_positions;
	double c_d,c_v;
	State():target_positions(Eigen::VectorXd::Zero(0)),c_d(0),c_v(0){};
};

class StateMachine
{
private:
	std::shared_ptr<VPC::World>  mWorld;
	std::vector<State> mStates;
	int mState;
	double mElapsedTime;

	double ComputeD();
	double ComputeV();
	Eigen::VectorXd AddBalanceControl(const Eigen::VectorXd& p0);
	std::string GetStanceFootName();
	std::string GetSwingFootName();

public:
	StateMachine(const std::shared_ptr<VPC::World>& world);
	int GetState(){return mState;};
	void SetState(int state) {mState =state;}
	void Initialize();
	Eigen::VectorXd TorsoControl(const Eigen::VectorXd& tau);
	void GetMotion(Eigen::VectorXd& p,Eigen::VectorXd& v);
};


}
#endif