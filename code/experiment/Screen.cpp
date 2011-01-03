#include "stdafx.h"
#include "Screen.h"
#include "experiment.h"
#include <sstream>

using namespace std;

Screen::Screen()
{
}

Screen::~Screen(void)
{
}

// This part can be seen as SetupRC with other initializations
BOOL Screen::initGlut(DEVMODE devMode,
        UINT displayMode, string title)
{
    if(ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        MessageBox(NULL, "Cannot change to selected desktop resolution.", NULL, MB_OK | MB_ICONSTOP);
        return FALSE;
    }

    // __argc and __argv are global variables
    // storing the arguments for the program
    glutInit(&__argc, __argv);
    this->displayMode = displayMode;

    glutInitDisplayMode(displayMode);
    glutInitWindowSize(devMode.dmPelsWidth, devMode.dmPelsHeight);
    glutCreateWindow(title.c_str());
    glutFullScreen();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    if(!Experiment::debug)
    {
        glutDisplayFunc(Experiment::proceedExperiment);
    }
    else
    {
        glutDisplayFunc(Screen::testRenderScene);
    }
    glutMainLoop();
    return TRUE;
}

BOOL Screen::cancelKMBinds()
{
    glutKeyboardFunc(NULL);
    glutMouseFunc(NULL);
    return TRUE;
}

BOOL Screen::displayString(string str, float x, float y)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    glutBitmapString(GLUT_BITMAP_9_BY_15, reinterpret_cast<const unsigned char *>(str.c_str()));

    glutSwapBuffers();
    glutPostRedisplay();

    return TRUE;
}

BOOL Screen::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
