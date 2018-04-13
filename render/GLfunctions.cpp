#include "GLfunctions.h"
#include <assimp/cimport.h>
#include <iostream>
#include "GLIncludes.h"
namespace VPC {
namespace GUI {
static GLUquadricObj *quadObj;
static void initQuadObj(void)
{
    quadObj = gluNewQuadric();
    if(!quadObj)
        // DART modified error output
        std::cerr << "OpenGL: Fatal Error in DART: out of memory." << std::endl;
}
#define QUAD_OBJ_INIT { if(!quadObj) initQuadObj(); }

void
DrawPoint(const Eigen::Vector3d& p0,const Eigen::Vector4d& color,double size)
{
    glColor4d(color[0],color[1],color[2],color[3]);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3d(p0[0],p0[1],p0[2]);
    glEnd();
}
void
DrawLine(const Eigen::Vector3d& p0,const Eigen::Vector3d& p1,const Eigen::Vector4d& color,double width)
{
    glColor4d(color[0],color[1],color[2],color[3]);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex3d(p0[0],p0[1],p0[2]);
    glVertex3d(p1[0],p1[1],p1[2]);
    glEnd();
}
void
DrawSphere(const Eigen::Vector3d& p,double r,const Eigen::Vector4d& color, int slices, int stacks)
{
    QUAD_OBJ_INIT;
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    glColor4d(color[0],color[1],color[2],color[3]);
    glPushMatrix();
    glTranslated(p[0],p[1],p[2]);
    gluSphere(quadObj, r, 16, 16);
    glPopMatrix();
}

void
DrawCube(const Eigen::Isometry3d& T,const Eigen::Vector3d& _size,const Eigen::Vector4d& color)
{
    glColor4d(color[0],color[1],color[2],color[3]);
    glPushMatrix();
    glMultMatrixd(T.data());
    glScaled(_size(0), _size(1), _size(2));

    

    // Code taken from glut/lib/glut_shapes.c
    static GLfloat n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;
    GLfloat size = 1;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    glPopMatrix();
}




void
DrawCylinder(const Eigen::Isometry3d& T,double _radius, double _height,const Eigen::Vector4d& color, int slices, int stacks)
{
    glColor4d(color[0],color[1],color[2],color[3]);
    glPushMatrix();
    glMultMatrixd(T.data());
    glTranslated(0.0, 0.0, -0.5*_height);

    QUAD_OBJ_INIT;
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);

    gluCylinder(quadObj, _radius, _radius, _height, slices, stacks);
    glPopMatrix();
}


void
DrawArrow3D(const Eigen::Vector3d& _pt, const Eigen::Vector3d& _dir,
            const double _length, const double _thickness,const Eigen::Vector4d& color,
            const double _arrowThickness)
{
    glColor4d(color[0],color[1],color[2],color[3]);
    Eigen::Vector3d normDir = _dir;
    normDir.normalize();

    double arrowLength;
    if (_arrowThickness == -1)
    arrowLength = 4*_thickness;
    else
    arrowLength = 2*_arrowThickness;

    // draw the arrow body as a cylinder
    GLUquadricObj *c;
    c = gluNewQuadric();
    gluQuadricDrawStyle(c, GLU_FILL);
    gluQuadricNormals(c, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(_pt[0], _pt[1], _pt[2]);
    glRotated(acos(normDir[2])*180/M_PI, -normDir[1], normDir[0], 0);
    gluCylinder(c, _thickness, _thickness, _length-arrowLength, 16, 16);

    glPopMatrix();

    gluDeleteQuadric(c);
}

void
DrawCircle2D(float _x, float _y, const std::string& _s,const Eigen::Vector4d& color)
{
    
    // draws text on the screen
    GLint oldMode;
    glGetIntegerv(GL_MATRIX_MODE, &oldMode);
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(0,0,0,1.0);

    glRasterPos2f(_x-0.005*_s.length(), _y-0.005);
    
    unsigned int length = _s.length();
    for (unsigned int c = 0; c < length; c++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, _s.at(c) );
    }  
    glPushMatrix();
    glColor4d(color[0],color[1],color[2],0.4);

    glTranslatef(_x,_y,0);
    glutSolidSphere(0.025,40,40);
    glPopMatrix();
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(oldMode);
}
void
DrawStringOnScreen(float _x, float _y, const std::string& _s,bool _bigFont,const Eigen::Vector3d& color)
{
    glColor3f(color[0],color[1],color[2]);
	
    // draws text on the screen
    GLint oldMode;
    glGetIntegerv(GL_MATRIX_MODE, &oldMode);
    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(_x, _y);
    unsigned int length = _s.length();
    for (unsigned int c = 0; c < length; c++) {
    if (_bigFont)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, _s.at(c) );
    else
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, _s.at(c) );
    }  
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(oldMode);
}


}
}