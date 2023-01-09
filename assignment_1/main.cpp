#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

// creating 10 constants
float a, b, c, d, e, f; //six constants for transformation
float x, y, x0, y0; // 4 constants for x & y coordinates
int it; // integer iterator

void resize(int width, int height) //width and height of window
{
    if(width<=height) // to resize the window dynamically based on width & height
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
 x0 = 1; //initializing origin x & y
 y0 = 1;
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
  // your code here
  for(int i=0; i<200000; ++i){ //200 thousand iterations for the function system
    it = rand()%100 + 1; //generates a random number from 1-100
    // if-else statements to perform 1 of the 4 transformation functions(f1, f2, f3, f4) on a point based on a given random number
    if(it <= 7){ //choose f3 7% of the time
        a= -0.15;
        b= 0.26;
        c = 0.28;
        d = 0.24;
        e = 0;
        f = 0.44;
    } else if (it <= 14){ //choose f4 7% of the time
        a= 0.2;
        b =0.23;
        c =-0.26;
        d =0.22;
        e =0;
        f=1.6;
    } else if (it <= 99){// choose f2 85% of the time
        a= 0.85;
        b = -0.04;
        c =0.04;
        d =0.85;
        e =0;
        f=1.6;
    }else { //choose f1 for the final 1% of the time
        a= 0;
        b =0;
        c =0;
        d =0.16;
        e =0;
        f=0;
    }
    // Performs x & y calculations using the 6 constants determined from if-else statements above for glVertex3f to plot in window
    x = a * x0 + c * y0 + e;
    y = b * x0 + d * y0 + f;

    glColor3f(0, 1, 0); // selecting the color to plot using RGB values for each argument ("0,1,0" = green)
    glBegin(GL_POINTS); // N points are drawn in window
    glVertex3f(x, y, 0); //pixels mapped in the window
    glEnd(); //delimit the vertices
    // setting new x and y coordinates for each iteration
    x0 = x;
    y0 = y;
  }
  glFlush ();                           // clear buffer
}


void key(unsigned char key, int x, int y) //some action taken on key press down
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay(); //calls the display function to re-render image based on key action
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}
