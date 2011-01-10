#include "stdafx.h"
#include "Separate2D3DViewScene.h"
#include "Trial.h"
#include "Experiment.h"

Separate2D3DViewScene::Separate2D3DViewScene(void)
{
}

Separate2D3DViewScene::~Separate2D3DViewScene(void)
{
    delete this->pObj;
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

BOOL Separate2D3DViewScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIX: should not hard code texID[0]
    // and any other codes
    
    for(unsigned int i = 0; i < this->screens.size(); i ++)
    {
        int scrWidth = this->screens[i]->rDevMode.dmPelsWidth;
        int scrHeight = this->screens[i]->rDevMode.dmPelsHeight;
        int halfWidth = scrWidth >> 1;
        GLfloat fAspect = (GLfloat)halfWidth / (GLfloat)scrHeight;

        //////////////////////////////////////////////////////
        // Draw the left part
        glViewport(0, 0, halfWidth, scrHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(60.0f, fAspect, 0.01f, 300.0f);
        gluLookAt(0.0f, 120.0f, 150.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3ub(255, 255, 255);
        
        glEnable(GL_TEXTURE_2D);
        // Draw the cylinder in 3D view
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, this->screens[i]->texIDs[0]);
        glTranslatef(0.0f, -20.0f, 0.0f);
        glScalef(1.0f, 1.0f, this->pObj->initZAsptRatio);
        this->pObj->draw();
        glPopMatrix();

        //////////////////////////////////////////////////////
        // Draw the right part
        glViewport(halfWidth + 1, 0, halfWidth, scrHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if(halfWidth <= scrHeight)
            glOrtho(-100.0, 100.0, -100.0/fAspect, 100.0/fAspect, 100.0, -100.0);
        else
            glOrtho(-100.0*fAspect, 100.0*fAspect, -100.0, 100.0, 100.0, -100.0);

        gluLookAt(0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3ub(255, 255, 255);

        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        // Draw the cylinder in 2D
        glScalef(1.0f, 1.0f, this->pObj->adjZAsptRatio);
        this->pObj->draw();
        
        glPopMatrix();

        //////////////////////////////////////////////////////
        // Switch to the full screen and display
        glViewport(0, 0, scrWidth, scrHeight);

        this->screens[i]->render();
    }
    return TRUE;
}


BOOL Separate2D3DViewScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(35.0f, fAspect, 0.01f, 50.0f);
    gluLookAt(0.0f, 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return TRUE;
}

BOOL Separate2D3DViewScene::initDisplay(Screen& scr)
{
    this->reshape(scr.rDevMode.dmPelsWidth, scr.rDevMode.dmPelsHeight); 

    glEnable(GL_TEXTURE_2D);

    //glDisable(GL_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
    //glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glEnable(GL_POLYGON_SMOOTH);

    return TRUE;
}

BOOL Separate2D3DViewScene::handleKeyboardEvent(unsigned char key, int x, int y)
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
