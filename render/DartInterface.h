#ifndef __VPC_GUI_DART_INTERFACE_H__
#define __VPC_GUI_DART_INTERFACE_H__
#include "dart/dart.hpp"
namespace VPC {
namespace GUI {
namespace DartInterface {
	
	void DrawSkeleton(
		const dart::dynamics::SkeletonPtr& skel,
		const Eigen::Vector4d& color = Eigen::Vector4d(0.8,0.8,0.8,1));

}
}
}
#endif