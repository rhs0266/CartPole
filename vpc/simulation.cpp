#include "simulation.h"

boost::python::list toPyList(const Eigen::VectorXd& vec)
{
    boost::python::list py_list;
    for(int i =0;i<vec.rows();i++)
        py_list.append(vec[i]);
    return py_list;
}
Eigen::VectorXd toEigen(boost::python::list py_list)
{
    Eigen::VectorXd vec(boost::python::len(py_list));
    for(int i =0;i<vec.rows();i++)
        vec[i] = boost::python::extract<int>(py_list[i]);
    return vec;
}

Simulation::Simulation(){
}

void Simulation::Init(){
    world = std::make_shared<VPC::World>(5); // world->TimeStepping() 이 호출 될 때마다 world->step() 이 호출되는 비율
    world->Initialize();
    world->GetWorld()->setTimeStep(0.0001);

    output_path = "../output/";
    if(boost::filesystem::exists(output_path))
        boost::filesystem::remove_all(output_path);
    boost::filesystem::create_directories(output_path);
}

void Simulation::Reset(){
    world->Initialize();
    if(boost::filesystem::exists(output_path))
        boost::filesystem::remove_all(output_path);
    boost::filesystem::create_directories(output_path);
}

void Simulation::Step(){
    world->TimeStepping();

//    records.push_back(std::make_shared<VPC::Record>());
//    records.back()->Set(world->GetWorld(),world->GetCharacter());
//    records.back()->Write(output_path+std::to_string(records.size()-1));
}

boost::python::list Simulation::GetState(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions() * 2.0;
    Eigen::VectorXd vec = world->GetCharacter()->GetSkeleton()->getVelocities() * 0.2;
    int size = pos.rows();
    Eigen::VectorXd state(size*2);
    state << pos,vec;
    return toPyList(state);
}

int Simulation::GetReward(){
    return 1;
}

int Simulation::GetDone(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions();
    if (abs(pos[0])>2.4 || abs(pos[1])>0.15) return 1;
    return 0;
}

void Simulation::SetAction(boost::python::list v){
    Eigen::VectorXd actList = toEigen(v);
    assert(actList.rows() == 2);
    if (actList[0] == 1) world->Action(true);
    else if (actList[1] == 1) world->Action(false);
    else std::cout<<"WRONG!!!\n";
}
//
//
//boost::python::list
//Simulation::
//Print()
//{
//    double v[4] = {1,2,3,65};
//    boost::python::list vec;
//    vec.append(v[0]);
//    vec.append(v[1]);
//    vec.append(v[2]);
//    vec.append(v[3]);
//    return vec;
//}

using namespace boost::python;

BOOST_PYTHON_MODULE(cp)
{
    class_<Simulation>("Simulation",init<>())
            .def("Init",&Simulation::Init)
            .def("Step",&Simulation::Step)
            .def("Reset",&Simulation::Reset)
            .def("GetState",&Simulation::GetState)
            .def("SetAction",&Simulation::SetAction)
            .def("GetReward",&Simulation::GetReward)
            .def("GetDone",&Simulation::GetDone);
}


