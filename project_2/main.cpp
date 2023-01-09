/*
Edmund Wei Jet, Chin
Modeling Transformations
*/
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
using namespace std;

bool WireFrame= false;
//to make objects shiny
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// initial boolean values for each object is set to false
bool  cubeFlag = false;
bool teaFlag = false;
bool sphereFlag = false;

// six double variables  using GLdouble from library
GLdouble cubeTrans;
GLdouble cubeAngle;
GLdouble sphereTrans;
GLdouble sphereAngle;
GLdouble teaTrans;
GLdouble teaAngle;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    double Ratio;

    if(width<=height)
    {
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
        Ratio = height/width;
    }

    else
    { //resize differently when width > height
        glViewport((GLsizei) (width-height)/2,0,(GLsizei) height,(GLsizei) height);
        Ratio = width/height;
    }

    glMatrixMode(GL_PROJECTION); //matrix operation on projection matrix stack
    glLoadIdentity(); //resets matrix back to its default state
    gluPerspective (50.0f,Ratio,0.1f, 100.0f); // provides the perception of depth
}

/*
glColor3f(): provides color to object using RGB values
glPushMatrix(): pushing matrix to apply transformation to object
glTranslatef(): translation of objects using floating point values (param: x, y, z)
glRotatef(): Rotates the selected object using floating point values (param: angle, x, y, z)
glPopMatrix(): end of operations, popping matrix off the stack
*/
void  cubeCode(){
    glPushMatrix();
    glColor3f(0, 1.5, 1.5);
    glTranslatef(3.7, 0.0, cubeTrans);
    glRotatef(cubeAngle, 0.0, 1.0, 0.0);
    glutSolidCube(2); //renders a solid cube (param: size)
    glPopMatrix();
}

void sphereCode(){
    glPushMatrix();
    glColor3f(2.0, 0.5, 2.0);
    glTranslatef(-3.3, 0.0, sphereTrans);
    glRotatef(sphereAngle, 0.0, 1.0, 0.0);
    glutSolidSphere(1.3, 50, 40); //renders a solid sphere (param: radius, longitude, latitude)
    glPopMatrix();
}

void teaCode(){
    glPushMatrix();
    glColor3f(5.0, 2.0, 1.0);
    glTranslatef(0.0, 0.0, teaTrans);
    glRotatef(teaAngle, 0.0, 1.0, 0.0);
    glutSolidTeapot(1.5); // renders a solid teapot (param: size)
    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clearing the buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0); //camera: x,y ,z, 0,0,0 (looking from center), 0, 1, 0 (1 for looking straight up)

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    //software engineering skills, divided into reusable functions :)
    cubeCode(); //  calls cube code
    sphereCode();  //calls sphere code
    teaCode(); //calls teaCode

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q': // exit on key press 'q'
        exit(0);
        break;
    case 'c': // selects cube on key press 'c'
        sphereFlag = false;
        teaFlag = false;
        cubeFlag = true;
        break;
    case 's': //selects sphere on key press 's'
        cubeFlag = false;
        teaFlag = false;
        sphereFlag = true;
        break;
    case 't': //selects teapot on key press 't'
        cubeFlag = false;
        sphereFlag = false;
        teaFlag = true;
        break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key) //each key press updates the value(s) of glTranslatef /glRotatef  based on the selected object from the key function
    {
    case GLUT_KEY_UP: // Up arrow key  moves object upwards
        //only the selected object will move
        if(cubeFlag){
            cubeTrans -= 0.5;
        }
        else if(sphereFlag){
            sphereTrans -= 0.5;
        }
        else{
            teaTrans -= 0.5;
        }
        break;
    case GLUT_KEY_DOWN: // Down arrow key moves object downwards
        if(cubeFlag) {
            cubeTrans += 0.5;
        }
        else if(sphereFlag) {
            sphereTrans += 0.5;
        }
        else {
            teaTrans += 0.5;
        }
        break;
    case GLUT_KEY_LEFT: //Left arrow key rotates object clockwise
        if(cubeFlag){
            cubeAngle -= 5;
        }
        else if(sphereFlag){
            sphereAngle -= 5;
        }
        else{
            teaAngle -= 5;
        }
        break;
    case GLUT_KEY_RIGHT: //Right arrow key rotates object counterclockwise
        if(cubeFlag){
            cubeAngle += 5;
        }
        else if(sphereFlag){
            sphereAngle += 5;
        }
        else{
            teaAngle += 5;
        }
        break;
    }
    glutPostRedisplay(); //render a new screen after each key press
}

static void idle(void) //runs in a loop
{
    glutPostRedisplay();
}

static void init(void)
{
    //initialize translation & angle values for cube, sphere, and teapot
    cubeTrans = -2.0;
    cubeAngle = 0.0;
    sphereTrans = -1.0;
    sphereAngle = 0.0;
    teaTrans = -3.0;
    teaAngle = 0.0;
    cubeFlag = false;
    teaFlag = false;
    sphereFlag = false;

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL); // color assigned to object

    glEnable(GL_DEPTH_TEST); // z depth
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

   // glEnable(GL_CULL_FACE); // transparency in the back, not needed for this project
  //  glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1000,850);
    glutInitWindowPosition(500,100);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
