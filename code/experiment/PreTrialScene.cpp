#include "stdafx.h"
#include "PreTrialScene.h"
#include <vector>
#include <algorithm>



PreTrialScene::PreTrialScene()
{
}

PreTrialScene::~PreTrialScene(void)
{
}

BOOL PreTrialScene::startScene()
{
    for(vector<Screen *>::iterator it = this->screens.begin();
        it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->cancelKMBinds();
    }   

    for(vector<Screen *>::iterator it = this->screens.begin();
        it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->clear();
    }  

    glutDisplayFunc(Scene::dispatchSceneRender);
    return TRUE;
}

BOOL PreTrialScene::renderScene()
{
    string message; 

    message = "Press spacebar for the next trial";
    this->screens[0]->displayString(message, 0.0, 0.0);
	
    return TRUE;
}

BOOL PreTrialScene::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    return TRUE;
}
