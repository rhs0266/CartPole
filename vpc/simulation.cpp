#include "simulation.h"

Simulation::Simulation(){
}

void Simulation::Init(){
    world = std::make_shared<VPC::World>(10); // world->TimeStepping() 이 호출 될 때마다 world->step() 이 호출되는 비율
    world->Initialize();
    world->GetWorld()->setTimeStep(0.002);

    output_path = "../output/";
    if(boost::filesystem::exists(output_path))
        boost::filesystem::remove_all(output_path);
    boost::filesystem::create_directories(output_path);
}

void Simulation::Step(){
    world->TimeStepping();

    records.push_back(std::make_shared<VPC::Record>());
    records.back()->Set(world->GetWorld(),world->GetCharacter());
    records.back()->Write(output_path+std::to_string(records.size()-1));
}

//boost::python::list Simulation::GetState(){}
//void Simulation::SetAction(boost::python::list v){}
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
            .def("Step",&Simulation::Step);
}


