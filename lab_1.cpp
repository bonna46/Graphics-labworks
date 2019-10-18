#include <GL/gl.h>
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <cmath >

static int slices = 16;
static int stacks = 16;
int ax=0,ay=0,az=0,zm=1;
int xx=0,yy=0,zz=0;


/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


void circle(double r,double a,double b, double c)
{

    double x=r,y=r;
    glBegin(GL_POLYGON);
    glColor3f(a,b,c);
    for(int i=0; i<=360; i++)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, .2);
    }
    glEnd();

}


void tire(double r,double d,double a,double b, double c)
{

    double x=r,y=r;
    glBegin(GL_POLYGON);
    glColor3f(a,b,c);
    for(int i=0; i<=360; i++)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, .2);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(a,b,c);
    for(int i=360; i>=0; i--)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, -.2);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(a,b,c);
    for(int i=0; i<360; i++)
    {
        double rd1=(2*3.1416*i)/360;
        double rd2=(2*3.1416*(i+1))/360;
        double x1=x*cosf(rd1);
        double y1=y*sinf(rd1);
        double x2=x*cosf(rd2);
        double y2=y*sinf(rd2);
        glVertex3f(x1,y1,d);
        glVertex3f(x1,y1,-d);
        glVertex3f(x2,y2,-d);
        glVertex3f(x2,y2,d);
    }
    glEnd();

}


void pyramid()
{
    glBegin(GL_TRIANGLES);
    glColor3f( 0.3f, 0.4f, 0.5f);
    glVertex3f(0,2,0);
    glVertex3f(-1,-2,1);
    glVertex3f(1,-2,1);

    glColor3f( 0.3f, 0.6f, 0.5f);
    glVertex3f(0,2,0);
    glVertex3f(1,-2,1);
    glVertex3f(1,-2,-1);

    glColor3f( 0.9f, 0.4f, 0.5f);
    glVertex3f(0,2,0);
    glVertex3f(-1,-2,-1);
    glVertex3f(-1,-2,1);

    glColor3f( 0.2f, 0.4f, 0.5f);
    glVertex3f(0,2,0);
    glVertex3f(1,-2,-1);
    glVertex3f(-1,-2,-1);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f( 0.2f, 0.2f, 0.5f);
    glVertex3f(1,-2,1);
    glVertex3f(-1,-2,1);
    glVertex3f(-1,-2,-1);
    glVertex3f(1,-2,-1);
    glEnd();
}

void pillar(double a, double b, double c)
{
    glLineWidth(9.0);
    glBegin(GL_LINE_STRIP);
    glColor3f( a,b,c);
    glVertex3f(0,0,0);
    glVertex3f(-3.5, -3.5, 3.5);
    glVertex3f(-3,-3,3);
    glVertex3f(3, -3, 3);
    glVertex3f(3.5, -3.5, 3.5);
    glVertex3f(0,0,0);
    glEnd();

    glLineWidth(5.0);
    glBegin(GL_LINE_STRIP);
    glColor3f( a,b,c);
    glVertex3f(-1, -1, 1);
    glVertex3f(1,-1,1);
    glVertex3f(2,-2,2);
    glVertex3f(-2, -2, 2);
    glEnd();
}

void line(double a,double b, double c)
{
    glBegin(GL_POLYGON);
    glColor3f( a,b,c);
    glVertex3f(0,1,.2);
    glVertex3f(0,0,.2);
    glVertex3f(4,0,.2);
    glVertex3f(4.5,0.5,.2);
    glVertex3f(4,1,.2);
    glVertex3f(0,1,.2);

    glColor3f( a,b,c);
    glVertex3f(0,1,.2);
    glVertex3f(4,1,.2);
    glVertex3f(4.5,0.5,.2);
    glVertex3f(4,0,.2);
    glVertex3f(0,0,.2);
    glVertex3f(0,1,.2);
    glEnd();
}


static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    {
        glPushMatrix();
        glScaled(zm, zm, zm);
        glTranslated(xx, yy, zz);
        glRotated(ax, 1, 0, 0);
        glRotated(ay, 0, 1, 0);
        glRotated(az, 0, 0, 1);


        glTranslated(0,2,0);
        glScaled(1,5,1);
        pillar(.85, .55, .13);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.85, .55, .13);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.85, .55, .13);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.85, .55, .13);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        glScaled(1.25,0.2,1.25);
        tire(1,.5,.22, .78, .77);
        glTranslated(0,-2,0);

        glTranslated(.8,1.5,0);
        glScaled(1.5,1,1);
        line(.53, .82,.20);
        glScaled(0.666,1,1);
        glTranslated(-.4,-1.5,0);



        glTranslated(0,2.9, 0);
        glRotated(118, 0, 0, 1);
        glScaled(1.5,1,1);
        line(.97, .29,.192);
        glScaled(0.666,1,1);
        glRotated(-118, 0, 0, 1);
        // glTranslated(0,-2.9, 0);



        glTranslated(-.1,-0.7, 0);
        glRotated(60, 0, 0, 1);
        glTranslated(-.7,.33, 0);
        glScaled(-1.5,-1,1);
        line(.91, .89,.22);

        glScaled(-0.666,-1,1);
        glTranslated(.7,-.33, 0);
        glRotated(-60, 0, 0, 1);


        glTranslated(-.2,-.2,.72);
        tire(1,.2,.18, .87, .84);
        glTranslated(0,0,.5);
        tire(.2,.3,.6, .8, .6);
        glPopMatrix();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(0, 0, 40, 0, 0, 0, 0, 1, 0);
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case '.':
        exit(0);
        break;

    case 'l':
        ax+=1.5;
        break;

    case 'a':
        ax-=1.5;
        break;

    case 'k':
        ay+=1.5;
        break;

    case 's':
        ay-=1.5;
        break;

    case 'j':
        az+=1.5;
        break;

    case 'd':
        az-=1.5;
        break;

    case '+':
        zm+=1;
        break;

    case '-':
        zm-=1;
        break;

    case 'p':
        xx+=1.5;
        break;

    case 'q':
        xx-=1.5;
        break;

    case 'o':
        yy+=1.5;
        break;

    case 'w':
        yy-=1.5;
        break;

    case 'i':
        zz+=1.5;
        break;

    case 'e':
        zz-=1.5;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitWindowSize(800,700);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glutMainLoop();

    return EXIT_SUCCESS;
}
