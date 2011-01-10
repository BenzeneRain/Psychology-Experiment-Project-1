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
    for(vector<Screen *>::iterator it = this->screens.begin();
        it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->cancelKMBinds();
    }   

    // Clear the screen
    for(vector<Screen *>::iterator it = this->screens.begin();
        it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->clear();
    }  

    // set display function and reshape function
    for(vector<Screen *>::iterator it = this->screens.begin();
            it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->setDisplayFunc(Scene::dispatchSceneRender);
        ((Screen *)*it)->setReshapeFunc(Scene::dispatchReshape);
    }  

    // Bind new keyboards and mouses events
    for(vector<Screen *>::iterator it = this->screens.begin();
            it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->setKeyboardFunc(Scene::dispatchKeyboardEvent);
        ((Screen *)*it)->setKeyboardSpecialFunc(Scene::dispatchKeyboardSpecialEvent);
    }  

    // Start running the scene
    // FIX: This is actually a run design if there are multiple screens
    // e.g. the program will be blocked for each run()
    for(vector<Screen *>::iterator it = this->screens.begin();
            it != this->screens.end(); it ++)
    {
        Screen *pScr = (Screen *) *it;
        this->initDisplay(*pScr);
        pScr->run();
    }  

    return TRUE;
}

string PostExperimentScene::buildString()
{
    string message("This is the end of the experiment. Thank you!");
    return message;
}

BOOL PostExperimentScene::renderScene()
{
    string message; 

    message = this->buildString();
    this->screens[0]->displayString(message, 0.0, 0.0);
	
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

BOOL PostExperimentScene::initDisplay(Screen& scr)
{
    this->reshape(scr.rDevMode.dmPelsWidth, scr.rDevMode.dmPelsHeight); 

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
                for(vector<Screen *>::iterator it = this->screens.begin();
                        it != this->screens.end(); it ++)
                {
                    ((Screen *)*it)->stopped = TRUE;
                }  
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
