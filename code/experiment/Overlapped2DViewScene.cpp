#include "stdafx.h"
#include "Overlapped2DViewScene.h"

Overlapped2DViewScene::Overlapped2DViewScene(cond_t& cond):
    condition(cond)
{
}

Overlapped2DViewScene::~Overlapped2DViewScene(void)
{
}

BOOL Overlapped2DViewScene::startScene()
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

    // Start running the scene
    // FIX: This is actually a run design if there are multiple screens
    // e.g. the program will be blocked for each run()
    this->initDisplay();
    this->rScreen.run();

    return TRUE;
}

BOOL Overlapped2DViewScene::renderScene()
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
    gluLookAt(0.0f, 120.0f, 150.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(255, 255, 255);

    // Draw the cylinder in 3D view
    glPushMatrix();
    glTranslatef(0.0f, -20.0f, 0.0f);
    glScalef(1.0f, 1.0f, rObject.initZAsptRatio);
    glRotatef(rObject.currRotDeg, 0.0f, 1.0f, 0.0f);
    rObject.draw(GLU_FILL, TRUE);
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

    // Draw the cylinder after adjust in 2D
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(0.0f, -20.0f, 0.0f);
    glScalef(1.0f, 1.0f, rObject.adjZAsptRatio);
    rObject.draw(GLU_FILL, FALSE);
    glPopMatrix();

    // Draw the cylinder before adjust in 2D
    glPushMatrix();
    glColor3ub(255, 0, 0);
    //glTranslatef(0.0f, -20.0f, 0.0f);
    glScalef(1.0f, 1.0f, rObject.initZAsptRatio);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xf0f0);

    GLfloat normalLineSize[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, normalLineSize);
    glLineWidth(3.0f);
    rObject.draw(GLU_SILHOUETTE, FALSE);
    glLineWidth(normalLineSize[0]);
    glDisable(GL_LINE_STIPPLE);
    glPopMatrix();

    //////////////////////////////////////////////////////
    // Switch to the full screen and display
    glViewport(0, 0, scrWidth, scrHeight);

    this->rScreen.render();

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

BOOL Overlapped2DViewScene::initDisplay()
{
    this->reshape(this->rScreen.rDevMode.dmPelsWidth,
            this->rScreen.rDevMode.dmPelsHeight); 

    // Enable multisample
    glEnable(GL_MULTISAMPLE);

    return TRUE;
}

BOOL Overlapped2DViewScene::handleKeyboardEvent(unsigned char key, int x, int y)
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
