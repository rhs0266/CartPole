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
        vec[i] = boost::python::extract<float>(py_list[i]);
    return vec;
}

Simulation::Simulation(){
}

void Simulation::init(){
    world = std::make_shared<VPC::World>(1); // world->TimeStepping() 이 호출 될 때마다 world->step() 이 호출되는 비율
    world->Initialize();
    world->GetWorld()->setTimeStep(0.0001);

    int size = world->GetCharacter()->GetSkeleton()->getPositions().rows();
    for (int i=0;i<4;i++) mDesiredDofs[i] = Eigen::VectorXd::Zero(size);

    fsm = std::make_shared<VPC::StateMachine>(world);
    controller = std::make_shared<VPC::Controller>(world);
    fsm->Initialize();

    output_path = "../output/";
    if(boost::filesystem::exists(output_path))
        boost::filesystem::remove_all(output_path);
    boost::filesystem::create_directories(output_path);
}

void Simulation::reset(){
    world->Initialize();
//    if(boost::filesystem::exists(output_path))
//        boost::filesystem::remove_all(output_path);
//    boost::filesystem::create_directories(output_path);
}

void Simulation::step(bool recordFlag){
    Eigen::VectorXd p,v;
    int lastState = 0, stepCount = 0;
    reward = 0.0;
    for (;stepCount<10;){
        stepCount++;
        fsm->GetMotion(p,v);

        int newState = fsm->GetState();
//        if (world->GetCharacter()->GetSkeleton()->getPosition(4) <= -0.2207){ // determine simbicon falls down
//            break;
//        }
        if (lastState == 3 && newState == 0){
            //break;
        }
        Eigen::VectorXd tau = controller->ComputeTorque(p,v);
        std::cout << "Tau : " << tau << std::endl;
        tau = fsm->TorsoControl(tau);
        world->GetCharacter()->GetSkeleton()->setForces(tau);
        world->TimeStepping();

        double desiredVelocity = -1.3; // z-axis
        double currentVelocity = world->GetCharacter()->GetSkeleton()->getVelocity(5);
        double zVelocityReward = exp(-pow((desiredVelocity - currentVelocity),2.0));

        double torsoAngle = world->GetCharacter()->GetSkeleton()->getPosition(0);
        double torsoUpwardReward = exp(-pow(torsoAngle, 2.0) );

        bool fallDownFlag = world->GetCharacter()->GetSkeleton()->getPosition(4) <= -0.2207;
        double fallDownPenalty = fallDownFlag ? -10 : 0.0;

        reward += 1.0 * zVelocityReward + 0.5 * torsoUpwardReward + 1.0 * fallDownPenalty;

        lastState = newState;

        //if (fallDownFlag) break;

        if (recordFlag) {
            records.push_back(std::make_shared<VPC::Record>());
            records.back()->Set(world->GetWorld(), world->GetCharacter(),fsm);
            records.back()->Write(output_path + std::to_string(records.size() - 1));
        }
    }
    reward /= stepCount;
}

int Simulation::getStateNum(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions();
    int size = pos.rows() * 2 + 1;
    return size;
}


int Simulation::getActionNum(){
    return 24; // state number(4) * joint number(6)
}

boost::python::list Simulation::getState(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions() * 2.0;
    Eigen::VectorXd vec = world->GetCharacter()->GetSkeleton()->getVelocities() * 0.2;
    Eigen::VectorXd phase(1); phase << fsm->GetState()/4.0;

    int size = pos.rows();
    Eigen::VectorXd state(size*2 + 1);

    //state << pos,vec,phase;
    //std::cout << state << "#" << std::endl;
    return toPyList(state);
}

double Simulation::getReward(){
    return reward;
}

int Simulation::getDone(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions();
    if (abs(pos[0])>1.5 || abs(pos[1])>15.0/180.0*3.141592) return 1;
    return 0;
}

void Simulation::setAction(boost::python::list action){
    fsm->SetStatePose(toEigen(action));
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
            .def("init",&Simulation::init)
            .def("step",&Simulation::step)
            .def("reset",&Simulation::reset)
            .def("getState",&Simulation::getState)
            .def("setAction",&Simulation::setAction)
            .def("getReward",&Simulation::getReward)
            .def("getDone",&Simulation::getDone);
}


