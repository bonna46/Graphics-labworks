#include <GL/gl.h>
#include <windows.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "RGBpixmap.cpp"


#define PI 3.1415926535897932384626433832795
#include <stdlib.h>
#include<vector>
#include <cmath >
#include<stdio.h>
using namespace std;

#define pi    acos(-1)

static int slices = 16;
static int stacks = 16;
int ax=0,ay=0,az=0;
GLfloat zm=1.0;
int xx=0,yy=0,zz=0;
double px=1,py=1,pz=1;
double qa=1,qb=1,qc=1,qa1=1,qb1=1,qc1=1;
double pa=0,pb=0,pc=0;
double ya=0,yb=0,yaw_a=0,yaw_b=0,pp=0;
double ch_b=1,f=1,ch_a=0;
double eye_x=2,eye_y=10,eye_z=60;
double fx,fy,fz;
int on=1,on1=1,dfon=1;
double df0=1.0,df1=0.0,df2=0.0;
double sp=91;

GLint width=1000,height=800;
float Near=0.01, Far=60.0,fova = 2.0;
RGBpixmap pix[6];

GLfloat mat_ambient[]    = {0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_diffuse[]    = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat mat_specular[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat high_shininess[] = { 10.0f };

GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat light_position[] = { -10.0f, 10.0f,10.0f, 0.0f };

GLfloat light_ambient1[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_diffuse1[]  = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat light_specular1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat light_position1[] = { -10.0f, 10.0f, -10.0f, 0.0f };

GLfloat light_ambient2[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_diffuse2[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_position2[] = { -5.0f, 10.0f, 50.0f, 0.0f };
GLfloat spot_direction[]  = { 0, 0, 0 };



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



static void snow_recur(int level, GLdouble t, GLdouble b, GLdouble l, GLdouble r)
{
    glVertex3d(l, t, 0.01);        /// left top vertex
    glVertex3d(l, b, 0.01);        /// left bottom vertex
    glVertex3d(r, b, 0.01);        /// right bottom vertex
    glVertex3d(r, t, 0.01);        /// right top vertex

    if (level <= 0)
        return;

    GLdouble hi = (t-b)/4.0;
    GLdouble wi = (r-l)/4.0;

    GLdouble y[2] = {t,b};
    GLdouble x[2] = {l,r};
    for (int i=0; i<2; i++)
    {
    for (int j=0; j<2; j++)
        {
        snow_recur(level-1,y[i]+hi,y[i]-hi,x[j]-wi,x[j]+wi);}
    }
}

static void snowflake(int level)
{
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        snow_recur(level, 0.2, -0.2, -0.3, 0.3);
    glEnd();
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
   // gluOrtho2D(lftx, rhtx, lfty, rhty);

//    float aspectR = (float)WindowSizX / WindowSizY;
//gluPerspective(fova,aspectR, Near, Far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void circle(double r)
{

    double x=r,y=r;
    glBegin(GL_POLYGON);
    for(int i=0; i<=360; i++)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, .2);
    }
    glEnd();

}


void tire(double r,double d)
{

    double x=r,y=r;
    glBegin(GL_POLYGON);
    for(int i=0; i<=360; i++)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, d);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(int i=360; i>=0; i--)
    {
        double rad=(2*3.1416*i)/360;
        double xx=x*cosf(rad);
        double yy=y*sinf(rad);
        glVertex3f(xx, yy, -d);
    }
    glEnd();

    glBegin(GL_QUADS);
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


void cube(int t)
{
    if(t!=0)
    {
        glBindTexture(GL_TEXTURE_2D,t);
        glEnable(GL_TEXTURE_2D);
    }
    glBegin(GL_QUADS);
    glVertex3f(0,2,0);
    glVertex3f(0,0,0);
    glVertex3f(2,0,0);
    glVertex3f(2,2,0);

    glVertex3f(2,2,0);
    glVertex3f(2,0,0);
    glVertex3f(2,0,-2);
    glVertex3f(2,2,-2);

    glVertex3f(2,2,-2);
    glVertex3f(2,0,-2);
    glVertex3f(0,0,-2);
    glVertex3f(0,2,-2);

    glVertex3f(0,0,0);
    glVertex3f(0,2,0);
    glVertex3f(0,2,-2);
    glVertex3f(0,0,-2);

    glVertex3f(0,2,0);
    glVertex3f(2,2,0);
    glVertex3f(2,2,-2);
    glVertex3f(0,2,-2);

    glVertex3f(2,0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,-2);
    glVertex3f(2,0,-2);
    glEnd();
    if(t!=0)
        glDisable(GL_TEXTURE_2D);

}


void pic()
{
    glBindTexture(GL_TEXTURE_2D,5);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex3d(0,0,0);
    glVertex3d(4,0,0);
    glVertex3d(4,4,0);
    glVertex3d(0,4,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}



void table(double d)
{
    glRotated(ax, 1, 0, 0);
    glRotated(ay, 0, 1, 0);
    glRotated(az, 0, 0, 1);
    glScaled(1.2,1,1);
    glTranslated(-6,0,0);
    glTranslated(-15,-20,10);
    glScaled(25,.5,20);
    if(d!=0)
        cube(2);
    else
        cube(0);
    glScaled(.04,2,.05);

    glPushMatrix();
    glRotated(90,0,1,0);
    glScaled(20,25,.5);
    if(d!=0)
        cube(3);
    else
        cube(0);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0,0,-40);
    glScaled(25,25,.5);
    if(d!=0)
        cube(3);
    else
        cube(0);
    glTranslated(.7,.75,3);
    glScaled(0.14,0.12,0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(7,0.5,-4);
    glScaled(.5,4.74,.5);
    if(d!=0)
        cube(4);
    else
        cube(0);
    glTranslated(48,0,0);
    if(d!=0)
        cube(4);
    else
        cube(0);
    glTranslated(0,0,-40);
    if(d!=0)
        cube(4);
    else
        cube(0);
    glTranslated(-48,0,0);
    if(d!=0)
        cube(4);
    else
        cube(0);
    glPopMatrix();

    glPushMatrix();
    glTranslated(4.5,10,-2);
    glScaled(16,.5,12);
    if(d!=0)
        cube(4);
    else
        cube(0);
    glPopMatrix();


}
void pillar()
{
    glLineWidth(9.0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0,0,0);
    glVertex3f(-3.5, -3.5, 3.5);
    glVertex3f(-3,-3,3);
    glVertex3f(3, -3, 3);
    glVertex3f(3.5, -3.5, 3.5);
    glVertex3f(0,0,0);
    glEnd();

    glLineWidth(5.0);
    glBegin(GL_LINE_STRIP);
    //glColor3f( a,b,c);
    glVertex3f(-1, -1, 1);
    glVertex3f(1,-1,1);
    glVertex3f(2,-2,2);
    glVertex3f(-2, -2, 2);
    glEnd();
}

void line()
{
    glBegin(GL_POLYGON);
    //glColor3f( a,b,c);
    glVertex3f(0,1,.2);
    glVertex3f(0,0,.2);
    glVertex3f(4,0,.2);
    glVertex3f(4.5,0.5,.2);
    glVertex3f(4,1,.2);
    glVertex3f(0,1,.2);

    //glColor3f( a,b,c);
    glVertex3f(0,1,.2);
    glVertex3f(4,1,.2);
    glVertex3f(4.5,0.5,.2);
    glVertex3f(4,0,.2);
    glVertex3f(0,0,.2);
    glVertex3f(0,1,.2);
    glEnd();
}

void windmill(double a)
{
    glRotated(ax, 1, 0, 0);
    glRotated(ay, 0, 1, 0);
    glRotated(az, 0, 0, 1);
    glTranslated(-6,8,0);
    ownScaled(1,5,1);
    pillar();

    glTranslated(0,0,0);
    glRotated(-90,0,1,0);
    pillar();

    glTranslated(0,0,0);
    glRotated(-90,0,1,0);
    pillar();

    glTranslated(0,0,0);
    glRotated(-90,0,1,0);
    pillar();
    glRotated(-90,0,1,0);

    glScaled(1,.2,1);
    glRotated(a,0,0,1);
    glScaled(1,5.0,1);

    glTranslated(0,0,1);
    glScaled(1.25,0.2,1.25);
    tire(1,.9);
    glTranslated(0,-2,-1);

    glTranslated(.8,1.5,1.8);
    glScaled(1.5,1,1);
    line();
    glScaled(0.666,1,1);
    glTranslated(-.4,-1.5,-1.8);

    glTranslated(0,2.9, 1.8);
    glRotated(118, 0, 0, 1);
    glScaled(1.5,1,1);
    line();
    glScaled(0.666,1,1);
    glRotated(-118, 0, 0, 1);
    glTranslated(-.4,-2.9,-1.8);

    glTranslated(-.65,1.8,1.8);
    glRotated(45, 0, 0, 1);
    glScaled(-1.6,-1,1);
    line();

    glScaled(-0.6024,-1,1);
    glRotated(-45, 0, 0, 1);
    glTranslated(.65,-1.8,-1.8);

    glTranslated(0,1.98,2.2);
    tire(1,.2);
    glTranslated(0,0,.5);
    tire(.3,.3);
    glTranslated(0,-1.98,-2.7);
}

void jack_hands()
{
    glPushMatrix();
    glScaled(1, 1, 5.0);
    glutSolidSphere(1, 15, 15);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, -5);
    glutSolidSphere(1, 15, 15);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, 5);
    glutSolidSphere(1, 15, 15);
    glPopMatrix();
}

void jack()
{
    glRotated(ax, 1, 0, 0);
    glRotated(ay, 0, 1, 0);
    glRotated(az, 0, 0, 1);
    glPushMatrix();

    glTranslated(10,10,-25);
    glScaled(5,5,5);
   snowflake(3);
    glScaled(0.2,0.2,0.2);
    glTranslated(-10,-10,28);

     glTranslated(-5,10,-28);
    glScaled(5,5,.5);
   cube(5);
    glScaled(0.2,0.2,2);
    glTranslated(5,-10,28);


    glRotated(54,1,0,0);
    glTranslated(5,-10,0);
    glScaled(0.8,0.8,0.8);
    glRotated(45, 0, 1, 0);
    jack_hands();
    glRotated(90, 1, 0, 0);
    jack_hands();
    glRotated(90, 0, 1, 0);
    jack_hands();
    glPopMatrix();
}


void view(double a,double b)
{
    glPushMatrix();
    glScaled(1,0.7,1);
    glPushMatrix();
    glScaled(zm, zm, zm);
    glTranslated(xx, yy, zz);

    if(b==0)
    table(0);
    else
        table(1);
    glPopMatrix();

    glPushMatrix();
    glScaled(zm, zm, zm);
    glTranslated(xx, yy, zz);
    windmill(a);
    glPopMatrix();

    glPushMatrix();
    glScaled(zm, zm, zm);
    glTranslated(xx+5, yy, zz);
    jack();
 glNormal3f(0.0,0.0,1.0);
    glPopMatrix();
    glPopMatrix();

}
static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    {
       gluLookAt(eye_x, eye_y, eye_z,px, py, pz,ch_a, ch_b,0);

       glPushMatrix();
        glViewport(0, 0, width/2, height);
        view(a,1);
        //tsquare(3);
        glPopMatrix();

        glPushMatrix();
        glViewport(width/2, 0, width/2, height/1);
        view(a,0);
        glPopMatrix();


    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    glutSwapBuffers();
}

void LIGHT()
{
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}
void LIGHT1()
{
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
}
void off_ambient()
{
    for(int i=0; i<3; i++)
        light_ambient[i]-=0.2;
    LIGHT();
}
void on_ambient()
{
    for(int i=0; i<3; i++)
        light_ambient[i]+=0.2;
    LIGHT();
}
void off_ambient1()
{
    for(int i=0; i<3; i++)
        light_ambient1[i]-=0.2;
    LIGHT1();
}
void on_ambient1()
{
    for(int i=0; i<3; i++)
        light_ambient1[i]+=.2;
    LIGHT1();
}
void off_specular()
{
    for(int i=0; i<3; i++)
        light_specular[i]=0;
    LIGHT();
}
void on_specular()
{
    for(int i=0; i<3; i++)
        light_specular[i]=1;
    LIGHT();
}

void spot_off()
{

    sp=90.0;
    glLightfv(GL_LIGHT2, GL_AMBIENT,  light_ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, sp);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);
}

void spot_on()
{

    sp=91;
    glLightfv(GL_LIGHT2, GL_AMBIENT,  light_ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, sp);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);
}

void off_light()
{
    if(on==0)
        return;
    on=0;
    for(int i=0; i<3; i++)
    {
        if(i==0)
            qa=light_diffuse[i];
        if(i==1)
            qb=light_diffuse[i];
        if(i==2)
            qc=light_diffuse[i];
        light_diffuse[i]=0;
        light_specular[i]=0;
    }

    LIGHT();
}

void on_light()
{
    if(on==1)
        return;
    on=1;
    light_diffuse[0]=qa;
    light_diffuse[1]=qb;
    light_diffuse[2]=qc;
    light_specular[0]=qa;
    light_specular[1]=qb;
    light_specular[2]=qc;
    LIGHT();
}


void off_light1()
{
    if(on1==0)
        return;
    on1=0;
    for(int i=0; i<3; i++)
    {
        if(i==0)
            qa1=light_diffuse1[i];
        if(i==1)
            qb1=light_diffuse1[i];
        if(i==2)
            qc1=light_diffuse1[i];
        light_diffuse1[i]=0;
        light_specular1[i]=0;
    }
    LIGHT1();
}

void on_light1()
{
    if(on1==1)
        return;
    on1=1;
    light_diffuse1[0]=qa1;
    light_diffuse1[1]=qb1;
    light_diffuse1[2]=qc1;
    light_specular1[0]=qa1;
    light_specular1[1]=qb1;
    light_specular1[2]=qc1;
    LIGHT1();
}
void change_light_position_x1()
{
    light_position1[0]+=1;
    LIGHT1();

}
void change_light_position_y1()
{
    light_position1[1]+=1;
    LIGHT1();

}
void change_light_position_z1()
{
    light_position1[2]+=1;
    LIGHT1();

}
void change_light_position_x()
{
    light_position[0]+=1;
    LIGHT();
}
void change_light_position_y()
{
    light_position[1]+=1;
    LIGHT();

}
void change_light_position_z()
{
    light_position[2]+=1;
    LIGHT();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case '.':
        exit(0);
        break;
    case 'A':
        ax+=1.5;
        break;
    case 'a':
        ax-=1.5;
        break;
    case 'S':
        ay+=1.5;
        break;
    case 's':
        ay-=1.5;
        break;
    case 'D':
        az+=1.5;
        break;
    case 'd':
        az-=1.5;
        break;
    case '+':
        zm+=.02;
        break;
    case '-':
        zm-=.02;
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

    case 'o':
        off_light();
        break;
    case 'O':
        on_light();
        break;
    case 'k':
        off_light1();
        break;
    case 'K':
        on_light1();
        break;
    case '1':
        change_light_position_x();
        break;
    case '2':
        change_light_position_y();
        break;
    case '3':
        change_light_position_z();
        break;
    case '4':
        change_light_position_x1();
        break;
    case '5':
        change_light_position_y1();
        break;
    case '6':
        change_light_position_z1();
        break;
    case 'm':
        on_ambient();
        break;
    case 'M':
        off_ambient();
        break;
    case 'n':
        on_ambient1();
        break;
    case 'N':
        off_ambient1();
        break;
//    case 'c':
//        on_specular();
//        break;
//    case 'C':
//        off_specular();
        break;
    case 't':
        spot_off();
        break;
    case 'T':
        spot_on();
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

void Init()
{
    pix[0].makeCheckImage();
    pix[0].setTexture(1);

    pix[1].readBMPFile("C:\\Users\\BORNA\\Desktop\\jambu\\new new\\teapot.bmp");
    pix[1].setTexture(2);

    pix[2].readBMPFile("C:\\Users\\BORNA\\Desktop\\jambu\\new new\\brick-con.bmp");
    pix[2].setTexture(3);

    pix[3].readBMPFile("C:\\Users\\BORNA\\Desktop\\jambu\\new new\\furniture.bmp");
    pix[3].setTexture(4);

    pix[4].readBMPFile("C:\\Users\\BORNA\\Desktop\\jambu\\new new\\dd.bmp");
    pix[4].setTexture(5);


}

int main(int argc, char *argv[])
{
    printf("A/a for X-axis rotation\n");
    printf("S/s for Y-axis rotation\n");
    printf("D/d for Z-axis rotation\n");
    printf("X/x for X-axis translation\n");
    printf("Y/y for Y-axis translation\n");
    printf("Z/z for Z-axis translation\n");
    printf("+ for Zoom-in\n");
    printf("- for Zoom-out\n");
    printf("r for roll\n");
    printf("p for pitch\n");
    printf("w for yaw\n");
    printf("1 for change light-1 position in X-axis\n");
    printf("2 for change light-1 position in Y-axis\n");
    printf("3 for change light-1 position in Z-axis\n");
    printf("4 for change light-2 position in X-axis\n");
    printf("5 for change light-2 position in Y-axis\n");
    printf("6 for change light-2 position in Z-axis\n");
    printf("O/o for light-1 on/off\n");
    printf("K/k for light-2 on/off\n");
    printf("m/M for increase/decrease of light-1 ambient\n");
    printf("n/N for increase/decrease of light-2 ambient\n");
    glutInit(&argc, argv);
    //width*height;
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);


    glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);


    glLightfv(GL_LIGHT2, GL_AMBIENT,  light_ambient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light_diffuse2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, sp);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    Init();
    glutMainLoop();


    return EXIT_SUCCESS;
}
