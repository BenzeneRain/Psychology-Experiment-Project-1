#include "stdafx.h"
#include "Separate2D3DViewScene.h"
#include "Trial.h"
#include "Experiment.h"

const int Separate2D3DViewScene::TIMERID = 1;

Separate2D3DViewScene::Separate2D3DViewScene(cond_t& cond):
    condition(cond)
{
}

Separate2D3DViewScene::~Separate2D3DViewScene(void)
{
}

BOOL Separate2D3DViewScene::startScene()
{
    // Cancel all keyboards and mouses events bindings
    // and reset all other functions, e.g. display
    this->rScreen.resetAllFunc();

    // Get the random object
    TestObject& rObject = *this->condition.pRealObject;
    if(Experiment::debug)
    {
        MessageBox(NULL, (LPCSTR)((rObject.getObjName()).c_str()), NULL, NULL);
    }

    // Clear the screen
    this->rScreen.clear();

    // set display function and reshape function
    this->rScreen.setDisplayFunc(Scene::dispatchSceneRender);
    this->rScreen.setReshapeFunc(Scene::dispatchReshape);

    // Bind new keyboards and mouses events
    this->rScreen.setKeyboardFunc(Scene::dispatchKeyboardEvent);
    this->rScreen.setKeyboardSpecialFunc(Scene::dispatchKeyboardSpecialEvent);

    //bind timer event
    GLfloat msecs;
    // Set the rotSpeed to 0 can make
    // the object stay still.
    if(rObject.rotSpeed != 0)
    {
        msecs = 1000.0f / rObject.rotSpeed;
        this->rScreen.setTimerFunc((unsigned int)msecs,
                Scene::dispatchTimerEvent, Separate2D3DViewScene::TIMERID);
    }

    // Start running the scene
    // FIX: This is actually a run design if there are multiple screens
    // e.g. the program will be blocked for each run()
    this->initDisplay();
    this->rScreen.run();

    return TRUE;
}

BOOL Separate2D3DViewScene::renderScene()
{
    TestObject& rObject = *this->condition.pRealObject;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIX: should not hard code texID[0]
    // and any other codes

    int scrWidth = this->rScreen.rDevMode.dmPelsWidth;
    int scrHeight = this->rScreen.rDevMode.dmPelsHeight;
    int halfWidth = scrWidth >> 1;
    GLfloat fAspect = (GLfloat)halfWidth / (GLfloat)scrHeight;

    //////////////////////////////////////////////////////
    // Draw the left part
    glViewport(0, 0, halfWidth, scrHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0f, fAspect, 0.01f, 300.0f);
    gluLookAt(0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(255, 255, 255);

    // Draw the cylinder in 3D view
    glPushMatrix();
    glScalef(1.0f, 1.0f, rObject.initZAsptRatio);
    rObject.draw(GLU_FILL, TRUE, TRUE, TRUE, 0.0f);
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

    gluLookAt(0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(255, 255, 255);

    glPushMatrix();
    //Draw the cylinder in 2D
    glScalef(1.0f, 1.0f, rObject.adjZAsptRatio);
    rObject.draw(GLU_FILL, FALSE, FALSE, FALSE, 0.0f);

    glPopMatrix();

    //////////////////////////////////////////////////////
    // Switch to the full screen and display
    glViewport(0, 0, scrWidth, scrHeight);

    this->rScreen.render();

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

BOOL Separate2D3DViewScene::initDisplay()
{
    this->reshape(this->rScreen.rDevMode.dmPelsWidth,
            this->rScreen.rDevMode.dmPelsHeight); 

    glEnable(GL_MULTISAMPLE);

    return TRUE;
}

BOOL Separate2D3DViewScene::handleKeyboardEvent(unsigned char key, int x, int y)
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

BOOL Separate2D3DViewScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    TestObject& rObject = *this->condition.pRealObject;

    switch(key)
    {
        case GLUT_KEY_UP:
            rObject.adjustAsptRatio(0.01f);
            break;
        case GLUT_KEY_DOWN:
            rObject.adjustAsptRatio(-0.01f);
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

BOOL Separate2D3DViewScene::handleTimerEvent(int timerID)
{
    GLfloat step = 1.0f;
    TestObject& rObject = *this->condition.pRealObject;

    if(timerID == Separate2D3DViewScene::TIMERID)
    {
        if((rObject.rotDirection == TestObject::CLOCKWISE &&
                    rObject.currRotDeg - step < -(rObject.maxRotDeg)) ||
                (rObject.rotDirection == TestObject::COUNTERCLOCKWISE && 
                 rObject.currRotDeg + step > rObject.maxRotDeg))
        {
            rObject.reverseRotDirection(); 
        }

        if(rObject.rotDirection == TestObject::CLOCKWISE)
            rObject.currRotDeg -= step;
        else
            rObject.currRotDeg += step;

        //bind timer event
        GLfloat msecs;

        // Set the rotSpeed to 0 can make
        // the object stay still.
        if(rObject.rotSpeed != 0)
        {
            msecs = 1000.0f / rObject.rotSpeed;
            this->rScreen.setTimerFunc((unsigned int)msecs,
                    Scene::dispatchTimerEvent, Separate2D3DViewScene::TIMERID);
        }
    }  

    return TRUE;
}
