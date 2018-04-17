#include "Character.h"
#include "DartHelper.h"
#include <iostream>
using namespace dart::dynamics;
namespace VPC{

Character::
Character(const std::string name)
	:mName(name)
{
	
}

void
Character::
Initialize()
{
	mSkeleton = Skeleton::create(mName);
	auto cart = DartHelper::MakePrismaticJointBody(
	 	"Cart",
	 	mSkeleton,
	 	nullptr,
	 	Eigen::Vector3d(0.2,0.1,0.1),
	 	Eigen::Vector3d(0,0,0),
	 	Eigen::Vector3d(0.0,0.0501,0.0),1.0);

	auto pole = DartHelper::MakeRevoluteJointBody(
	 	"Pole",
	 	mSkeleton,
	 	cart,
	 	Eigen::Vector3d(0.05,0.5,0.05),
	 	Eigen::Vector3d(0,0.05,0),
	 	Eigen::Vector3d(0,0.3,0),1.0);

	// Pole initially tilted
    srand(time(NULL));
    double angle = rand()%40 - 20;
	mSkeleton->setPosition(1,0.001*angle);

//	DartHelper::MakeFreeJointBody(
//		"Torso",
//		mSkeleton,
//		nullptr,
//		Eigen::Vector3d(0.0,0.4,0.2),
//		Eigen::Vector3d(0,1.1,0),
//		70);
//
//	DartHelper::MakeRevoluteJointBody(
//		"ThighR",
//		mSkeleton,
//		mSkeleton->getBodyNode("Torso"),
//		Eigen::Vector3d(0.0,0.4,0.1),
//		Eigen::Vector3d(-0.0,0.9,0),
//		Eigen::Vector3d(-0.0,0.7,0),
//		5);
//
//	DartHelper::MakeRevoluteJointBody(
//		"ThighL",
//		mSkeleton,
//		mSkeleton->getBodyNode("Torso"),
//		Eigen::Vector3d(0.0,0.4,0.1),
//		Eigen::Vector3d(0.0,0.9,0),
//		Eigen::Vector3d(0.0,0.7,0),
//		5);
//
//	DartHelper::MakeRevoluteJointBody(
//		"KneeR",
//		mSkeleton,
//		mSkeleton->getBodyNode("ThighR"),
//		Eigen::Vector3d(0.0,0.4,0.1),
//		Eigen::Vector3d(-0.0,0.5,0),
//		Eigen::Vector3d(-0.0,0.3,0),
//		4);
//
//	DartHelper::MakeRevoluteJointBody(
//		"KneeL",
//		mSkeleton,
//		mSkeleton->getBodyNode("ThighL"),
//		Eigen::Vector3d(0.0,0.4,0.1),
//		Eigen::Vector3d(0.0,0.5,0),
//		Eigen::Vector3d(0.0,0.3,0),
//		4);
//
//	DartHelper::MakeRevoluteJointBody(
//		"FootR",
//		mSkeleton,
//		mSkeleton->getBodyNode("KneeR"),
//		Eigen::Vector3d(0.0,0.06,0.2),
//		Eigen::Vector3d(-0.0,0.1,0),
//		Eigen::Vector3d(-0.0,0.05,-0.03),
//		1);
//
//	DartHelper::MakeBallJointBody(
//		"FootL",
//		mSkeleton,
//		mSkeleton->getBodyNode("KneeL"),
//		Eigen::Vector3d(0.0,0.06,0.2),
//		Eigen::Vector3d(0.0,0.1,0),
//		Eigen::Vector3d(0.0,0.05,-0.03),
//		1);







	// DartHelper::MakeFreeJointBody(
	// 	"Torso",
	// 	mSkeleton,
	// 	nullptr,
	// 	Eigen::Vector3d(0.4,0.4,0.2),
	// 	Eigen::Vector3d(0,1.1,0),
	// 	70);

	// DartHelper::MakeRevoluteJointBody(
	// 	"ThighR",
	// 	mSkeleton,
	// 	mSkeleton->getBodyNode("Torso"),
	// 	Eigen::Vector3d(0.1,0.4,0.1),
	// 	Eigen::Vector3d(-0.2,0.9,0),
	// 	Eigen::Vector3d(-0.2,0.7,0),
	// 	5);

	// DartHelper::MakeRevoluteJointBody(
	// 	"ThighL",
	// 	mSkeleton,
	// 	mSkeleton->getBodyNode("Torso"),
	// 	Eigen::Vector3d(0.1,0.4,0.1),
	// 	Eigen::Vector3d(0.2,0.9,0),
	// 	Eigen::Vector3d(0.2,0.7,0),
	// 	5);

	// DartHelper::MakeRevoluteJointBody(
	// 	"KneeR",
	// 	mSkeleton,
	// 	mSkeleton->getBodyNode("ThighR"),
	// 	Eigen::Vector3d(0.1,0.4,0.1),
	// 	Eigen::Vector3d(-0.2,0.5,0),
	// 	Eigen::Vector3d(-0.2,0.3,0),
	// 	4);

	// DartHelper::MakeRevoluteJointBody(
	// 	"KneeL",
	// 	mSkeleton,
	// 	mSkeleton->getBodyNode("ThighL"),
	// 	Eigen::Vector3d(0.1,0.4,0.1),
	// 	Eigen::Vector3d(0.2,0.5,0),
	// 	Eigen::Vector3d(0.2,0.3,0),
	// 	4);

	// DartHelper::MakeRevoluteJointBody(
	// 	"FootR",
	// 	mSkeleton,
	// 	mSkeleton->getBodyNode("KneeR"),
	// 	Eigen::Vector3d(0.2,0.1,0.3),
	// 	Eigen::Vector3d(-0.2,0.1,0),
	// 	Eigen::Vector3d(-0.2,0.05,0.07),
	// 	1);

	// DartHelper::MakeBallJointBody(
	// 	"FootL",
	// 	mSkeleton,
	// 	mSkeleton->getBodyNode("KneeL"),
	// 	Eigen::Vector3d(0.2,0.1,0.3),
	// 	Eigen::Vector3d(0.2,0.1,0),
	// 	Eigen::Vector3d(0.2,0.05,0.07),
	// 	1);
}
}