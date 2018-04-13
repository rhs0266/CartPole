#include "DartInterface.h"
#include "GLfunctions.h"
#include "GL/glut.h"
using namespace dart::dynamics;
namespace VPC {
namespace GUI {
namespace DartInterface {
void DrawShape(const Eigen::Isometry3d& T,
	const dart::dynamics::Shape* shape,
	const Eigen::Vector4d& color);
void DrawFrame(const Eigen::Isometry3d& T);



void DrawSkeleton(
	const dart::dynamics::SkeletonPtr& skel,
	const Eigen::Vector4d& color)
{
	for(int i=0;i<skel->getNumBodyNodes();i++)
	{
		auto bn = skel->getBodyNode(i);
		auto shapeNodes = bn->getShapeNodesWith<VisualAspect>();

		// const Eigen::Isometry3d& T = shapeNodes[0]->getTransform();
		const Eigen::Isometry3d& T = bn->getTransform();
		
		DrawShape(T,shapeNodes[0]->getShape().get(),color);
		// glDisable(GL_DEPTH_TEST);
		// DrawFrame(T);
		// glEnable(GL_DEPTH_TEST);
	}	
	glDisable(GL_DEPTH_TEST);
	for(int i = 1;i<skel->getNumJoints();i++)
	{
		auto parent = skel->getJoint(i)->getParentBodyNode();
		Eigen::Isometry3d T;
		T.setIdentity();
		if(parent!=nullptr)
			T = parent->getTransform();
		T = T*skel->getJoint(i)->getTransformFromParentBodyNode();
		DrawSphere(T.translation(),0.03,Eigen::Vector4d(0.8,0,0,1));
		// DrawFrame(T);
	}
	glEnable(GL_DEPTH_TEST);
}

void DrawShape(const Eigen::Isometry3d& T,
	const dart::dynamics::Shape* shape,
	const Eigen::Vector4d& color)
{

	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	if (shape->is<BoxShape>())
	{
		const auto* box = dynamic_cast<const BoxShape*>(shape);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    	DrawCube(T,box->getSize(),color);
	}
	glDisable(GL_LIGHTING);
}
void DrawFrame(const Eigen::Isometry3d& T)
{
	Eigen::Vector3d p0,p1;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glMultMatrixd(T.data());
	p0 = Eigen::Vector3d::Zero();

	p1 = 0.1*Eigen::Vector3d::UnitX();
	DrawLine(p0,p1,Eigen::Vector4d(1,0,0,1),2.0);
	p1 = 0.1*Eigen::Vector3d::UnitY();
	DrawLine(p0,p1,Eigen::Vector4d(0,1,0,1),2.0);
	p1 = 0.1*Eigen::Vector3d::UnitZ();
	DrawLine(p0,p1,Eigen::Vector4d(0,0,1,1),2.0);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}



}
}
}