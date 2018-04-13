#include "SimWindow.h"
#include "DartInterface.h"
#include "dart/external/lodepng/lodepng.h"
#include <boost/filesystem.hpp>
#include "GLIncludes.h"

using namespace dart::simulation;
using namespace dart::dynamics;
namespace VPC {
namespace GUI {
SimWindow::
SimWindow(const std::string& path)
	:GLUTWindow(),mIsDrag(false),mIsPlay(false),mFrame(0),mCapture(false)
{
	mWorld = std::make_shared<VPC::World>(1);
	mWorld->Initialize();
	LoadFromFolder(path);
	dp = mWorld->GetCharacter()->GetSkeleton()->getPositions();
	dp.setZero();
//	mFSM = std::make_shared<VPC::StateMachine>(mWorld);
	std::cout<<"record size : "<<mRecords.size()<<std::endl;
	mDisplayTimeout = 33;
}

void
SimWindow::
Display() 
{
	if(mFrame<0)
		mFrame = mRecords.size()-1;
	if(mFrame>mRecords.size()-1)
		mFrame = 0;	
	 mRecords[mFrame]->Get(mWorld->GetWorld(),mWorld->GetCharacter());
//    mRecords[mFrame]->Get(mWorld->GetWorld(),mWorld->GetCharacter(),mFSM);
	mWorld->GetCharacter()->GetSkeleton()->setPositions(mWorld->GetCharacter()->GetSkeleton()->getPositions()+dp);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	initLights();
	mCamera->Apply();
	glDisable(GL_LIGHTING);
	// glLineWidth(2.0);
	// DrawLine(Eigen::Vector3d(0,0,0),Eigen::Vector3d(100,0,0),Eigen::Vector4d(1,0,0,1));
	// DrawLine(Eigen::Vector3d(0,0,0),Eigen::Vector3d(0,100,0),Eigen::Vector4d(0,1,0,1));
	// DrawLine(Eigen::Vector3d(0,0,0),Eigen::Vector3d(0,0,100),Eigen::Vector4d(0,0,1,1));
	// glLineWidth(1.0);
	glColor3f(0,0,0);
	// glLineWidth(1.0);
	// glBegin(GL_LINES);
	// for(double z = -0.2;z<=0.2;z+=0.1)
	// {
	// 	double z = 0.0;
	// 	for(double x=-2.0;x<=2.0;x+=0.1)
	// 	{
	// 		glVertex3f(x,-1.0,z);
	// 		glVertex3f(x,3.0,z);
	// 	}
	// 	for(double y=-1.0;y<=3.0;y+=0.1)
	// 	{
	// 		glVertex3f(-2.0,y,z);
	// 		glVertex3f(2.0,y,z);
	// 	}
	// }
	// glEnd();

	glBegin(GL_LINES);
	{
		glLineWidth(5.0);
		double z = 0.0;
		for(double x=-100.0;x<=100.0;x+=10.0)
		{
			glVertex3f(x,z,-100.0);
			glVertex3f(x,z,100.0);
		}
		for(double y=-100.0;y<=100.0;y+=10.0)
		{
			glVertex3f(-100.0,z,y);
			glVertex3f(100.0,z,y);
		}
		glLineWidth(1.0);
	}
	glEnd();
	
//	Eigen::Vector4d color;
//	int state_count = 0;
//	color = Eigen::Vector4d(0,0,0,1);
//	if(state_count ==mFSM->GetState())
//		color = Eigen::Vector4d(1.0,0,0,1);
//	VPC::GUI::DrawCircle2D(0.1,0.9,std::to_string(0),color);
//	state_count++;
//	color = Eigen::Vector4d(0,0,0,1);
//	if(state_count ==mFSM->GetState())
//		color = Eigen::Vector4d(1.0,0,0,1);
//	VPC::GUI::DrawCircle2D(0.2,0.9,std::to_string(1),color);
//	state_count++;
//	color = Eigen::Vector4d(0,0,0,1);
//	if(state_count ==mFSM->GetState())
//		color = Eigen::Vector4d(1.0,0,0,1);
//	VPC::GUI::DrawCircle2D(0.2,0.8,std::to_string(2),color);
//	state_count++;
//	color = Eigen::Vector4d(0,0,0,1);
//	if(state_count ==mFSM->GetState())
//		color = Eigen::Vector4d(1.0,0,0,1);
//	VPC::GUI::DrawCircle2D(0.1,0.8,std::to_string(3),color);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	VPC::GUI::DrawStringOnScreen(0.9,0.1,std::to_string(mFrame),true,Eigen::Vector3d(0,0,0));
	VPC::GUI::DartInterface::DrawSkeleton(mWorld->GetWorld()->getSkeleton(0),Eigen::Vector4d(0,0,0,1));
	if(mCapture)
		Screenshot();
	glutSwapBuffers();
}
void
SimWindow::
GoToFrame()
{
	int n;
	std::cout<<"Go to : ";
	std::cin>>n;
	mFrame = n;
}
void
SimWindow::
Keyboard(unsigned char key,int x,int y) 
{

	switch(key)
	{
		case '1' : dp[0] += 0.1; break;
		case '2' : dp[1] += 0.1; break;
		case '3' : dp[2] += 0.1; break;
		case '4' : dp[3] += 0.1; break;
		case '5' : dp[4] += 0.1; break;
		case '6' : dp[5] += 0.1; break;
		case '7' : dp[12] += 0.1; break;
		case '8' : dp[13] += 0.1; break;
		case '9' : dp[14] += 0.1; break;
		case '0' : dp[15] += 0.1; break;
		case 't' : GoToFrame();break;
		case 'C' : mCapture =!mCapture;break;
		case ' ' : mIsPlay =!mIsPlay;break;
		case 'r' : mFrame = 0;break;
		case '[' : mFrame--;break;
		case ']' : mFrame++;break;
		case 27: exit(0);break;
		default : break;
	}

	std::cout<<dp.transpose()<<std::endl;

	glutPostRedisplay();
}
void
SimWindow::
Mouse(int button, int state, int x, int y) 
{
	if (state == GLUT_DOWN)
	{
		mIsDrag = true;
		mMouseType = button;
		mPrevX = x;
		mPrevY = y;
		if(button == 3)
			mCamera->Pan(0,0,0,-10);
		else if(button == 4)
			mCamera->Pan(0,0,0,10);
	}
	else
	{
		mIsDrag = false;
		mMouseType = 0;
	}

	glutPostRedisplay();
}
void
SimWindow::
Motion(int x, int y) 
{
	if (!mIsDrag)
		return;

	if (mMouseType == GLUT_LEFT_BUTTON)
		mCamera->Rotate(x,y,mPrevX,mPrevY);
	else if (mMouseType == GLUT_RIGHT_BUTTON){
		
		mCamera->Translate(x,y,mPrevX,mPrevY);
	}

	mPrevX = x;
	mPrevY = y;
	glutPostRedisplay();
}
void
SimWindow::
Reshape(int w, int h) 
{
	glViewport(0, 0, w, h);
	mCamera->Apply();
}
void
SimWindow::
Timer(int value) 
{
	if(mIsPlay){
		double curr_time = mRecords[mFrame]->t;
		int i;
		for(i =0;i<mDisplayTimeout;i++)
		{
			if(mFrame+i<mRecords.size()){
				double next_time = mRecords[mFrame+i]->t;
				if(next_time-curr_time>0.033)
					break;
			}
		}
		mFrame+=i;
	}
		
	glutPostRedisplay();
	glutTimerFunc(mDisplayTimeout, TimerEvent,1);
}


void
SimWindow::
LoadFromFolder(const std::string& path)
{
	int count = 0;
	while(true)
	{
		std::string real_path = path+std::to_string(count++);
		if(!boost::filesystem::exists(real_path))
			break;

		mRecords.push_back(std::make_shared<VPC::Record>());
		mRecords.back()->Read(real_path);		
	}
}

void
SimWindow::
Screenshot()
{
	static int count = 0;
	const char directory[16] = "../frames";
	const char fileBase[8] = "Capture";
	char fileName[32];

	boost::filesystem::create_directories(directory);
	std::snprintf(	fileName, sizeof(fileName), "%s%s%s%.4d.png",
					directory, "/", fileBase, count++);
	int tw = glutGet(GLUT_WINDOW_WIDTH);
	int th = glutGet(GLUT_WINDOW_HEIGHT);

	glReadPixels(0, 0,  tw, th, GL_RGBA, GL_UNSIGNED_BYTE, &mScreenshotTemp[0]);

	for (int row = 0; row < th; row++)
	{
		memcpy(&mScreenshotTemp2[row * tw * 4],
				&mScreenshotTemp[(th - row - 1) * tw * 4], tw * 4);
	}

	unsigned result = lodepng::encode(fileName, mScreenshotTemp2, tw, th);

	if (result)
	{
		std::cout << "lodepng error " << result << ": "
					<< lodepng_error_text(result) << std::endl;
		return;
	}
	else
	{
		std::cout << "wrote screenshot " << fileName <<std::endl;
		return;
	}
}
}
}