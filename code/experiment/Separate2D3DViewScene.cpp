#include "stdafx.h"
#include "Separate2D3DViewScene.h"
#include "Trial.h"
#include "Experiment.h"

Separate2D3DViewScene::Separate2D3DViewScene(void)
{
}

Separate2D3DViewScene::~Separate2D3DViewScene(void)
{
}

BOOL Separate2D3DViewScene::startScene()
{
    // Cancel all keyboards and mouses events bindings
    for(vector<Screen *>::iterator it = this->screens.begin();
        it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->cancelKMBinds();
    }   

    // Get the random object
    this->pObj = this->getRandObj(); 
    this->pObj->setRandPara();
    if(Experiment::debug)
    {
        MessageBox(NULL, (this->pObj->getObjName()).c_str(), NULL, NULL);
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
    return TRUE;
}

BOOL Separate2D3DViewScene::renderScene()
{
    return TRUE;
}

BOOL Separate2D3DViewScene::reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    return TRUE;
}


BOOL Separate2D3DViewScene::handleKeyboardEvent(unsigned char key, int x, int y)
{
    switch(key)
    {
        case VK_SPACE:
            {
                Trial *pTrial = Trial::getInstance();
                pTrial->proceedNextScene();
                break;
            }
        default:
            break;
    }
    return TRUE;
}

BOOL Separate2D3DViewScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            pObj->adjustAsptRatio(0.1f);
            break;
        case GLUT_KEY_DOWN:
            pObj->adjustAsptRatio(-0.1f);
            break;
        default:
            break;
    }
    return TRUE;
}

BOOL Separate2D3DViewScene::handleMouseEvent(int button, int state, int x, int y)
{
    return TRUE;
}

BOOL Separate2D3DViewScene::handleMouseMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL Separate2D3DViewScene::handleMousePassiveMotionEvent(int x, int y)
{
    return TRUE;
}
