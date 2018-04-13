#ifndef __VPC_GL_INCLUDES_H__
#define __VPC_GL_INCLUDES_H__
#if defined(__linux__)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#elif defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
    #include <Glut/glut.h>
#endif
#endif