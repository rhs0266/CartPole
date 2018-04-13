#ifndef __VPC_RECORD_H__
#define __VPC_RECORD_H__
#include "StdIncludes.h"
#include "EigenIncludes.h"
#include "dart/dart.hpp"
namespace VPC {
class Character;
class StateMachine;
class Record
{
public:
	double t;
	std::map<std::string,Eigen::VectorXd> positions;
	std::map<std::string,Eigen::VectorXd> velocities;
	int state;
public:
	Record():state(-1){};

	void Set(const dart::simulation::WorldPtr& world,
			const std::shared_ptr<Character>& character);
	void Set(const dart::simulation::WorldPtr& world,
			const std::shared_ptr<Character>& character,
			const std::shared_ptr<VPC::StateMachine>& fsm);

	void Get(const dart::simulation::WorldPtr& world,
			const std::shared_ptr<Character>& character);
	void Get(const dart::simulation::WorldPtr& world,
			const std::shared_ptr<Character>& character,
			const std::shared_ptr<VPC::StateMachine>& fsm);
	bool Write(const std::string path);
	bool Read(const std::string path);
};

}

#endif
