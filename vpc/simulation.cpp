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

    // test
//    {
//
//        fsm->GetMotion(p,v);
//        {
//            world->GetCharacter()->GetSkeleton()->setPositions(p);
//
//            if (recordFlag) {
//                records.push_back(std::make_shared<VPC::Record>());
//                records.back()->Set(world->GetWorld(), world->GetCharacter(), fsm);
//                records.back()->Write(output_path + std::to_string(records.size() - 1));
//            }
//        }
//        fsm->GetMotion(p,v);
//        {
//            world->GetCharacter()->GetSkeleton()->setPositions(p);
//
//            if (recordFlag) {
//                records.push_back(std::make_shared<VPC::Record>());
//                records.back()->Set(world->GetWorld(), world->GetCharacter(), fsm);
//                records.back()->Write(output_path + std::to_string(records.size() - 1));
//            }
//        }
//        fsm->GetMotion(p,v);
//        {
//            world->GetCharacter()->GetSkeleton()->setPositions(p);
//
//            if (recordFlag) {
//                records.push_back(std::make_shared<VPC::Record>());
//                records.back()->Set(world->GetWorld(), world->GetCharacter(), fsm);
//                records.back()->Write(output_path + std::to_string(records.size() - 1));
//            }
//        }
//        fsm->GetMotion(p,v);
//        {
//            world->GetCharacter()->GetSkeleton()->setPositions(p);
//
//            if (recordFlag) {
//                records.push_back(std::make_shared<VPC::Record>());
//                records.back()->Set(world->GetWorld(), world->GetCharacter(), fsm);
//                records.back()->Write(output_path + std::to_string(records.size() - 1));
//            }
//        }
//
//    }
//    return;
//

    for (;;){
        stepCount++;
        fsm->GetMotion(p,v);
        int newState = fsm->GetState();

        if (lastState != newState){
            reward += 10;
        }

        if (lastState == 3 && newState == 0){
            break;
        }
        Eigen::VectorXd tau = controller->ComputeTorque(p,v);
        tau = fsm->TorsoControl(tau);
//        std::cout << "State : " << newState << std::endl;
//        std::cout << "Tau : " << tau << std::endl;
        world->GetCharacter()->GetSkeleton()->setForces(tau);
        world->TimeStepping();

//        double zPositionReward = world->GetCharacter()->GetSkeleton()->getPosition(2); // z-value
        double zPositionReward = 1.0; // z-value

        double desiredVelocity = -1.3; // z-axis
        double currentVelocity = world->GetCharacter()->GetSkeleton()->getVelocity(5);
        double zVelocityReward = exp(-pow((desiredVelocity - currentVelocity),2.0));

        double torsoAngle = world->GetCharacter()->GetSkeleton()->getPosition(0);
        double torsoUpwardReward = exp(-pow(torsoAngle, 2.0) );

        bool strangeFlag = !((world->GetCharacter()->GetSkeleton()->getPosition(4) >= -0.3207) && (world->GetCharacter()->GetSkeleton()->getPosition(4) <= 0.3207));
        double strangePenalty = strangeFlag ? -10 : 0.0;

        if (lastState != newState){
            reward += zPositionReward + zVelocityReward + torsoUpwardReward;
        }
        reward += strangePenalty;
//        reward += zPositionReward + torsoUpwardReward + strangePenalty;
        //std::cout << reward << std::endl;

        lastState = newState;

        if (strangeFlag) break;

        if (recordFlag) {
            records.push_back(std::make_shared<VPC::Record>());
            records.back()->Set(world->GetWorld(), world->GetCharacter(),fsm);
            records.back()->Write(output_path + std::to_string(records.size() - 1));
        }
    }
//    reward /= stepCount;
}

int Simulation::getStateNum(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions();
    int size = pos.rows() * 2;
    return size;
}


int Simulation::getActionNum(){
    return 24; // state number(4) * joint number(6)
}

boost::python::list Simulation::getState(){
    Eigen::VectorXd pos = world->GetCharacter()->GetSkeleton()->getPositions();
//    pos[0] = pos[2] = 0; // delete body x, z position
    Eigen::VectorXd vec = world->GetCharacter()->GetSkeleton()->getVelocities();

    int size = pos.rows();
    Eigen::VectorXd state(size*2);

    state << pos,vec;
    //std::cout << state << "#" << std::endl;
    return toPyList(state);
}

double Simulation::getReward(){
    return reward;
}

int Simulation::getDone(){
    bool strangeFlag = !((world->GetCharacter()->GetSkeleton()->getPosition(4) >= -0.3207) && (world->GetCharacter()->GetSkeleton()->getPosition(4) <= 0.3207));
    return strangeFlag;
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


