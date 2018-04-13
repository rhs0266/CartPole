#ifndef __VPC_GUI_GL_FUNCTIONS_H__
#define __VPC_GUI_GL_FUNCTIONS_H__
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/Geometry>
#include <assimp/cimport.h>
#include <assimp/scene.h>
namespace VPC {
namespace GUI {
	
	void DrawPoint(const Eigen::Vector3d& p0,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1),double size = 1.0);
	void DrawLine(const Eigen::Vector3d& p0,const Eigen::Vector3d& p1,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1),double width = 1.0);
	void DrawSphere(const Eigen::Vector3d& p,double r,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1), int slices = 16, int stacks = 16);
	void DrawCube(const Eigen::Isometry3d& T,const Eigen::Vector3d& size,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1));
	void DrawCylinder(const Eigen::Isometry3d& T,double _radius, double _height,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1), int slices = 16, int stacks = 16);
	void DrawArrow3D(const Eigen::Vector3d& _pt, const Eigen::Vector3d& _dir,
                 const double _length, const double _thickness,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1),const double _arrowThickness = -1);

	void DrawCircle2D(float _x, float _y, const std::string& _s,const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1));
	void DrawStringOnScreen(float _x, float _y, const std::string& _s,bool _bigFont,const Eigen::Vector3d& color=Eigen::Vector3d(0.8,0.8,0.8));
}
}
#endif