/*
Edmund Wei Jet, Chin
Skybox
*/
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>
#include <SOIL.h>

#define PI 3.14159

using namespace std;

bool WireFrame= false; //boolean toggle for wireframe
float Wwidth, Wheight; //width & height for reshape function

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

double cube[]= // x, y & z coordinates for cube with separate vertices
    {
    +1.0,-1.0,-1.0,0.0,1.0,
    -1.0,-1.0,-1.0,1.0,1.0,
    -1.0,+1.0,-1.0,1.0,0.0,
    +1.0,+1.0,-1.0,0.0,0.0,

    -1.0,+1.0,-1.0,0.0,0.0,
    -1.0,-1.0,-1.0,0.0,1.0,
    -1.0,-1.0,+1.0,1.0,1.0,
    -1.0,+1.0,+1.0,1.0,0.0,

    -1.0,-1.0,+1.0,0.0,1.0,
    +1.0,-1.0,+1.0,1.0,1.0,
    +1.0,+1.0,+1.0,1.0,0.0,
    -1.0,+1.0,+1.0,0.0,0.0,

    +1.0,-1.0,-1.0,1.0,1.0,
    +1.0,+1.0,-1.0,1.0,0.0,
    +1.0,+1.0,+1.0,0.0,0.0,
    +1.0,-1.0,+1.0,0.0,1.0,

    +1.0,+1.0,-1.0,0.0,1.0,
    -1.0,+1.0,-1.0,1.0,1.0,
    -1.0,+1.0,+1.0,1.0,0.0,
    +1.0,+1.0,+1.0,0.0,0.0,

    +1.0,-1.0,+1.0,0.0,0.0,
    -1.0,-1.0,+1.0,1.0,0.0,
    -1.0,-1.0,-1.0,1.0,1.0,
    +1.0,-1.0,-1.0,0.0,1.0,
    };

GLfloat cube_x_angle; // float value to rotate x-axis of cube
GLfloat cube_y_angle; // float value to rotate y-axis of cube
GLfloat sky_x_angle, sky_y_angle; //float values to rotate the skybox through x & y values respectively

GLfloat scale_cube; //scaling float value for cube
GLfloat scale_sky; //scaling float value for skybox

GLfloat cam_x; // x position of camera
GLfloat cam_y; // y position of camera

GLuint tex; //texture binding pointer

/* GLUT callback Handlers */

