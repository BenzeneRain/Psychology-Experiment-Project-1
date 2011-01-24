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
    this->rScreen.setMouseFunc(Scene::dispatchMouseEvent);

    // Start running the scene
    // FIX: This is actually a run design if there are multiple screens
    // e.g. the program will be blocked for each run()
    this->initDisplay();
    this->rScreen.run();

    return this->status;
}

BOOL Overlapped2DViewScene::renderScene()
{
    TestObject& rObject = *this->condition.pRealObject;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FIX: should not hard code texID[0]
    // and any other codes

    int scrWidth = this->rScreen.rDevMode.dmPelsWidth;
    int scrHeight = this->rScreen.rDevMode.dmPelsHeight;
    GLfloat fAspect = (GLfloat)scrWidth / (GLfloat)scrHeight;

    glViewport(0, 0, scrWidth, scrHeight);
    //////////////////////////////////////////////////////
    // Draw the 3D Object

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //gluPerspective(60.0f, fAspect, 0.01f, 300.0f);
    //gluLookAt(0.0f, 0.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glLoadMatrixd(this->rScreen.stereoFrame.centerprojmatrix.data());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(255, 255, 255);

    // Draw the cylinder in 3D view
    rObject.draw(GLU_FILL, TRUE, TRUE, TRUE, 1.0f, 1.0f, rObject.initZAsptRatio,
            this->condition.xyz3D[0],
            this->condition.xyz3D[1],
            this->condition.xyz3D[2]);

    //////////////////////////////////////////////////////
    // Draw the 2D Object

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//
//    float objWidth = 30.0f;
//    float objHeight = 30.0f;
//    float objDepth = 30.0f;
//
//    if(scrWidth <= scrHeight)
//        glOrtho(-objWidth, objWidth, -objHeight/fAspect, objHeight/fAspect, -objDepth, objDepth);
//    else
//        glOrtho(-objWidth*fAspect, objWidth*fAspect, -objHeight, objHeight, -objDepth, objDepth);
//    
//    gluLookAt(this->rScreen.stereoFrame.topCenterEye[0],
//        this->rScreen.stereoFrame.topCenterEye[1],
//        this->rScreen.stereoFrame.topCenterEye[2],
//        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    glPushMatrix();
//    glTranslatef(25.0f, 0.0f, 0.0f);
//    // Draw the cylinder after adjust in 2D
//    glColor3ub(255, 255, 255);
//    rObject.draw(GLU_FILL, FALSE, FALSE, FALSE, rObject.adjZAsptRatio, 0.0f);
//
//    // Draw the cylinder before adjust in 2D
//    glColor3ub(255, 0, 0);
//    //glTranslatef(0.0f, -1.0f, 0.0f);
//    glEnable(GL_LINE_STIPPLE);
//    glLineStipple(1, 0xf0f0);
//
//    GLfloat normalLineSize[2];
//    glGetFloatv(GL_LINE_WIDTH_RANGE, normalLineSize);
//    glLineWidth(3.0f);
//    rObject.draw(GLU_SILHOUETTE, FALSE, FALSE, FALSE, rObject.initZAsptRatio, 1.0f);
//    glLineWidth(normalLineSize[0]);
//    glDisable(GL_LINE_STIPPLE);
//    glPopMatrix(); 

    glColor3ub(255, 0, 0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xf0f0);

    GLfloat normalLineSize[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, normalLineSize);
    glLineWidth(3.0f);
    rObject.draw2D(GLU_SILHOUETTE, 1.0f, 1.0f, rObject.initZAsptRatio,
            this->condition.xyz2D[0],
            this->condition.xyz2D[1],
            this->condition.xyz2D[2]);
    glLineWidth(normalLineSize[0]);
    glDisable(GL_LINE_STIPPLE);

    glColor3ub(255, 255, 255);
    rObject.draw2D(GLU_FILL, 1.0f, 1.0f, rObject.adjZAsptRatio,
            this->condition.xyz2D[0],
            this->condition.xyz2D[1],
            this->condition.xyz2D[2]);

    this->rScreen.render();

    return TRUE;
}

BOOL Overlapped2DViewScene::reshape(int w, int h)
{
    glViewport(0, 0, w, h);

    GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    //if(w <= h)
    //    glOrtho(-100.0, 100.0, -100.0/fAspect, 100.0/fAspect, 100.0, -100.0);
    //else
    //    glOrtho(-100.0*fAspect, 100.0*fAspect, -100.0, 100.0, 100.0, -100.0);

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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        this->rScreen.stopped = TRUE;
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
