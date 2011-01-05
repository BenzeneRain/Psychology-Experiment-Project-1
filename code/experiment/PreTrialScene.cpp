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
}

PreTrialScene::~PreTrialScene(void)
{
}

BOOL PreTrialScene::startScene()
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
      ((Screen *)*it)->run();
    }  

    return TRUE;
}

string PreTrialScene::buildString()
{
    string message("Press spacebar for the next trial");

    Experiment *pExperi = Experiment::getInstance(NULL);

    if(pExperi->isNewSection())
    {
        ostringstream ossMessage;

        ossMessage << message << endl
            << "Progress: Section " << pExperi->currSecNo + 1 << "/"
            << pExperi->maxSecNo << endl;

        message = ossMessage.str();
    }

    return message;
}

BOOL PreTrialScene::renderScene()
{
    string message; 

    message = this->buildString();
    this->screens[0]->displayString(message, 0.0, 0.0);
	
    return TRUE;
}

// FIX: We should move it to the Screen class
BOOL PreTrialScene::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    return TRUE;
}


BOOL PreTrialScene::handleKeyboardEvent(unsigned char key, int x, int y)
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

BOOL PreTrialScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    return TRUE;
}

BOOL PreTrialScene::handleMouseEvent(int button, int state, int x, int y)
{
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