void TextureLoader(char *fileName, GLuint &tex){
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

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

 void draw_cube(){ //Function to draw the cube
    glPushMatrix();
    glRotatef(cube_x_angle, 1, 0, 0); //Rotate cube through x
    glRotatef(cube_y_angle, 0,1, 0); // Rotate cube through y
    glScalef(scale_cube, scale_cube, scale_cube); //Scaling of the cube

    int i,n=sizeof(cube)/(sizeof(cube[0]));
        glGenTextures(1, &tex);
        TextureLoader("images/mine_cube.png", tex);//Applying texture to cube
        glBegin(GL_QUADS);
        for (i=0;i<n;i+=5) // Drawing the cube using the matrix declared above
            {
            glTexCoord2dv(cube+i+3);
            glVertex3dv(cube+i+0);
            }
        glEnd();
    glPopMatrix();
 }

 void skyBox(){
    glGenTextures(1, &tex);
    TextureLoader("images/forest.png", tex); //Texture for the skybox
    glPushMatrix();

    glRotatef(sky_x_angle, 1, 0, 0); //To rotate the skybox, viewing of different scenes.
    glRotatef(sky_y_angle, 0, 1, 0);
    glScalef(scale_sky, scale_sky, scale_sky); // Scales the skybox

    glBegin(GL_QUADS);

    //front face of skybox
    glNormal3f(0, 0, -1);
    glTexCoord2f(.25, .667);
    glVertex3f(1, -1, 1);
    glTexCoord2f(.5, .667);
    glVertex3f(-1, -1, 1);
    glTexCoord2f(.5, .333);
    glVertex3f(-1, 1, 1);
    glTexCoord2f(.25, .333);
    glVertex3f(1, 1, 1);

    //back face of skybox
    glNormal3f(0,0,1);
    glTexCoord2f(.75, 0.667);
    glVertex3f(-1,-1,-1);
    glTexCoord2f(1, 0.667);
    glVertex3f(1, -1, -1);
    glTexCoord2f(1, 0.333);
    glVertex3f(1, 1, -1);
    glTexCoord2f(0.75, 0.333);
    glVertex3f(-1, 1, -1);

    //top face of skybox
    glNormal3f(0, -1, 0);
    glTexCoord2f(.25, .333);
    glVertex3f(1, 1, 1);
    glTexCoord2f(.5, .333);
    glVertex3f(-1, 1, 1);
    glTexCoord2f(.5, 0);
    glVertex3f(-1, 1, -1);
    glTexCoord2f(.25, 0);
    glVertex3f(1, 1, -1);

    //bottom face of skybox
    glNormal3f(0, 1, 0);
    glTexCoord2f(.25, 1);
    glVertex3f(1, -1, -1);
    glTexCoord2f(.5, 1);
    glVertex3f(-1, -1, -1);
    glTexCoord2f(.5, .667);
    glVertex3f(-1, -1, 1);
    glTexCoord2f(.25, .667);
    glVertex3f(1, -1, 1);

    //left face of skybox
    glNormal3f(-1,0,0);
    glTexCoord2f(0, 0.667);
    glVertex3f(1,-1,-1);
    glTexCoord2f(0.25, 0.667);
    glVertex3f(1, -1, 1);
    glTexCoord2f(0.25, 0.333);
    glVertex3f(1, 1, 1);
    glTexCoord2f(0, 0.333);
    glVertex3f(1, 1, -1);

    //right face of skybox
    glNormal3f(1, 0, 0);
    glTexCoord2f(.5, .667);
    glVertex3f(-1, -1, 1);
    glTexCoord2f(.75, .667);
    glVertex3f(-1, -1, -1);
    glTexCoord2f(.75, .333);
    glVertex3f(-1, 1, -1);
    glTexCoord2f(.5, .333);
    glVertex3f(-1, 1, 1);

    glEnd();
    glPopMatrix();
 }



static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,4, cam_x,cam_y,0.0,0.0,1.0,0.0); //cam_x & cam_y changes reference point

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

 // your code here
    //Calling the skybox & cube functions to draw them
   glDisable(GL_LIGHTING);  // I was actually trying to figure out how to remove the lighting from the cubemap face being looked at, appreciate that you shared this :)
   skyBox();
   glDisable(GL_LIGHTING);
   draw_cube();

 glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q': //key 'q' to exit application
            exit(0);
            break;

        case '/': //key '/' to toggle wireframe
		WireFrame =!WireFrame;
	       break;
	       //keys 'a' & 'd' to change the y position of cube
        case 'a':
            cube_y_angle -=5;
            break;
        case 'd':
            cube_y_angle +=5;
            break;
        //keys 'w' & 's' to change the x position of cube
        case 'w':
            cube_x_angle -=5;
            break;
        case 's':
            cube_x_angle +=5;
            break;
        case 'j':
            cam_x -= 0.2; //looks left by decreasing cam_x
            break;
        case 'l':
            cam_x += 0.2; //looks right by increasing cam_x
            break;
        case 'i':
            cam_y += 0.2; //looks to the top by increasing cam_y
            break;
        case 'k':
            cam_y -=0.2; //looks to the bottom by decreasing cam_y
            break;
        case 'o':  //resets the scene
            cam_x = 0;
            cam_y = 0;
            break;
        case 'z': // zooms in on the whole scene
            scale_cube += 0.1;
            scale_sky +=0.1;
            break;
        case 'x': //zooms out
            scale_cube -= 0.1;
            scale_sky -=0.1;
            break;
        case ' ': // Space bar to reset all rotations and scaling of the scene
            cam_x = 0;
            cam_y=0;
            cube_x_angle =0;
            cube_y_angle=0;
            sky_x_angle=0;
            sky_y_angle=0;
            scale_cube =0.3;
            scale_sky = 5;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_RIGHT: //rotates skybox to the right
            sky_y_angle += 5;
            break;

     case GLUT_KEY_LEFT: //rotates skybox to the left
            sky_y_angle -=5;
            break;
     case GLUT_KEY_UP: //rotates skybox upwards
            sky_x_angle -=5;
            break;
     case GLUT_KEY_DOWN: //rotates skybox downwards
        sky_x_angle += 5;
        break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop
     glutPostRedisplay();
}

static void init(void)
{
    //initializing the variables declared above
    cam_x = 0;
    cam_y = 0;

    cube_x_angle =0;
    cube_y_angle=0;

    sky_x_angle=0;
    sky_y_angle=180;

    scale_cube =0.3;
    scale_sky = 5;

   // glEnable(GL_CULL_FACE);
  //  glCullFace(GL_BACK);

    glClearColor(0, 0, 0, 0);                 // assign a color you like

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

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1200,900);
    glutInitWindowPosition(50,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("SkyBox with Cube");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
