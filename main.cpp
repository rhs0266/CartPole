#include "engine/engine.h"
#include <dart/dart.hpp>
#include <dart/gui/gui.hpp>

const double default_height = 1.0; // m
const double default_width = 0.2;  // m
const double default_depth = 0.2;  // m

const double default_torque = 15.0; // N-m
const double default_force =  15.0; // N
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

class MyWindow : public dart::gui::SimWindow
{
public:

    /// Constructor
    MyWindow(WorldPtr world)
            : mPositiveSign(true),
              mBodyForce(false),
              mPDControl(false),
              Kp(default_Kp), Kd(default_Kd)
    {
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

    void changeDirection()
    {
        mPositiveSign = !mPositiveSign;
        if(mPositiveSign)
        {
            mArrow->setPositions(
                    Eigen::Vector3d(-default_height, 0.0, default_height / 2.0),
                    Eigen::Vector3d(-default_width / 2.0, 0.0, default_height / 2.0));
        }
        else
        {
            mArrow->setPositions(
                    Eigen::Vector3d(default_height, 0.0, default_height / 2.0),
                    Eigen::Vector3d(default_width / 2.0, 0.0, default_height / 2.0));
        }
    }

    void applyForce(std::size_t index)
    {
        if(index < mForceCountDown.size())
            mForceCountDown[index] = default_countdown;
    }

    void changeRestPosition(double delta)
    {
        // Lesson 2a
        for (std::size_t i=0;i < mCartPole->getNumDofs();i++){
            DegreeOfFreedomPtr dof = mCartPole->getDof(i);
            double newRestPosition = dof->getRestPosition() + delta;
            if(std::abs(newRestPosition) > 90.0 * M_PI / 180.0)
                newRestPosition = (newRestPosition > 0)? (90.0 * M_PI / 180.0) : -(90.0 * M_PI / 180.0);
            dof->setRestPosition(newRestPosition);
        }
        std::cout << "rest position : " << mCartPole->getDof(4)->getRestPosition() << std::endl;
        mCartPole->getDof(0)->setRestPosition(0.0);
        mCartPole->getDof(2)->setRestPosition(0.0);
    }

    void changeStiffness(double delta)
    {
        // Lesson 2b
        for (std::size_t i=0;i < mCartPole->getNumDofs();i++){
            DegreeOfFreedomPtr dof = mCartPole->getDof(i);
            double stiffness = dof->getSpringStiffness() + delta;
            if (stiffness < 0.0) stiffness = 0.0;
            dof->setSpringStiffness(stiffness);
        }
        std::cout << "stiffness: " << mCartPole->getDof(4)->getSpringStiffness() << std::endl;

        mCartPole->getDof(0)->setSpringStiffness(0.0);
        mCartPole->getDof(2)->setSpringStiffness(0.0);
    }

    void changeDamping(double delta)
    {
        // Lesson 2c
        for(size_t i = 0; i < mCartPole->getNumDofs(); ++i)
        {
            DegreeOfFreedom* dof = mCartPole->getDof(i);
            double damping = dof->getDampingCoefficient() + delta;
            if(damping < 0.0)
                damping = 0.0;
            dof->setDampingCoefficient(damping);
        }
        std::cout << "damping: " << mCartPole->getDof(4)->getDampingCoefficient() << std::endl;
    }

    void changeKp(double delta){
        Kp += delta;
        if (Kp<0) Kp=0;
        std::cout << "Proportional gain : " << Kp << std::endl;
    }

    void changeKd(double delta){
        Kd += delta;
        if (Kd<0) Kd=0;
        std::cout << "Derivative gain : " << Kd << std::endl;
    }

    void printDofs(){
        for (std::size_t i=0;i<mCartPole->getNumDofs();i++){
            std::cout << "Dof #" << i << " : " << mCartPole->getDof(i)->getPosition() / M_PI * 180.0 << std::endl;
        }
    }

    /// Handle keyboard input
    void keyboard(unsigned char key, int x, int y) override
    {
        switch(key) {
            case '-':
                changeDirection();
                break;

            case '1':
                applyForce(0);
                break;
            case '2':
                applyForce(1);
                break;
            case '3':
                applyForce(2);
                break;
            case '4':
                applyForce(3);
                break;
            case '5':
                applyForce(4);
                break;
            case '6':
                applyForce(5);
                break;
            case '7':
                applyForce(6);
                break;
            case '8':
                applyForce(7);
                break;
            case '9':
                applyForce(8);
                break;
            case '0':
                applyForce(9);
                break;

            case 'q':
                changeRestPosition(delta_rest_position);
                break;
            case 'a':
                changeRestPosition(-delta_rest_position);
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

    void timeStepping() override
    {
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

int main(int argc, char* argv[])
{
   // Create a world and add the pendulum to the world
    std::shared_ptr<VPC::World> world = std::make_shared<VPC::World>(1);
    world->Initialize();
    world->GetWorld()->setTimeStep(0.0001);

    // Create a window for rendering the world and handling user input
    MyWindow window(world->GetWorld());

    // Print instructions

    // Initialize glut, initialize the window, and begin the glut event loop
    glutInit(&argc, argv);
    window.initWindow(1280, 960, "CartPole");
    glutMainLoop();
}