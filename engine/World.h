#ifndef __VPC_WORLD_H__
#define __VPC_WORLD_H__
#include "DartHelper.h"
namespace VPC {
class Character;
class World
{
private:
	std::shared_ptr<Character> mCharacter;
	dart::dynamics::SkeletonPtr mGround;
	dart::simulation::WorldPtr mWorld;

	int mNumSimulationStep;
public:
	World(int num_sim_step);
	void Initialize();
	void TimeStepping();
	void Action(bool isleft);
	const std::shared_ptr<Character>& GetCharacter(){return mCharacter;}
	const dart::simulation::WorldPtr& GetWorld(){return mWorld;}
};

}

#endif
