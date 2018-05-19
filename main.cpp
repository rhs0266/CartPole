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

/**
 * @mainpage hello~
 * @brief main.cpp
 * @details it is main~
 */

class MyWindow : public dart::gui::SimWindow {
public:

    /// Constructor
    MyWindow(std::shared_ptr<VPC::World> world)
            : mPositiveSign(true),
              mBodyForce(false),
              mPDControl(false),
              Kp(default_Kp), Kd(default_Kd) {
        setWorld(world->GetWorld());

        mWorld = world;

        // Find the Skeleton named "CartPole" within the World
        mSimBiCon = world->GetWorld()->getSkeleton("SimBiCon");

        // Make sure that the pendulum was found in the World
        // assert(mSimBiCon != nullptr);


    }

    void printDofs() {
        for (std::size_t i = 0; i < mSimBiCon->getNumDofs(); i++) {
            std::cout << "Dof #" << i << " : " << mSimBiCon->getDof(i)->getPosition() / M_PI * 180.0 << std::endl;
        }
    }

    /// Handle keyboard input
    void keyboard(unsigned char key, int x, int y) override {
        switch (key) {
            case 'k':
                mWorld->GetCharacter()->GetSkeleton()->setPosition(4,1.1);
//                std::cout << mWorld->GetCharacter()->GetSkeleton()->getVelocity(4) << std::endl;
//                if (mWorld->GetCharacter()->GetSkeleton()->getPosition(4) >= -0.2207)
                SimWindow::timeStepping();
                break;

            case 'p':
                printDofs();
                break;

            default:
                SimWindow::keyboard(key, x, y);
        }
    }

    void timeStepping() override {
//        std::cout << mWorld->GetCharacter()->GetSkeleton()->getPositions() << std::endl << std::endl;
//        SimWindow::timeStepping();
    }

protected:

    /// An arrow shape that we will use to visualize applied forces
    std::shared_ptr<ArrowShape> mArrow;

    /// The world pointer
    std::shared_ptr<VPC::World> mWorld;

    /// The pendulum that we will be perturbing
    SkeletonPtr mSimBiCon;

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

int main(int argc, char *argv[]) {
    // Create a world and add the pendulum to the world
    std::shared_ptr<VPC::World> world = std::make_shared<VPC::World>(1);
    world->Initialize();
    world->GetWorld()->setTimeStep(0.002);
    std::cout << world->GetCharacter()->GetSkeleton()->getPositions() << std::endl;


    // Create a window for rendering the world and handling user input
    MyWindow window(world);

    // Print instructions

    // Initialize glut, initialize the window, and begin the glut event loop
    glutInit(&argc, argv);
    window.initWindow(1080, 810, "SimBiCon");
    glutMainLoop();
}