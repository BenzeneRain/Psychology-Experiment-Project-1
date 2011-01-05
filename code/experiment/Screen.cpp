#include "stdafx.h"
#include "Screen.h"
#include "experiment.h"
#include <sstream>

using namespace std;

Screen::Screen()
{
    stopped = TRUE;
}

Screen::~Screen(void)
{
}

// This part can be seen as SetupRC with other initializations
BOOL Screen::initGlut(DEVMODE devMode,
        UINT displayMode, string title)
{
    // Change the display settings to the selected resolution and refresh rate
    if(ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        MessageBox(NULL, "Cannot change to selected desktop resolution.", NULL, MB_OK | MB_ICONSTOP);
        return FALSE;
    }

    // __argc and __argv are global variables
    // storing the arguments for the program
    glutInit(&__argc, __argv);
    this->displayMode = displayMode;

    // Init the glut 
    glutInitDisplayMode(displayMode);
    glutInitWindowSize(devMode.dmPelsWidth, devMode.dmPelsHeight);
    glutCreateWindow(title.c_str());
    glutFullScreen();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return TRUE;
}

// Cancel all the keyboard and mouse events binding
BOOL Screen::cancelKMBinds()
{
    glutKeyboardFunc(NULL);
    glutSpecialFunc(NULL);

    glutMouseFunc(NULL);
    glutMotionFunc(NULL);
    glutPassiveMotionFunc(NULL);
    return TRUE;
}

BOOL Screen::setKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
    glutKeyboardFunc(func);
    return TRUE;
}

BOOL Screen::setKeyboardSpecialFunc(void (*func)(int key, int x, int y))
{
    glutSpecialFunc(func);
    return TRUE;
}

BOOL Screen::setMouseFunc(void (*func)(int button, int state, int x, int y))
{
    glutMouseFunc(func);
    return TRUE;
}

BOOL Screen::setMouseMotionFunc(void (*func)(int x, int y))
{
    glutMotionFunc(func);
    return TRUE;
}

BOOL Screen::setMousePassiveMotionFunc(void (*func)(int x, int y))
{
    glutPassiveMotionFunc(func);
    return TRUE;
}


// Call this to display the string
BOOL Screen::displayString(string str, float x, float y)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char *>(str.c_str()));

    this->render();

    return TRUE;
}

void Screen::render()
{
    if(((this->displayMode) & GLUT_DOUBLE) != 0)
    {
        glutSwapBuffers();
        glutPostRedisplay();
    }
    else
    {
        glFlush();
    }
}

BOOL Screen::setDisplayFunc(void (*displayFunc)(void))
{
    glutDisplayFunc(displayFunc);
    return TRUE;
}

BOOL Screen::setReshapeFunc(void (*func)(int w, int h))
{
    glutReshapeFunc(func);
    return TRUE;
}

// Clear the screen
BOOL Screen::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->render();

    return TRUE;
}

BOOL Screen::run()
{
   this->stopped = FALSE; 

   while(this->stopped == FALSE)
   {
       glutMainLoopEvent();
   }

   return TRUE;
}

// the function is only for test purpose
void Screen::testRenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
}
