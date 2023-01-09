/*
Edmund Wei Jet, Chin
Model Loader
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
#include<string>
#include <sstream>
#include <cstring>
#include <SOIL.h>
#include <fstream>
#include <time.h>

#define PI 3.14159

using namespace std;

// data types to hold three values for vertices
typedef struct vec3{
    float x; //x coordinate
    float y; // y coordinate
    float z; //z coordinate

    vec3(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
}vec3;

//data type to hold two values for vertices
typedef struct vec2{
    float x; // x coordinate
    float y; // y coordinate

    vec2(float x, float y){
        this->x = x;
        this->y = y;
    }
}vec2;

 //cube setup
int ** cube_face = new int* [12];  //2D array for cube faces
vector<vec3> cube_v; //to store cube vertices
vector<vec3> cube_vn; // to store cube normals
vector<vec2> cube_vt; // to store cube texture indices
//ven
int **ven_face = new int*[43357]; //2D array for ven faces
vector<vec3> ven_v; // to store ven vertices
vector<vec3> ven_vn; // to store ven normals
vector<vec2> ven_vt_empty; // to store ven texture indices (not given in obj file)
//ateneam
int **ateneam_face = new int* [22250]; //2D array for ateneam faces
vector<vec3> ateneam_v; // to store ateneam vertices
vector<vec3> ateneam_vn; // to store ateneam normals
vector<vec2> ateneam_vt; // to store ateneam texture indices
//ele
int **elephant_face = new int* [10150]; //2D array for elephant faces
vector<vec3> ele_v; // to store elephant vertices
vector<vec3> ele_vn; // to store elephant normals
vector<vec2> ele_vt_empty; // to store elephant texture indices (not given in obj file)
//trunk
int **trunk_face = new int* [4030]; //2D array for trunk faces
vector<vec3> trunk_v; // to store trunk vertices
vector<vec3> trunk_vn; // to store trunk normals
vector<vec2> trunk_vt_empty; // to store trunk texture indices (not given in obj file)
//cow
int **cow_face = new int* [3263]; // 2D array for cow faces
vector<vec3> cow_v; // to store cow vertices
vector<vec3> cow_vn; // to store cow normals
vector<vec2> cow_vt_empty; // to store cow texture indices (not given in obj file)

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

GLfloat sky_x_angle, sky_y_angle; //float values to rotate the skybox through x & y values respectively

GLfloat scale_sky; //scaling float value for skybox

GLfloat cam_x; // x position of camera
GLfloat cam_y; // y position of camera
GLfloat cam_z; //z position of camera
GLfloat eye_x; // x position of eye
GLfloat eye_y; // y position of eye
GLfloat eye_z; // z position of eye


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

	gluPerspective (60.0f,Ratio,0.1f, 1500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

 void cube_draw(){
    glPushMatrix();
    glTranslatef(3.0, -1.0, -2); //moving cube to center right of screen
    TextureLoader("images/mine_cube.png", tex); // applying texture for cube
    int z =0; // to attain next three values that have been stored
    for(int i=0; i< 12; ++i){
        glBegin(GL_TRIANGLES);
        for(int c=0, z=0; c<=2; ++c){
            glNormal3f(cube_vn[cube_face[i][2+z]-1].x, cube_vn[cube_face[i][2+z]-1].y, cube_vn[cube_face[i][2+z]-1].y); // drawing of normals
            glTexCoord2f(cube_vt[cube_face[i][1+z]-1].x, cube_vt[cube_face[i][1+z]-1].y); // drawing of textures
            glVertex3f(cube_v[cube_face[i][0+z]-1].x, cube_v[cube_face[i][0+z]-1].y, cube_v[cube_face[i][0+z]-1].z); // drawing of vertices
            z+=3;
        }
        glEnd();
    }
    glPopMatrix();
 }

 void ateneam_draw(){
    glPushMatrix();
    TextureLoader("images/orange.png", tex); //applying texture to ateneam
    glTranslatef(1.5, 2.0, -2); //positions the object to be levitating above
    glRotatef(-180,0, 1, 0); // rotates the object sideways
    glScalef(.4 / (1500), .4 / (1500), .4 / (1500)); // decreasing the size of ateneam
    int z=0;// to attain next three values that have been stored

    for (int i = 0; i < 22250; i++) {
        glBegin(GL_TRIANGLES);
        for (int c = 0, z = 0; c <= 2; ++c) {
            glNormal3f(ateneam_vn[ateneam_face[i][2 + z] - 1].x, ateneam_vn[ateneam_face[i][2 + z] - 1].y, ateneam_vn[ateneam_face[i][2 + z] - 1].z); //drawing of normals
            glTexCoord2f(ateneam_vt[ateneam_face[i][1 + z] - 1].x, ateneam_vt[ateneam_face[i][1 + z] - 1].y); // drawing of textures
            glVertex3f(ateneam_v[ateneam_face[i][0 + z] - 1].x, ateneam_v[ateneam_face[i][0 + z] - 1].y, ateneam_v[ateneam_face[i][0 + z] - 1].z); //drawing of vertices
            z+=3;
        }
        glEnd();
    }
    glPopMatrix();
 }



 void ven_draw(){
     glPushMatrix();
     glTranslatef(0, -4.7, 1); // positions object to be right in front of user
     TextureLoader("images/abstract.png", tex); // applying texture to ven
     glScalef(.4 / (1300), .4 / (1300), .4 / (1300)); //decreasing the size of ven
     for(int i=0; i<43357; ++i){
        glBegin(GL_TRIANGLES);
        glDisable(GL_TEXTURE_2D); //disabling texture for faces
        //ven does not have texture values
        glVertex3f(ven_v[ven_face[i][0]-1].x, ven_v[ven_face[i][0]-1].y, ven_v[ven_face[i][0]-1].z); //drawing vertex
        glNormal3f(ven_vn[ven_face[i][1]-1].x, ven_vn[ven_face[i][1]-1].y, ven_vn[ven_face[i][1]-1].z); //drawing normals

        glVertex3f(ven_v[ven_face[i][2]-1].x, ven_v[ven_face[i][2]-1].y, ven_v[ven_face[i][2]-1].z); //drawing vertex
        glNormal3f(ven_vn[ven_face[i][3]-1].x, ven_vn[ven_face[i][3]-1].y, ven_vn[ven_face[i][3]-1].z);//drawing normals

        glVertex3f(ven_v[ven_face[i][4]-1].x, ven_v[ven_face[i][4]-1].y, ven_v[ven_face[i][4]-1].z);//drawing vertex
        glNormal3f(ven_vn[ven_face[i][5]-1].x, ven_vn[ven_face[i][5]-1].y, ven_vn[ven_face[i][5]-1].z);//drawing normals

        glEnable(GL_TEXTURE_2D);//re-enabling textures
        glEnd();
     }
     glPopMatrix();
 }

 void ele_draw(){
     glPushMatrix();
     glTranslatef(-3.0, -5.0, -3); //positioned at the bottom left of skybox
     TextureLoader("images/grey.png", tex); //applying texture for elephant
     glScalef(.4/100, .4/100, .4/100); //decreasing the size of elephant
     for(int i=0; i<10150; ++i){
        glBegin(GL_TRIANGLES);
        glDisable(GL_TEXTURE_2D); //disable texture for faces
        glVertex3f(ele_v[elephant_face[i][0]-1].x, ele_v[elephant_face[i][0]-1].y, ele_v[elephant_face[i][0]-1].z); //drawing vertex
        glNormal3f(ele_vn[elephant_face[i][1]-1].x, ele_vn[elephant_face[i][1]-1].y, ele_vn[elephant_face[i][1]-1].z);//drawing normals

        glVertex3f(ele_v[elephant_face[i][2]-1].x, ele_v[elephant_face[i][2]-1].y, ele_v[elephant_face[i][2]-1].z);//drawing vertex
        glNormal3f(ele_vn[elephant_face[i][3]-1].x, ele_vn[elephant_face[i][3]-1].y, ele_vn[elephant_face[i][3]-1].z);//drawing normals

        glVertex3f(ele_v[elephant_face[i][4]-1].x, ele_v[elephant_face[i][4]-1].y, ele_v[elephant_face[i][4]-1].z);//drawing vertex
        glNormal3f(ele_vn[elephant_face[i][5]-1].x, ele_vn[elephant_face[i][5]-1].y, ele_vn[elephant_face[i][5]-1].z);//drawing normals

        glEnable(GL_TEXTURE_2D);//re-enabling textures
        glEnd();
     }
     glPopMatrix();
 }

  void trunk_draw(){
     glPushMatrix();
     glTranslatef(-1.8, -6.0, -2); //positioned right next to elephant
     TextureLoader("images/green.png", tex); //applying texture for trunk
     glScalef(.8, .8, .8); // decreasing size of trunk
     for(int i=0; i<4030; ++i){
        glBegin(GL_TRIANGLES);
        glDisable(GL_TEXTURE_2D); // disabled texture for faces
        glVertex3f(trunk_v[trunk_face[i][0]-1].x, trunk_v[trunk_face[i][0]-1].y, trunk_v[trunk_face[i][0]-1].z);//drawing vertex
        glNormal3f(trunk_vn[trunk_face[i][1]-1].x, trunk_vn[trunk_face[i][1]-1].y, trunk_vn[trunk_face[i][1]-1].z);//drawing normals

        glVertex3f(trunk_v[trunk_face[i][2]-1].x, trunk_v[trunk_face[i][2]-1].y, trunk_v[trunk_face[i][2]-1].z);//drawing vertex
        glNormal3f(trunk_vn[trunk_face[i][3]-1].x, trunk_vn[trunk_face[i][3]-1].y, trunk_vn[trunk_face[i][3]-1].z);//drawing normals

        glVertex3f(trunk_v[trunk_face[i][4]-1].x, trunk_v[trunk_face[i][4]-1].y, trunk_v[trunk_face[i][4]-1].z);//drawing vertex
        glNormal3f(trunk_vn[trunk_face[i][5]-1].x, trunk_vn[trunk_face[i][5]-1].y, trunk_vn[trunk_face[i][5]-1].z);//drawing normals

        glEnable(GL_TEXTURE_2D);//re-enable textures
        glEnd();
     }
     glPopMatrix();
 }

   void cow_draw(){
     glPushMatrix();
     glTranslatef(3.0, -4.0, -2);
     TextureLoader("images/green.png", tex); //applying texture for cow
     glScalef(.2, .2, .2); //decreasing the size of cow
     glRotatef(-50,0,1,0); //positions cow to the bottom right of the screen
     for(int i=0; i<3263; ++i){
        glBegin(GL_QUADS);
        glDisable(GL_TEXTURE_2D);//disable texture for faces
        glVertex3f(cow_v[cow_face[i][0]-1].x, cow_v[cow_face[i][0]-1].y, cow_v[cow_face[i][0]-1].z); //draw vertex
        glNormal3f(cow_vn[cow_face[i][1]-1].x, cow_vn[cow_face[i][1]-1].y, cow_vn[cow_face[i][1]-1].z);//draw normals

        glVertex3f(cow_v[cow_face[i][2]-1].x, cow_v[cow_face[i][2]-1].y, cow_v[cow_face[i][2]-1].z);//draw vertex
        glNormal3f(cow_vn[cow_face[i][3]-1].x, cow_vn[cow_face[i][3]-1].y, cow_vn[cow_face[i][3]-1].z);//draw normals

        glVertex3f(cow_v[cow_face[i][4]-1].x, cow_v[cow_face[i][4]-1].y, cow_v[cow_face[i][4]-1].z);//draw vertex
        glNormal3f(cow_vn[cow_face[i][5]-1].x, cow_vn[cow_face[i][5]-1].y, cow_vn[cow_face[i][5]-1].z);//draw normals

        glVertex3f(cow_v[cow_face[i][6]-1].x, cow_v[cow_face[i][6]-1].y, cow_v[cow_face[i][6]-1].z);//draw vertex
        glNormal3f(cow_vn[cow_face[i][7]-1].x, cow_vn[cow_face[i][7]-1].y, cow_vn[cow_face[i][7]-1].z);//draw normals

        glEnable(GL_TEXTURE_2D); //enables texture for faces
        glEnd();
     }
     glPopMatrix();
 }

 void skyBox(){
    glGenTextures(1, &tex);
    //Texture for the skybox
    glPushMatrix();
    TextureLoader("images/forest.png", tex);
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

    gluLookAt(eye_x,eye_y,eye_z, cam_x,cam_y,cam_z,0.0,1.0,0.0); //eye & cam values manipulated by keys w,a,s,d &, i,j,k,l

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

 // your code here
    //Calling functions to draw objects
   glDisable(GL_LIGHTING);
   skyBox(); //draws skybox
   glEnable(GL_LIGHTING);
   cube_draw(); //draws cube
   ven_draw(); //draws ven
   ateneam_draw(); //draws ven
   ele_draw(); //draws elephant
   trunk_draw(); //draws trunk
    cow_draw(); //draws cow


 glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case '?': //key '?' to exit application
            exit(0);
            break;

        case '/': //key '/' to toggle wireframe
		WireFrame =!WireFrame;
	       break;
        case 'q': //fly up
            eye_y +=.3;
            cam_y += 0.3;
            break;
        case 'e': //fly down
            eye_y -=.3;
            cam_y -=0.3;
            break;
        case 'a': //move left
            eye_x -= 0.1;
            cam_x -=0.1;
            break;
        case 'd': //move right
           eye_x +=0.1;
           cam_x += 0.1;
            break;
        //keys 'w' & 's' to change the x position of cube
        case 'w': // move forward
            eye_z -=0.1;
            cam_z -= 0.1;
            break;
        case 's': //move backwards
            eye_z +=0.1;
            cam_z += 0.1;
            break;
        case 'j': //pan left
            cam_x -= 0.5;
            break;
        case 'l': //pan right
            cam_x +=0.5;
            break;
        case 'i'://pan up
            cam_y += 0.1; //looks to the top by increasing cam_y
            break;
        case 'k'://pan down
            cam_y -=0.1; //looks to the bottom by decreasing cam_y
            break;
        case ' ': // Space bar to reset viewing angles
                 cam_x = 0;
                cam_y = -2.5;
                cam_z = 0;
                eye_x = 0;
                eye_y = -4.2;
                eye_z =4.6;
                sky_x_angle=0;
                sky_y_angle=0;
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
    cam_y = -2.5;
    cam_z = 0;
    eye_x = 0;
    eye_y = -4.2;
    eye_z =4.6;

    sky_x_angle=0;
    sky_y_angle=180;

    scale_sky = 5;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

void loadObjs(string fileName, int **F, vector<vec3> &v, vector<vec3> &vn, vector<vec2> &vt, bool printOut){
    fstream objFile;
    objFile.open(fileName);

    if(objFile.is_open()){ //opens the object file
        cout<<"File opened! Parsing..."<<endl;
    }else{
        cout<<"Something went wrong with loading the file!"<<endl;
    }
    string curChar; //reads current character
    string token; //get string tokens

    int vi, vni, vti, fi; //vertex index, vertex normal index, vertex texture index, face index
     vi = vni= vti= fi= 0;

     string x, y, z;
     int values[4]; //to store predefined values of faces, vertex, vertex normals, and vertex textures
     while(getline(objFile, curChar)){ //loop to read lines in object file
        stringstream ss(curChar);

        if(curChar.empty()){ //ignore empty lines
            continue;
        }else if(curChar.front() == '#'){ //ignore lines that start with '#'
            continue;
        }else if(curChar.front() == 'd'){ //stores default values for vertex, vertex textures, normals, and faces
            getline(ss, token, ' ');
            getline(ss, token, ' '); //skipping spaces
            values[0] = atoi(token.c_str());
            if(printOut){
                cout<<"v = tableSize[0]= "<< values[0]<<endl; //value of vertex
            }
            getline(ss, token, ' ');
            values[1] = atoi(token.c_str());
            if(printOut){
                cout<<"vt = tableSize[1]= "<< values[1]<<endl; //value of vertex textures
            }
            getline(ss, token, ' ');
            values[2] = atoi(token.c_str());
            if(printOut){
                cout<<"vn = tableSize[2]= "<< values[2]<<endl; //value of normals
            }
            getline(ss, token, ' ');
            values[3] = atoi(token.c_str());
            if(printOut){
                cout<<"f = tableSize[3]= "<< values[3]<<endl; //value of faces
                cout<<"Done reading for values!"<<endl;
            }
            continue;
        }else if(curChar.front() == 'g'){ //ignore lines that start with 'g'
            continue;
        }else if(curChar.front() == 's'){ //ignore lines that start with 's'
            continue;
        }
        while(getline(ss, token, ' ')){ //loop for vertex, normals, and texture values
            if(token == "v"){ // v indicates vertex in object file
                getline(ss, token, ' ');
                if(token.empty()){ //if retrieved NULL, move on to next token
                    getline(ss, token, ' '); //get x value
                }
                x = token; //stores x

                getline(ss, token, ' ');
                if(token.empty()){
                    getline(ss, token, ' '); //get y value
                }
                y= token; //stores y

                getline(ss, token, ' ');
                if(token.empty()){
                    getline(ss, token, ' '); //get z value
                }
                z=token; //stores z

                v.push_back(vec3(atof(x.c_str()), atof(y.c_str()), atof(z.c_str()))); //vertex structure with three values x,y,and z
                if(printOut){
                    cout<<" x: "<< v[vi].x<<" y: "<<v[vi].y<<" z: "<<v[vi].z<<endl;
                }
                vi+=1;
            }

            if(token == "vn"){ // vn indicates normals
                getline(ss, token, ' ');
                if(token.empty()){
                    getline(ss, token, ' '); //get x
                }
                x=token; // stores x

                 getline(ss, token, ' ');
                if(token.empty()){
                    getline(ss, token, ' '); // get y
                }
                y=token; // stores y

                 getline(ss, token, ' ');
                if(token.empty()){
                    getline(ss, token, ' '); // get z
                }
                z=token; // stores z

                vn.push_back(vec3(atof(x.c_str()), atof(y.c_str()), atof(z.c_str()))); // vertex normal structure with three values x, y, and z
                if(printOut){
                    cout<<"vn = "<<" x: "<< vn[vni].x<<" y: "<<vn[vni].y<<" z: "<<vn[vni].z<<endl;
                }
                vni+=1;
            }

            if(token == "vt"){ //vt indicates vertex texture
                getline(ss, token, ' ');
                if(token.empty()){
                    getline(ss, token, ' '); //get x
                }
                x = token; //stores x

                getline(ss, token, ' ');
                if (token.empty()) {
                    getline(ss, token, ' '); // get y
                }
                y= token; //stores y

                vt.push_back(vec2(atof(x.c_str()), atof(y.c_str()))); //vertex texture structure with only two values x and y

                if(printOut){
                    cout<<"vt = "<<" x: "<< vt[vti].x<<" y: "<<vt[vti].y<<endl;
                }
                vti+=1;
            }

            if(token == "f"){ //f indicates faces
                 int numSlots = 9; //default number of faces

                int cowFaces = 3263; //number of faces for cow

                int trunkFaces = 4030; //number of faces for trunk
                int eleFaces = 10150; // number of faces for elephant
                int venFaces = 43357; // number of faces for ven

                if(values[3] == trunkFaces || values[3] == eleFaces || values[3] == venFaces){ //trunk, elephant, and ven have 6 faces
                    numSlots = 6;
                }
                if(values[3] == cowFaces){ //cow has 8 faces
                    numSlots = 8;
                }
//            cout<<"number of faces: "<<numSlots<<endl;

                char separators[] = " /s,\t\n"; // detects separators in between values
                char *token, *c_array; //for token and characters

                string str_obj(curChar);
                c_array = &str_obj[0];
                token = strtok(c_array, separators); //first token
                F[fi] = new int[numSlots]; //initializes 2D array of faces

                for(int i=0; i<numSlots; i++){ //store values based on the number of faces
                    token = strtok(NULL, separators); //get next token
                    F[fi][i] = atoi(token); //convert to integer and store
                    if(printOut){
                        cout<<"inserted face "<<F[fi][i]<<" at F["<<fi<<"]["<<i<<"]"<<endl;
                    }
                }
                fi+=1;
            }
        }
     }
     cout<<"End file read!"<<endl;
     objFile.close();
}



/* Program entry point */

int main(int argc, char *argv[])
{
    loadObjs("obj_load/cube.obj", cube_face, cube_v, cube_vn, cube_vt, false);
    loadObjs("obj_load/ven-2.obj", ven_face, ven_v, ven_vn, ven_vt_empty, false);
    loadObjs("obj_load/ateneam-2.obj", ateneam_face, ateneam_v, ateneam_vn, ateneam_vt, false);
    loadObjs("obj_load/ele.obj", elephant_face, ele_v, ele_vn, ele_vt_empty, false);
    loadObjs("obj_load/trunk.obj", trunk_face, trunk_v, trunk_vn, trunk_vt_empty, false);
    loadObjs("obj_load/cow.obj", cow_face, cow_v, cow_vn, cow_vt_empty,false);


    glutInit(&argc, argv);

    glutInitWindowSize(1200,900);
    glutInitWindowPosition(50,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Model Loader");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
