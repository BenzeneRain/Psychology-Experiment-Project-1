#include "stdafx.h"
#include "Screen.h"
#include "Scene.h"
#include "experiment.h"
#include "texture.h"
#include <sstream>

using namespace std;


Screen::Screen(DEVMODE& devMode):
    rDevMode(devMode)
{
    stopped = TRUE;
}

Screen::~Screen(void)
{
    glDeleteTextures(this->texNo, this->texIDs);
    delete this->texIDs;
}

// This part can be seen as SetupRC with other initializations
BOOL Screen::initGlut(UINT displayMode, string title)
{
    // Change the display settings to the selected resolution and refresh rate
    if(ChangeDisplaySettings(&rDevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        MessageBox(NULL, (LPCSTR)"Cannot change to selected desktop resolution.", NULL, MB_OK | MB_ICONSTOP);
        return FALSE;
    }

    // __argc and __argv are global variables
    // storing the arguments for the program
    glutInit(&__argc, __argv);
    glewInit();
    this->displayMode = displayMode;

    // Init the glut 
    glutInitDisplayMode(displayMode);
    glutInitWindowSize(rDevMode.dmPelsWidth, rDevMode.dmPelsHeight);
    glutCreateWindow(title.c_str());
    glutFullScreen();

    // Set the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearStencil(0x0);

    glEnable(GL_DEPTH_TEST);

    // Setting up textures environment
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    return TRUE;
}

BOOL Screen::initTextures(vector<texture_t *>& textures)
{
    this->texNo = textures.size();

    this->texIDs = new GLuint[this->texNo];
    memset(this->texIDs, 0, sizeof(GLuint) * this->texNo);
    glGenTextures(this->texNo, this->texIDs);

    for(unsigned int i = 0; i < textures.size(); i ++)
    {
        if(textures[i]->type == 'T')
        { 
            glBindTexture(GL_TEXTURE_2D, this->texIDs[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  

            BITMAP bm;
            HBITMAP hBitmap;

            hBitmap = textures[i]->hBitmap;
            GetObject(hBitmap, sizeof(bm), &bm);

            BYTE *imageBuffer = new BYTE[bm.bmWidthBytes * bm.bmHeight];
            GetBitmapBits(hBitmap, bm.bmWidthBytes * bm.bmHeight, imageBuffer);

            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bm.bmWidth, bm.bmHeight,
                    GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
            delete imageBuffer;
        }
    }

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

    glutKeyboardUpFunc(NULL);

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

// Call this before actual rendering
void Screen::render()
{
    // TODO: caculate the FPS
      


    // If we set double buffer, then swap the buffer
    // If we set single buffer, just flush the buffer
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

BOOL Screen::setTimerFunc(unsigned int msecs, void (*func)(int timerID), int timerID)
{
    if(msecs == 0)
        msecs = 1;
    glutTimerFunc(msecs, func, timerID);
    return TRUE;
}

void Screen::nullTimerFunc(int timerID)
{
    return;
}

BOOL Screen::setIdleFunc(void (*func)(void))
{
    glutIdleFunc(func);
    return TRUE;
}

// Clear the screen
BOOL Screen::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->render();

    return TRUE;
}

// Use this to reset all the glut functions
BOOL Screen::resetAllFunc()
{
    this->cancelKMBinds();
    //this->setDisplayFunc(NULL); 
    //this->setReshapeFunc(NULL);
    this->setTimerFunc(0, Screen::nullTimerFunc, 0); 
    //this->setIdleFunc(NULL);
    
    return TRUE;
}

// Call this after everything ready
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
