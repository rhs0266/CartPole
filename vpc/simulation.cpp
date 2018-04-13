#include "simulation.h"
#include "engine/engine.h"
#include <dart/dart.hpp>
#include <dart/gui/gui.hpp>

const double default_height = 0.1; // m
const double default_width = 0.2;  // m
const double default_depth = 0.1;  // m

const double default_torque = 15.0; // N-m
const double default_force = 25.0; // N
const int default_countdown = 200;  // Number of timesteps for applying force

const double default_rest_position = 0.0;
const double delta_rest_position = 10.0 * M_PI / 180.0;

const double default_stiffness = 0.0;
const double delta_stiffness = 10;

const double default_damping = 5.0;
const double delta_damping = 1.0;

const double default_Kp = 0.0;
const double delta_Kp = 3.0;

const double default_Kd = 0.0;
const double delta_Kd = 3.0;

using namespace dart::dynamics;
using namespace dart::simulation;

class MyWindow : public dart::gui::SimWindow {
public:

    /// Constructor
    MyWindow(WorldPtr world)
            : mPositiveSign(true),
              mBodyForce(false),
              mPDControl(false),
              Kp(default_Kp), Kd(default_Kd) {
        setWorld(world);

        // Find the Skeleton named "CartPole" within the World
        mCartPole = world->getSkeleton("cartpole");

        // Make sure that the pendulum was found in the World
        assert(mCartPole != nullptr);

        mForceCountDown.resize(mCartPole->getNumDofs(), 0);

        ArrowShape::Properties arrow_properties;
        arrow_properties.mRadius = 0.05;
        mArrow = std::shared_ptr<ArrowShape>(new ArrowShape(
                Eigen::Vector3d(-default_height, 0.0, default_height / 2.0),
                Eigen::Vector3d(-default_width / 2.0, 0.0, default_height / 2.0),
                arrow_properties, dart::Color::Orange(1.0)));
    }

    void changeDirection() {
        mPositiveSign = !mPositiveSign;
        if (mPositiveSign) {
            mArrow->setPositions(
                    Eigen::Vector3d(-default_height, 0.0, default_height / 2.0),
                    Eigen::Vector3d(-default_width / 2.0, 0.0, default_height / 2.0));
        } else {
            mArrow->setPositions(
                    Eigen::Vector3d(default_height, 0.0, default_height / 2.0),
                    Eigen::Vector3d(default_width / 2.0, 0.0, default_height / 2.0));
        }
    }

    void applyForce(std::size_t index) {
        if (index < mForceCountDown.size())
            mForceCountDown[index] = default_countdown;
    }

    void changeRestPosition(double delta) {
        // Lesson 2a
        for (std::size_t i = 0; i < mCartPole->getNumDofs(); i++) {
            DegreeOfFreedomPtr dof = mCartPole->getDof(i);
            double newRestPosition = dof->getRestPosition() + delta;
            if (std::abs(newRestPosition) > 90.0 * M_PI / 180.0)
                newRestPosition = (newRestPosition > 0) ? (90.0 * M_PI / 180.0) : -(90.0 * M_PI / 180.0);
            dof->setRestPosition(newRestPosition);
        }
//        mCartPole->getDof(0)->setRestPosition(0.0);
//        mCartPole->getDof(2)->setRestPosition(0.0);
    }

    void changeStiffness(double delta) {
        // Lesson 2b
        for (std::size_t i = 0; i < mCartPole->getNumDofs(); i++) {
            DegreeOfFreedomPtr dof = mCartPole->getDof(i);
            double stiffness = dof->getSpringStiffness() + delta;
            if (stiffness < 0.0) stiffness = 0.0;
            dof->setSpringStiffness(stiffness);
        }

//        mCartPole->getDof(0)->setSpringStiffness(0.0);
//        mCartPole->getDof(2)->setSpringStiffness(0.0);
    }

    void changeDamping(double delta) {
        // Lesson 2c
        for (size_t i = 0; i < mCartPole->getNumDofs(); ++i) {
            DegreeOfFreedom *dof = mCartPole->getDof(i);
            double damping = dof->getDampingCoefficient() + delta;
            if (damping < 0.0)
                damping = 0.0;
            dof->setDampingCoefficient(damping);
        }
    }

    void changeKp(double delta) {
        Kp += delta;
        if (Kp < 0) Kp = 0;
        std::cout << "Proportional gain : " << Kp << std::endl;
    }

