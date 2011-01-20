#include "StdAfx.h"
#include "PostExperimentScene.h"

#include <vector>
#include <algorithm>
#include <sstream>

PostExperimentScene::PostExperimentScene(void)
{
}

PostExperimentScene::~PostExperimentScene(void)
{
}

BOOL PostExperimentScene::startScene()
{
    // Cancel all keyboards and mouses events bindings
    // and reset all other functions, e.g. display
    this->rScreen.resetAllFunc();

    // Clear the screen
    this->rScreen.clear();

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

    return this->status;
}

string PostExperimentScene::buildString()
{
    string message("This is the end of the experiment. Thank you!");
    return message;
}

BOOL PostExperimentScene::renderScene()
{ 
    string message; 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    message = this->buildString();
    this->rScreen.displayString(message, 0.0, 0.0);
	
    this->rScreen.render();
    return TRUE;
}

// FIX: We should move it to the Screen class
BOOL PostExperimentScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
}

BOOL PostExperimentScene::initDisplay()
{
    this->reshape(this->rScreen.rDevMode.dmPelsWidth,
            this->rScreen.rDevMode.dmPelsHeight); 

    // Disable texture
    glDisable(GL_TEXTURE_2D);

    // Disable multisample
    glDisable(GL_MULTISAMPLE);

    return TRUE;
}

BOOL PostExperimentScene::handleKeyboardEvent(unsigned char key, int x, int y)
{
    switch(key)
    {
        case VK_SPACE:
            {
                this->rScreen.stopped = TRUE;
                break;
            }
        default:
            break;
    }
    return TRUE;
}

BOOL PostExperimentScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    return TRUE;
}

BOOL PostExperimentScene::handleMouseEvent(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        this->rScreen.stopped = TRUE;
    return TRUE;
}

BOOL PostExperimentScene::handleMouseMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL PostExperimentScene::handleMousePassiveMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL PostExperimentScene::handleTimerEvent(int timerID)
{
    return TRUE;
}
