/*
Edmund Wei Jet, Chin
Calculating Normals for Bunny
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

//bunny
int **bunny_face = new int* [30338]; // 2D array for bunny faces
vector<vec3> b_v; // to store bunny vertices
vector<vec3> b_vn; // to store bunny normals
vector<vec2> b_vt; // to store bunny texture indices

bool WireFrame= false; //boolean toggle for wireframe
float Wwidth, Wheight; //width & height for reshape function

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
GLfloat position[] = { 0.0f, 0.0f, 3.5f, 1.0f }; //light position used in display function

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
GLfloat rx; // rotation value for bunny

GLfloat angleX; //angle value for lighting on the bunny
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

	gluPerspective (45.0f,Ratio,0.1f, 100.0f); //respective fovy, aspect, near, far values for better perspective
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

 void getNormals(){ // to calculate the surface normals for the bunny
    vector<vec3> normals;
    vector<vector<int>> vertex(b_v.size());

    for(int i=0; i<30338; i++){ //30338 faces
        //storing of vertices for nine faces
        vec3 l1(b_v[bunny_face[i][0] - 1].x, b_v[bunny_face[i][0] - 1].y, b_v[bunny_face[i][0] - 1].z);
        vec3 l2(b_v[bunny_face[i][3] - 1].x, b_v[bunny_face[i][3] - 1].y, b_v[bunny_face[i][3] - 1].z);
        vec3 l3(b_v[bunny_face[i][6] - 1].x, b_v[bunny_face[i][6] - 1].y, b_v[bunny_face[i][6] - 1].z);

        //using the stored vertices above to calculate vectors u & v
        vec3 u(l2.x - l1.x, l2.y - l1.y, l2.z - l1.z);
        vec3 v(l3.x - l1.x, l3.y - l1.y, l3.z - l1.z);

        //cross product calculations to get vector normal
        float crossX = (u.y * v.z) - (u.z * v.y);
        float crossY = (u.z * v.x) - (u.x * v.z);
        float crossZ = (u.x * v.y) - (u.y * v.x);

        float length = sqrt((crossX * crossX) + (crossY * crossY) + (crossZ * crossZ)); //vector length calculation

        //normalizing the result by dividing values from cross product with vector length
        float normCrossX = (crossX / length);
        float normCrossY = (crossY / length);
        float normCrossZ = (crossZ / length);

        //storing final surface normal calculation for one face
        vec3 normalized(normCrossX, normCrossY, normCrossZ);
        normals.push_back(normalized);

        //storing of faces that share the same vertex
        vertex[bunny_face[i][0] - 1].push_back(i);
        vertex[bunny_face[i][3] - 1].push_back(i);
        vertex[bunny_face[i][6] - 1].push_back(i);
    }

    for(int i=0; i<b_v.size(); ++i){
        float s_x = 0;
        float s_y = 0;
        float s_z = 0;
        vector<int> shared_face = vertex[i];
        vector<vec3> shared_normals;


        for (int i = 0; i < shared_face.size(); i++)
            shared_normals.push_back(normals[shared_face[i]]); //get surface normal calculations performed above for shared faces of each vertex

            for(int i=0; i<shared_normals.size(); ++i){ //retrieving x, y, & z values
                s_x += shared_normals[i].x;
                s_y += shared_normals[i].y;
                s_z += shared_normals[i].z;
            }

            float s_length = sqrt((s_x * s_x) + (s_y * s_y) + (s_z * s_z)); //vector length calculation

            //normalizing x, y, & z values
            float final_x_norm = (s_x / s_length);
            float final_y_norm = (s_y / s_length);
            float final_z_norm = (s_z / s_length);
            vec3 norm2(final_x_norm, final_y_norm, final_z_norm);

            //final normal calculations for each vertex is stored
            b_vn.push_back(norm2);
    }
 }

 void bunny_draw(){ //function to create bunny object
    glPushMatrix();
    TextureLoader("images/orange.png", tex); //applying texture to bunny
    glRotatef(rx, 0, 1, 0); // rx value to rotate around the y-axis
    glTranslatef(0,-4,0); //placing bunny directly in front of viewer
    glScalef(.4 / (150), .4 / (150), .4 / (150)); // decreasing the size of bunny
    int z=0;// to attain next three values that have been stored

    for (int i = 0; i < 30338; i++) {
        glBegin(GL_TRIANGLES);
        for (int c = 0, z = 0; c <= 2; ++c) {
            glNormal3f(b_vn[bunny_face[i][2 + z] - 1].x, b_vn[bunny_face[i][2 + z] - 1].y, b_vn[bunny_face[i][2 + z] - 1].z); //drawing of normals
            glTexCoord2f(b_vt[bunny_face[i][1 + z] - 1].x, b_vt[bunny_face[i][1 + z] - 1].y); // drawing of textures
            glVertex3f(b_v[bunny_face[i][0 + z] - 1].x, b_v[bunny_face[i][0 + z] - 1].y, b_v[bunny_face[i][0 + z] - 1].z); //drawing of vertices
            z+=3; //nine values for face
        }
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
     glPushMatrix();
        glRotatef(angleX, 1, 0, 0);  // rotates x through angle angleX, controlled by mouse left and right click
        glTranslatef(.7, 0, 0); // light translation
        glColor3f(1.0, .85, 0.7); // warmer color
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();

    //Calling functions to draw objects
    glDisable(GL_LIGHTING);
    skyBox(); //draws skybox
    glEnable(GL_LIGHTING);
    bunny_draw(); //draws bunny


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
                cam_y = -4;
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

void mouse(int btn, int state, int x, int y){ // invokes mouse functions

    switch(btn){
        case GLUT_LEFT_BUTTON: //left click to rotate bunny

        if(state==GLUT_DOWN){
                rx += 8;
            }
            break;

        case GLUT_RIGHT_BUTTON://right click increases light angle

        if(state==GLUT_DOWN){
                angleX += 8;
            }
            break;
    }
     glutPostRedisplay();
};

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
    angleX -= .5;
    glutPostRedisplay();
}

static void init(void)
{
    //initializing the variables declared above
    cam_x = 0;
    cam_y = -4;
    cam_z = 0;
    eye_x = 0;
    eye_y = -4.2;
    eye_z =4.6;
    rx=0;

    angleX = 0;

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
     getNormals();
     cout<<"End file read!"<<endl;
     objFile.close();
}



/* Program entry point */

int main(int argc, char *argv[])
{
    loadObjs("obj_load/bunny-3.obj", bunny_face, b_v, b_vn, b_vt,false);


    glutInit(&argc, argv);

    glutInitWindowSize(1200,900);
    glutInitWindowPosition(50,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Calculating Normals");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse); //for mouse operations
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
