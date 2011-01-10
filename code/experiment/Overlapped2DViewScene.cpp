#include "stdafx.h"
#include "Overlapped2DViewScene.h"

Overlapped2DViewScene::Overlapped2DViewScene(void)
{
}

Overlapped2DViewScene::Overlapped2DViewScene(TestObject &rObject)
{
    this->pObj = rObject.newObj(rObject);
}

Overlapped2DViewScene::~Overlapped2DViewScene(void)
{
    delete this->pObj;
}

BOOL Overlapped2DViewScene::startScene()
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

BOOL Overlapped2DViewScene::renderScene()
{
    return TRUE;
}

BOOL Overlapped2DViewScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(35.0f, fAspect, 0.01f, 50.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return TRUE;
}

BOOL Overlapped2DViewScene::initDisplay(Screen& scr)
{
    this->reshape(scr.rDevMode.dmPelsWidth, scr.rDevMode.dmPelsHeight); 

    // Disable texture
    glDisable(GL_TEXTURE_2D);
    
    // Disable multisample
    glEnable(GL_MULTISAMPLE);

    return TRUE;
}

BOOL Overlapped2DViewScene::handleKeyboardEvent(unsigned char key, int x, int y)
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

                //TODO: If in experiment mode, write the test outputs

                break;
            }
        default:
            break;
    }
    return TRUE;
}

BOOL Overlapped2DViewScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    return TRUE;
}

BOOL Overlapped2DViewScene::handleMouseEvent(int button, int state, int x, int y)
{
    return TRUE;
}

BOOL Overlapped2DViewScene::handleMouseMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL Overlapped2DViewScene::handleMousePassiveMotionEvent(int x, int y)
{
    return TRUE;

}
