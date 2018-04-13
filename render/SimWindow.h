#ifndef __VPC_GUI_SIM_WINDOW_H__
#define __VPC_GUI_SIM_WINDOW_H__
#include "Camera.h"
#include "GLUTWindow.h"
#include "GLfunctions.h"
#include "engine/engine.h"

namespace VPC {
namespace GUI {
class SimWindow : public VPC::GUI::GLUTWindow
{
public:
	SimWindow(const std::string& path);

//	std::shared_ptr<VPC::StateMachine> 		mFSM;
	std::shared_ptr<VPC::World>				mWorld;
	std::vector<std::shared_ptr<VPC::Record>> mRecords;
	int 						mFrame;

	bool 						mIsPlay;
	bool 						mIsDrag;
	
	bool						mCapture;	
	void LoadFromFolder(const std::string& path);


	Eigen::VectorXd dp;
protected:
	void GoToFrame();
	void Display() override;
	void Keyboard(unsigned char key,int x,int y) override;
	void Mouse(int button, int state, int x, int y) override;
	void Motion(int x, int y) override;
	void Reshape(int w, int h) override;
	void Timer(int value) override;
	void Screenshot();
};
}
}
#endif