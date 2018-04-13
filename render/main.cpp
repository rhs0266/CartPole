#include "SimWindow.h"
#include "GL/glut.h"
using namespace VPC;
using namespace VPC::GUI;

int main(int argc,char** argv)
{
	std::string record_path;
	if(argc==2)
		record_path = argv[1];
	else
		record_path = "../output/";

	if(record_path.back()!='/')
		record_path.append("/");
	SimWindow simwindow(record_path);
	glutInit(&argc, argv);
	simwindow.InitWindow(960,960,"Render");
	glutMainLoop();
}