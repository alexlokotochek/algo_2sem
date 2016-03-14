#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include "opengl.h"
#include <math.h>
#include <random>
#include <chrono>
using namespace std;
vector<Point> points;
vector<Edge> tree;

struct colourPoint {
    double a;
    double b;
    double c;
};
vector<colourPoint> colours;
/*! glut display callback function.  Every time the window needs to be drawn,
 glut will call this function.  This includes when the window size
 changes, or when another window covering part of this window is
 moved so this window is uncovered.
 */
void DrawPoints(double x1, double y1, double region)
{
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    glPointSize(4);
    glColor3f(colours[region].a, colours[region].b, colours[region].c);
    glBegin(GL_POINTS);
    
    glVertex2f(x1, y1);
    glEnd();
}

void DrawEdges(int a, int b)
{
    glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);
    glBegin(GL_LINES);
    glVertex2f(points[a].x+512, points[a].y+512);
    glVertex2f(points[b].x+512, points[b].y+512);
    glEnd();
}

void display()
{
    /* clear the color buffer (resets everything to black) */
    glClear(GL_COLOR_BUFFER_BIT);
    /* set painting color to black */
    glColor3f(0, 0, 0);
    
    /* рисуем точки */
    for (int i = 0; i < points.size(); ++i){
        DrawPoints(points[i].x + 512, points[i].y + 512, points[i].region);
    }
    
    glColor3f(0, 0, 0);
    /* рисуем ребра */
    for (int i = 0; i < tree.size(); ++i){
        DrawEdges(tree[i].a, tree[i].b);
    }
    
    
    
    
    /* swap the back and front buffers so we can see what we just drew */
    glutSwapBuffers();
}

/*! glut reshape callback function.  GLUT calls this function whenever
 the window is resized, including the first time it is created.
 You can use variables to keep track the current window size.
 */
void reshape(int width, int height)
{
    /* tell OpenGL we want to display in a recangle that is the
     same size as the window
     */
    glViewport(0,0,width,height);
    
    /* switch to the projection matrix */
    glMatrixMode(GL_PROJECTION);
    
    /* clear the projection matrix */
    glLoadIdentity();
    
    /* set the camera view, orthographic projection in 2D */
    gluOrtho2D(0,width,0,height);
    
    /* switch back to the model view matrix */
    glMatrixMode(GL_MODELVIEW);
}

void visualizer(vector<Point> &data, vector<Edge> &edges, int regions)
{
    points = data;
    tree = edges;
    
    colours.resize(regions);
    
    uniform_real_distribution<double> unif(0.00000001,0.99999999);
    default_random_engine re;
    for (int i = 0; i < regions; ++i){
//        double lower_bound = 0;
//        double upper_bound = 10000;
//        std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
//        std::default_random_engine re;
//        double a_random_double = unif(re);
        colours[i].a = unif(re);
        colours[i].b = unif(re);
        colours[i].c = unif(re);

    }
    
    int argc = 0;
    char** argv = new(char*);
    glutInit(&argc, argv);
    
    /* set the window size to 512 x 512 */
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(300, 300);
    
    /* set the display mode to Red, Green, Blue and Alpha
     allocate a depth buffer
     enable double buffering
     */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* create the window (and call it Lab 1) */
    glutCreateWindow("Result");
    
    /* set the glut display callback function
     this is the function GLUT will call every time
     the window needs to be drawn
     */
    glutDisplayFunc(display);
    
    /* set the glut reshape callback function
     this is the function GLUT will call whenever
     the window is resized, including when it is
     first created
     */
    glutReshapeFunc(reshape);
    
    /* set the default background color to black */
    glClearColor(1,1,1,1);
    
    /* enter the main event loop so that GLUT can process
     all of the window event messages
     */
    glutMainLoop();
}
