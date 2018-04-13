#include "Record.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include "StateMachine.h"
namespace VPC {

void
Record::
Set(const dart::simulation::WorldPtr& world,
	const std::shared_ptr<Character>& character)
{
	t = world->getTime();
	for(int i=0;i<world->getNumSkeletons();i++)
	{
		positions.insert(std::make_pair(world->getSkeleton(i)->getName(),world->getSkeleton(i)->getPositions()));
		velocities.insert(std::make_pair(world->getSkeleton(i)->getName(),world->getSkeleton(i)->getVelocities()));
	}
}
void
Record::
Get(const dart::simulation::WorldPtr& world,
	const std::shared_ptr<Character>& character)
{
	world->setTime(t);
	for(int i =0;i<world->getNumSkeletons();i++)
	{

		std::string key = world->getSkeleton(i)->getName();
		if(positions.find(key)!=positions.end()){
			world->getSkeleton(i)->setPositions(positions.at(key));
			world->getSkeleton(i)->setVelocities(velocities.at(key));
		}
		else
		{
			std::cout<<"No skeleton name "<<key<<std::endl;
			exit(0);
		}
	}
}
void
Record::
Set(const dart::simulation::WorldPtr& world,
			const std::shared_ptr<Character>& character,
			const std::shared_ptr<VPC::StateMachine>& fsm)
{
	this->Set(world,character);
	state = fsm->GetState();
}
void
Record::
Get(const dart::simulation::WorldPtr& world,
		const std::shared_ptr<Character>& character,
		const std::shared_ptr<VPC::StateMachine>& fsm)
{
	this->Get(world,character);
	fsm->SetState(state);
}


bool
Record::
Write(const std::string path)
{
	std::ofstream ofs(path);
	for(auto tup : positions)
	{
		ofs<<"rpos ";
		if(dart::math::isNan(tup.second))
		{
			ofs.close();
			boost::filesystem::remove(path);
			return false;
		}
		ofs<<tup.first<<" "<<tup.second.transpose()<<std::endl;	
	}
	for(auto tup : velocities)
	{
		ofs<<"rvel ";
		if(dart::math::isNan(tup.second))
		{
			ofs.close();
			boost::filesystem::remove(path);
			return false;
		}
		ofs<<tup.first<<" "<<tup.second.transpose()<<std::endl;	
	}

	ofs<<"time "<<t<<std::endl;
	if(state!=-1)
		ofs<<"state "<<state<<std::endl;
	ofs.close();
	return true;
}
bool
Record::
Read(const std::string path)
{
	std::ifstream ifs(path);
	if(!(ifs.is_open()))
	{
		std::cout<<"Can't read file "<<path<<std::endl;
		return false;
	}
	std::string str;
	std::string index;
	std::stringstream ss;
	while(!ifs.eof())
	{
		str.clear();
		index.clear();
		ss.clear();

		std::getline(ifs,str);
		ss.str(str);	
		ss>>index;

		Eigen::VectorXd eigen_vec;
		std::vector<double> vec;
		double val;

		if(!index.compare("rpos"))
		{
			std::string name;
			ss>>name;
			while(!ss.eof())
			{
				if(ss>>val)
					vec.push_back(val);
				else{
					continue;
					// return false;
				}
			}
			eigen_vec.resize(vec.size());
			for(int i=0;i<vec.size();i++)
				eigen_vec[i] = vec[i];
			positions.insert(std::make_pair(name,eigen_vec));
		}
		else if(!index.compare("rvel"))
		{
			std::string name;
			ss>>name;
			while(!ss.eof())
			{
				if(ss>>val)
					vec.push_back(val);
				else
				{
					continue;
					// return false;
				}
			}
			eigen_vec.resize(vec.size());
			for(int i=0;i<vec.size();i++)
			{
				eigen_vec[i] = vec[i];
			}
			velocities.insert(std::make_pair(name,eigen_vec));

		}
		else if(!index.compare("time"))
		{
			ss>>val;
			t = val;
		}
		else if(!index.compare("state"))
		{	
			int int_val;
			ss>>int_val;
			state = int_val;
		}
		
	}
	ifs.close();
	return true;
}

}