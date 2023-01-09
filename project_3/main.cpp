/*
Edmund Wei Jet, Chin
Space
*/
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include <math.h>
#include <SOIL.h>
using namespace std;

#define SPACEBAR 32 //decimal number for spacebar (toggles the start of simulation)

GLUquadricObj *sphere= NULL; //renders a sphere
GLuint tex; //texture pointer
//variables to rotate the two planets & moon
static int year, day;
int spin; //rotate value for the lighting from the sun
//variables used for scaling and rotating the entire matrix
int scale_val;
int rotate_val;
clock_t start = clock(); //setting processor time
clock_t endt; //used for processor end time

bool simulation_start; //boolean to start the simulation
bool WireFrame;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 3.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* GLUT callback Handlers */

void TextureLoader(char *fileName ,GLuint &tex){ //applies texture for object
    glEnable(GL_TEXTURE_2D);
     glGenTextures(1, &tex);
     glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

     glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
     int width, height; // width & height for the image reader
     unsigned char* image;

     image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
    if(!image){ // check if image is loaded
        cout<<"not found"<<endl;
     }
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // binding image data
     SOIL_free_image_data(image);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(8,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


    // your code here
    glScalef( scale_val,scale_val,scale_val ); //scale_val is manipulated on key press up / down to zoom in or out respectively
    glRotatef(rotate_val, 0, 1, 0); // rotate_val is manipulated using left / right keys to rotate the view of planets

    glPushMatrix();
    glEnable(GL_LIGHTING); //enables lighting for objects
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position); //set the position of the light, back side of planet not lighted
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); //directional impact of light on object
   // glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // sets ambient lighting
   // glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); // bright spots on object

    if(simulation_start){ //starts the simulation when variable is true
      glRotatef((GLfloat) year,  0.0, 1.0, 0.0); //rotates on its own axis
       year = (year+2) %360; //changes the y angle of earth
    }
    glGenTextures(1, &tex); //applying earth texture to sphere
    TextureLoader("images/earth2.png", tex);
    gluSphere(sphere,2.0, 40,40); // draws earth

    glPushMatrix();

    glPushMatrix();
    glTranslatef(3.0, 0.0, 0.0); //move the second sphere based on x value
    if(simulation_start){ //starts the simulation when variable is true
        glRotatef((GLfloat) day * 10, 0.0, 1.0, 0.0); //rotates around earth
        day = (day +10) %360; //changes the y angle of moon
    }
    glGenTextures(1, &tex);
    TextureLoader("images/moon.png", tex); //applying moon texture to second sphere
    gluSphere(sphere, .5, 50, 50); // smaller sphere for moon
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.0, 1.0, 0.0); // translating another planet x & y position away from earth
    if(simulation_start){ //starts the simulation when variable is true
        glRotatef((GLfloat) year*5, 0.0, 1.0, 0.0); //orbit and rotate around earth
        year = (year+2) % 360; //changes the y angle of  the second planet
    }
     glGenTextures(1, &tex);
    TextureLoader("images/mars.png", tex); // applying mars texture for the second planet
    gluSphere(sphere,1.0, 40,40); //draws second planet
    glPopMatrix();

    glPopMatrix();
    if(simulation_start){
        glDisable(GL_TEXTURE_2D); //disable active texture unit
        glPushMatrix();
        glRotatef(spin, 0, 1,0); //rotates the lighting (spin value controlled in idle function)
        glLightfv(GL_LIGHT0, GL_POSITION, light_position); //position of lighting
        glPopMatrix();
    }
    glPopMatrix();


    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'w': // 'w' key to switch to wire frame view
            WireFrame =!WireFrame;
            break;
        case 32: //'spacebar' starts or stops simulation
            simulation_start = !simulation_start;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP: //zoom in on key up
        scale_val += 0.2;
    break;
    case GLUT_KEY_DOWN: // zoom out on key down
        scale_val -= 0.2;
    break;
    case GLUT_KEY_LEFT: //left key to rotate left
        rotate_val += 10;
    break;
    case GLUT_KEY_RIGHT: // right key to rotate right
        rotate_val -= 10;
    break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    endt=clock(); //getting processor time
    if(endt-start > 10){ //manipulating spin value based on condition
        spin+=7;
        start = clock();
    }
    glutPostRedisplay();
}



static void init(void)
{
    //initial values for rotation
    year =0;
    day=0;
    WireFrame = false; // boolean value for wire frame set to false, can be toggled using 'w' key
    simulation_start = false; // boolean value to start the simulation, toggled using 'spacebar'
    //initial values to scale and rotate the matrix
    scale_val =1;
    rotate_val =1;
    //spin value for sun lighting on planets
    spin= 2;

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    // draws a sphere
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1000,800);
    glutInitWindowPosition(200,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 3");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
