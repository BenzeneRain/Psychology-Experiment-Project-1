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
    // and reset all other functions, e.g. display
    for(vector<Screen *>::iterator it = this->screens.begin();
        it != this->screens.end(); it ++)
    {
        ((Screen *)*it)->resetAllFunc();
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIX: should not hard code texID[0]
    // and any other codes

    for(unsigned int i = 0; i < this->screens.size(); i ++)
    {
        int scrWidth = this->screens[i]->rDevMode.dmPelsWidth;
        int scrHeight = this->screens[i]->rDevMode.dmPelsHeight;
        GLfloat fAspect = (GLfloat)scrWidth / (GLfloat)scrHeight;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if(scrWidth <= scrHeight)
            glOrtho(-100.0, 100.0, -100.0/fAspect, 100.0/fAspect, 100.0, -100.0);
        else
            glOrtho(-100.0*fAspect, 100.0*fAspect, -100.0, 100.0, 100.0, -100.0);

        gluLookAt(0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_TEXTURE_2D);
        // Draw the cylinder before adjust in 2D
        glPushMatrix();
        glColor3ub(255, 255, 255);
        glTranslatef(0.0f, -20.0f, 0.0f);
        glScalef(1.0f, 1.0f, this->pObj->initZAsptRatio);
        this->pObj->draw(GLU_SILHOUETTE);
        glPopMatrix();

        // Draw the cylinder after adjust in 2D
        glPushMatrix();
        glColor3ub(255, 0, 0);
        glTranslatef(0.0f, -20.0f, 0.0f);
        glScalef(1.0f, 1.0f, this->pObj->adjZAsptRatio);
        this->pObj->draw(GLU_SILHOUETTE);
        glPopMatrix();

        this->screens[i]->render();
    }
    return TRUE;
}

BOOL Overlapped2DViewScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    if(w <= h)
        glOrtho(-100.0, 100.0, -100.0/fAspect, 100.0/fAspect, 100.0, -100.0);
    else
        glOrtho(-100.0*fAspect, 100.0*fAspect, -100.0, 100.0, 100.0, -100.0);

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

BOOL Overlapped2DViewScene::handleTimerEvent(int timerID)
{
    return TRUE;
}