    void changeKd(double delta) {
        Kd += delta;
        if (Kd < 0) Kd = 0;
        std::cout << "Derivative gain : " << Kd << std::endl;
    }

    void printDofs() {
        for (std::size_t i = 0; i < mCartPole->getNumDofs(); i++) {
            std::cout << "Dof #" << i << " : " << mCartPole->getDof(i)->getPosition() / M_PI * 180.0 << std::endl;
        }
    }

    /// Handle keyboard input
    void keyboard(unsigned char key, int x, int y) override {
        switch (key) {
            case '-':
                changeDirection();
                break;

            case '1':
                applyForce(0);
                break;

            case 'w':
                changeStiffness(delta_stiffness);
                break;
            case 's':
                changeStiffness(-delta_stiffness);
                break;

            case 'e':
                changeDamping(delta_damping);
                break;
            case 'd':
                changeDamping(-delta_damping);
                break;

            case 'f':
                mBodyForce = !mBodyForce;
                break;

            case 'v':
                mPDControl = !mPDControl;
                if (mPDControl) std::cout << "PD On" << std::endl;
                else std::cout << "PD off" << std::endl;
                break;

            case 'h':
                changeKp(delta_Kp);
                break;

            case 'j':
                changeKp(-delta_Kp);
                break;

            case 'k':
                changeKd(delta_Kd);
                break;

            case 'l':
                changeKd(-delta_Kd);
                break;

            case 'p':
                printDofs();
                break;

            default:
                SimWindow::keyboard(key, x, y);
        }
    }

    void timeStepping() override {
        // Reset all the shapes to be Blue
        // Lesson 1a
//        for (std::size_t i = 0; i < mCartPole->getNumBodyNodes(); i++) {
//            BodyNodePtr bn = mCartPole->getBodyNode(i);
//            bn->getShapeNode(1)->getVisualAspect()->setColor(dart::Color::Blue()); // body
//            bn->getShapeNode(0)->getVisualAspect()->setColor(dart::Color::Red()); // joint
//
//            // TODO: Remove any arrows
//            if (bn->getNumShapeNodes() == 3) {
//                //(bn->getShapeNodesWith<VisualAspect>())[2]->remove();
//                bn->getShapeNode(2)->remove();
//            }
//
//        }
//
        // Apply body forces based on user input, and color the body shape red
        for (std::size_t i = 0; i < mCartPole->getNumBodyNodes(); ++i) {
            if (mForceCountDown[i] > 0) {
                // Lesson 1c
                BodyNodePtr bn = mCartPole->getBodyNode(i);

                Eigen::Vector3d force = default_force * Eigen::Vector3d::UnitX();
                Eigen::Vector3d location(0, 0, 0);

                if (!mPositiveSign) force = -force, location[0] = -location[0];

                bn->addExtForce(force, location, true, true);
//                bn->getShapeNode(1)->getVisualAspect()->setColor(dart::Color::Black());
//                bn->createShapeNodeWith<VisualAspect>(mArrow);

                --mForceCountDown[i];
            }
        }
        SimWindow::timeStepping();
    }

protected:

    /// An arrow shape that we will use to visualize applied forces
    std::shared_ptr<ArrowShape> mArrow;

    /// The pendulum that we will be perturbing
    SkeletonPtr mCartPole;

    /// Number of iterations before clearing a force entry
    std::vector<int> mForceCountDown;

    /// Whether a force should be applied in the positive or negative direction
    bool mPositiveSign;

    /// True if 1-9 should be used to apply a body force. Otherwise, 1-9 will be
    /// used to apply a joint torque.
    bool mBodyForce;

    /// True if doing PD Control to desired dofs
    bool mPDControl;

    /// gain of PD control
    double Kp, Kd;
};


Simulation::
Simulation()
{

}

void Simulation::Init(){
    std::shared_ptr<VPC::World> world = std::make_shared<VPC::World>(1);
    world->Initialize();
    world->GetWorld()->setTimeStep(0.002);

    // Create a window for rendering the world and handling user input
    MyWindow window(world->GetWorld());

    // Print instructions

    // Initialize glut, initialize the window, and begin the glut event loop
    // glutInit(&argc, argv);
    int argc = 1;
    char *argv[]={};
    glutInit(&argc, argv);
    window.initWindow(1080, 810, "CartPole");
    glutMainLoop();
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
            .def("Init",&Simulation::Init);
}


