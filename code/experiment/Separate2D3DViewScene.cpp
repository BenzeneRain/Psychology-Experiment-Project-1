#include "stdafx.h"
#include "Separate2D3DViewScene.h"
#include "Trial.h"
#include "Experiment.h"


Separate2D3DViewScene::Separate2D3DViewScene(cond_t& cond):
    condition(cond), ROTATION_TIMERID(1), SWITCH_TIMERID(2),
    currStatus(DISPLAY_OBJECT) 
{
}

Separate2D3DViewScene::~Separate2D3DViewScene(void)
{
    // FIX: need to be uninterruptable
    Scene::unregisterTimer(this->ROTATION_TIMERID);
    Scene::unregisterTimer(this->SWITCH_TIMERID);
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
    this->rScreen.setMousePassiveMotionFunc(Scene::dispatchMousePassiveMotionEvent);
    this->rScreen.setMouseFunc(Scene::dispatchMouseEvent);
    //bind timer event
    
    Scene::registerTimer(this->ROTATION_TIMERID);

    GLfloat msecs;
    // Set the rotSpeed to 0 can make
    // the object stay still.
    if(rObject.rotSpeed != 0 && this->condition.dispMode == CONTINUOUS_DISPLAY)
    {
        msecs = 1000.0f / rObject.rotSpeed;
        this->rScreen.setTimerFunc((unsigned int)msecs,
                Scene::dispatchTimerEvent, this->ROTATION_TIMERID);
    }

    if(this->condition.dispMode == DISCRETE_DISPLAY)
    {
        Scene::registerTimer(this->SWITCH_TIMERID);
        this->rScreen.setTimerFunc((unsigned int)(this->condition.secDisplay * 1000.0f),
                Scene::dispatchTimerEvent, this->SWITCH_TIMERID);
    }


    // Start running the scene
    // FIX: This is actually a run design if there are multiple screens
    // e.g. the program will be blocked for each run()
    this->initDisplay();

    this->rScreen.startSampleFPS();

    QueryPerformanceCounter(&this->_startTime);
    this->rScreen.run();
    QueryPerformanceCounter(&this->_endTime);

    this->_duration = (float)(this->_endTime.QuadPart - this->_startTime.QuadPart) /
        (float)(this->rScreen.getCounterFrequency().QuadPart);

    this->rScreen.stopSampleFPS();

    this->_fps = this->rScreen.getFPS();

    return this->status;
}

BOOL Separate2D3DViewScene::renderScene()
{
    TestObject& rObject = *this->condition.pRealObject;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIX: should not hard code texID[0]
    // and any other codes

    int scrWidth = this->rScreen.rDevMode.dmPelsWidth;
    int scrHeight = this->rScreen.rDevMode.dmPelsHeight;
    GLfloat fAspect = (GLfloat)scrWidth / (GLfloat)scrHeight;

    glViewport(0, 0, scrWidth, scrHeight);
        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(this->rScreen.stereoFrame.centerprojmatrix.data());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(this->condition.dispMode == CONTINUOUS_DISPLAY ||
            (this->condition.dispMode == DISCRETE_DISPLAY &&
             this->currStatus == DISPLAY_OBJECT))
    {
        //////////////////////////////////////////////////////
        // Draw the 3D View

        glColor3ub(255, 255, 255);

        // Draw the cylinder in 3D view
        rObject.draw(GLU_FILL, TRUE, TRUE, TRUE, 1.0f, 1.0f, rObject.initZAsptRatio,
                this->condition.xyz3D[0],
                this->condition.xyz3D[1],
                this->condition.xyz3D[2]);

    }
    //////////////////////////////////////////////////////
    // Draw 2D View

    rObject.draw2D(GLU_FILL, 1.0f, 1.0f, rObject.adjZAsptRatio,
            this->condition.xyz2D[0],
            this->condition.xyz2D[1],
            this->condition.xyz2D[2]);

#ifdef _DEBUG
    ostringstream ossCurrRot;
    ossCurrRot << rObject.currRotDeg;
    this->rScreen.displayString(ossCurrRot.str(), 0.5, 0.5);
#endif

    this->rScreen.render();

    return TRUE;
}


BOOL Separate2D3DViewScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //gluPerspective(35.0f, fAspect, 0.01f, 50.0f);
    //gluLookAt(0.0f, 30.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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
                if(this->_isStopConditionSatisfied())
                    this->rScreen.stopped = TRUE;
                break;
            }
        case VK_ESCAPE:
            {
                int ret = MessageBox(NULL, "Do you want to abort the experiment?", "Abort", MB_YESNO | MB_ICONWARNING);
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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if(this->_isStopConditionSatisfied())
            this->rScreen.stopped = TRUE;
    }

    return TRUE;
}

BOOL Separate2D3DViewScene::handleMouseMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL Separate2D3DViewScene::handleMousePassiveMotionEvent(int x, int y)
{
    int lastY = this->rScreen.rDevMode.dmPelsHeight >> 1;
    TestObject& rObject = *this->condition.pRealObject;

    if(y > lastY)
    {
        rObject.adjustAsptRatio(0.01f);
        glutWarpPointer(0, lastY);
    }
    else if(y < lastY)
    {
        rObject.adjustAsptRatio(-0.01f);
        glutWarpPointer(0, lastY);
    }
    else
    {
    }
    return TRUE;
}

BOOL Separate2D3DViewScene::handleTimerEvent(int timerID)
{
    GLfloat step = 1.0f;
    TestObject& rObject = *this->condition.pRealObject;

    if(timerID == this->ROTATION_TIMERID && 
            this->condition.dispMode == CONTINUOUS_DISPLAY)
    {
        GLfloat msecs;
        step = 1.0f;
        if(rObject.rotSpeed != 0.0f)
            msecs = 1000.0f / rObject.rotSpeed;
        else 
            return TRUE;

        rObject.rotate(step);

        this->rScreen.setTimerFunc((unsigned int)msecs,
                Scene::dispatchTimerEvent, this->ROTATION_TIMERID);

    }
    else if(this->condition.dispMode == DISCRETE_DISPLAY &&
            timerID == this->SWITCH_TIMERID)
    {
        
        step = (this->condition.secDisplay + this->condition.secBlackScreen) * rObject.rotSpeed;

        if(this->currStatus == DISPLAY_OBJECT)
        {
            if(this->condition.secBlackScreen != 0.0f)
            {
                this->currStatus = DISPLAY_BLACKSCREEN;
                this->rScreen.setTimerFunc((unsigned int)(this->condition.secBlackScreen * 1000.0f),
                        Scene::dispatchTimerEvent, this->SWITCH_TIMERID);
            }
            else
            {
                rObject.rotate(step);
                this->rScreen.setTimerFunc((unsigned int)(this->condition.secDisplay * 1000.0f),
                        Scene::dispatchTimerEvent, this->SWITCH_TIMERID);
            }
        }
        else
        {
            this->currStatus = DISPLAY_OBJECT;
            rObject.rotate(step);
            this->rScreen.setTimerFunc((unsigned int)(this->condition.secDisplay * 1000.0f),
                    Scene::dispatchTimerEvent, this->SWITCH_TIMERID);
        }
    }
    else
    {
    }

return TRUE;
}
