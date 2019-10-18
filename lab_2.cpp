#include <GL/gl.h>
#include <windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <cmath >
using namespace std;

#define pi    acos(-1)

static int slices = 16;
static int stacks = 16;
int ax=0,ay=0,az=0,zm=1;
int xx=0,yy=0,zz=0;
double px=1,py=1,pz=1;
double pa=0,pb=0,pc=0;
double ya=0,yb=0,yaw_a=0,yaw_b=0,pp=0;
double ch_b=1,f=1,ch_a=0;
double eye_x=1,eye_y=-1,eye_z=45;
double fx,fy,fz;
double width_p,height_p;

void ownScaled(GLfloat a, GLfloat b, GLfloat c)
{
    GLfloat m[]=
    {
        a,0,0,0,
        0,b,0,0,
        0,0,c,0,
        0,0,0,1
    };
    glMatrixMode(GL_MODELVIEW);
    glMultMatrixf(m);

}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

   // glViewport(0, 0, width/1.3, height/1.3);
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
        glVertex3f(xx, yy, d);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(a,b,c);
    for(int i=360; i>=0; i--)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, -d);
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

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.0f, 0.0f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    {

        gluLookAt(eye_x, eye_y, eye_z,px, py, pz,ch_a, ch_b,0);

        glPushMatrix();
        glViewport(0, 0, width_p, height_p);
        glScaled(zm, zm, zm);
        glTranslated(xx, yy, zz);
        glRotated(ax, 1, 0, 0);
        glRotated(ay, 0, 1, 0);
        glRotated(az, 0, 0, 1);


        glTranslated(0,4,0);
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,-4,0);




        glTranslated(0,0,0);
        ownScaled(1,5,1);
        pillar(.63, .48, .48);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.63, .48, .48);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.63, .48, .48);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.63, .48, .48);
        glRotated(90,0,1,0);


        glScaled(1,.2,1);
        glRotated(a,0,0,1);
        glScaled(1,5.0,1);

        glTranslated(0,0,1);
        glScaled(1.25,0.2,1.25);
        tire(1,.9,.22, .78, .77);
        glTranslated(0,-2,-1);



        glTranslated(.8,1.5,1.8);
        glScaled(1.5,1,1);
        line(.18,.15,.46);
        glScaled(0.666,1,1);
        glTranslated(-.4,-1.5,-1.8);



        glTranslated(0,2.9, 1.8);
        glRotated(118, 0, 0, 1);
        glScaled(1.5,1,1);
        line(.18,.15,.46);
        glScaled(0.666,1,1);
        glRotated(-118, 0, 0, 1);
        glTranslated(-.4,-2.9,-1.8);


        glTranslated(-.65,1.8,1.8);
        glRotated(45, 0, 0, 1);
        glScaled(-1.6,-1,1);
        line(.18,.15,.46);

        glScaled(-0.6024,-1,1);
        glRotated(-45, 0, 0, 1);
        glTranslated(.65,-1.8,-1.8);

        glTranslated(0,1.98,2.2);
        tire(1,.2,.18, .87, .84);
        //tire(1,.2,1,1,1);
        glTranslated(0,0,.5);
        tire(.3,.3,.18,.15,.46);
        glTranslated(0,-1.98,-2.7);

        glPopMatrix();

         glPushMatrix();
         glViewport(width_p/4, 0, width_p, height_p);
        glScaled(zm, zm, zm);
        glTranslated(xx, yy, zz);
        glRotated(ax, 1, 0, 0);
        glRotated(ay, 0, 1, 0);
        glRotated(az, 0, 0, 1);


         glTranslated(0,4,0);
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
        glutSolidSphere(1,slices,stacks);
        glTranslated(0,-4,0);


        glTranslated(0,0,0);
        ownScaled(1,5,1);
        pillar(.63, .48, .48);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.63, .48, .48);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.63, .48, .48);

        glTranslated(0,0,0);
        glRotated(90,0,1,0);
        pillar(.63, .48, .48);
        glRotated(90,0,1,0);


        glScaled(1,.2,1);
        glRotated(a,0,0,1);
        glScaled(1,5.0,1);

        glTranslated(0,0,1);
        glScaled(1.25,0.2,1.25);
        tire(1,.9,.22, .78, .77);
        glTranslated(0,-2,-1);



        glTranslated(.8,1.5,1.8);
        glScaled(1.5,1,1);
        line(.18,.15,.46);
        glScaled(0.666,1,1);
        glTranslated(-.4,-1.5,-1.8);



        glTranslated(0,2.9, 1.8);
        glRotated(118, 0, 0, 1);
        glScaled(1.5,1,1);
        line(.18,.15,.46);
        glScaled(0.666,1,1);
        glRotated(-118, 0, 0, 1);
        glTranslated(-.4,-2.9,-1.8);


        glTranslated(-.65,1.8,1.8);
        glRotated(45, 0, 0, 1);
        glScaled(-1.6,-1,1);
        line(.18,.15,.46);

        glScaled(-0.6024,-1,1);
        glRotated(-45, 0, 0, 1);
        glTranslated(.65,-1.8,-1.8);

        glTranslated(0,1.98,2.2);
        tire(1,.2,.18, .87, .84);
        //tire(1,.2,1,1,1);
        glTranslated(0,0,.5);
        tire(.3,.3,.18,.15,.46);
        glTranslated(0,-1.98,-2.7);
        glPopMatrix();





    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
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

    case 'X':
        xx+=1.5;
        break;

    case 'x':
        xx-=1.5;
        break;

    case 'Y':
        yy+=1.5;
        break;

    case 'y':
        yy-=1.5;
        break;

    case 'Z':
        zz+=1.5;
        break;

    case 'z':
        zz-=1.5;
        break;

    //for roll,pitch,yaw;
    case 'p':
        pa+=10;
        fy=py;
        fz=pz;
        py=((cos((pa*pi)/180)))-((sin((pa*pi)/180)));
        pz=((sin((pa*pi)/180)))+((cos((pa*pi)/180)));
        break;
    case 'w':
        pb+=10;
        fx=px;
        fz=pz;
        px=(cos((pb*pi)/180))+(sin((pb*pi)/180));
        pz=(cos((pb*pi)/180))-(sin((pb*pi)/180));
        break;
    case 'r':
        pc+=10;
        fy=py;
        fx=px;
        ch_a=(cos((pc*pi)/180))-((sin((pc*pi)/180)));
        ch_b=(cos((pc*pi)/180))+((sin((pc*pi)/180)));
        break;

    case 'q':
        pp+=10;
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
    //width*height;
    width_p=900,height_p=670;
    glutInitWindowSize(900,670);
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



glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//
// glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
//    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
