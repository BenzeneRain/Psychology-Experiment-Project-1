#include "stdafx.h"
#include "PreTrialScene.h"
#include "Experiment.h"
#include "Trial.h"

#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

PreTrialScene::PreTrialScene()
{
    this->status = TRUE;
}

PreTrialScene::~PreTrialScene(void)
{
}

BOOL PreTrialScene::startScene()
{
    // Cancel all keyboards and mouses events bindings
    // and reset all other functions, e.g. display
    this->rScreen.resetAllFunc();

    // Clear the screen
    this->rScreen.clear();

    Experiment *pExperi = Experiment::getInstance(NULL);

    if(pExperi->isNewSection() || pExperi->isNewBlock())
    {
        // set display function and reshape function
        this->rScreen.setDisplayFunc(Scene::dispatchSceneRender);
        this->rScreen.setReshapeFunc(Scene::dispatchReshape);
     
        // Bind new keyboards and mouses events
        this->rScreen.setKeyboardFunc(Scene::dispatchKeyboardEvent);
        this->rScreen.setKeyboardSpecialFunc(Scene::dispatchKeyboardSpecialEvent);
        this->rScreen.setMouseFunc(Scene::dispatchMouseEvent);

        // Start running the scene
        // FIX: This is actually a run design if there are multiple screens
        // e.g. the program will be blocked for each run()
        this->initDisplay();
        this->rScreen.run();
    }

    return this->status;
}

string PreTrialScene::buildString()
{
    ostringstream ossMessage;

    //string message("Press spacebar for the next trial\n");
    //string message;
    //message.clear();

    Experiment *pExperi = Experiment::getInstance(NULL);

    //if(pExperi->isNewSection())
    //{
        ossMessage << "Progress: Section " << pExperi->currSecNo + 1 << "/"
            << pExperi->maxSecNo;
    //}

    if(pExperi->isNewBlock())
    {
        ossMessage << ", Block " << pExperi->currBlockID + 1 << "/"
            << pExperi->blocksPerSec << endl;
    }
    else
    {
        ossMessage << endl;
    }

    return ossMessage.str();
}

BOOL PreTrialScene::renderScene()
{
    string message(this->buildString());

    glColor3f(1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    this->rScreen.displayString(message, 0.0, 0.0);
	
    this->rScreen.render();
    return TRUE;
}

// FIX: We should move it to the Screen class
BOOL PreTrialScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
}

BOOL PreTrialScene::initDisplay()
{
    this->reshape(this->rScreen.rDevMode.dmPelsWidth,
            this->rScreen.rDevMode.dmPelsHeight); 

    // Disable texture
    glDisable(GL_TEXTURE_2D);

    // Disable multisample
    glDisable(GL_MULTISAMPLE_ARB);

    return TRUE;
}

BOOL PreTrialScene::handleKeyboardEvent(unsigned char key, int x, int y)
{
    switch(key)
    {
        case VK_SPACE:
            {
                this->rScreen.stopped = TRUE;
                break;
            }
        case VK_ESCAPE:
            {
               int ret = MessageBox(this->rScreen.hWnd(), "Do you want to abort the experiment?",\
                   "Abort", MB_YESNO | MB_ICONWARNING);
                if(ret == IDYES)
                {
                    this->rScreen.stopped = TRUE;
                    this->status = FALSE;
                }

                break;
            }
        default:
            break;
    }
    return TRUE;
}

BOOL PreTrialScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    return TRUE;
}

BOOL PreTrialScene::handleMouseEvent(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        this->rScreen.stopped = TRUE;
    return TRUE;
}

BOOL PreTrialScene::handleMouseMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL PreTrialScene::handleMousePassiveMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL PreTrialScene::handleTimerEvent(int timerID)
{
    return TRUE;
}
