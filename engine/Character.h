#ifndef __VPC_CHARACTER_H__
#define __VPC_CHARACTER_H__
#include "StdIncludes.h"
#include "dart/dynamics/dynamics.hpp"

namespace VPC {
class Character
{
private:
	std::string mName;
	dart::dynamics::SkeletonPtr mSkeleton;
public:
	Character(const std::string name);
	void Initialize();

	const dart::dynamics::SkeletonPtr& GetSkeleton() {return mSkeleton;}
};
}


#endif