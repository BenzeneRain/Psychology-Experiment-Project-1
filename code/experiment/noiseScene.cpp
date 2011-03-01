#include "stdafx.h"
#include "noiseScene.h"

noiseScene::noiseScene(float duration):
    _duration(duration)
{
}

noiseScene::~noiseScene(void)
{
    Scene::unregisterTimer(this->_timer_id);
}

BOOL noiseScene::startScene()
{
    // Cancel all keyboards and mouses events bindings
    // and reset all other functions, e.g. display
    this->rScreen.resetAllFunc();

    // Clear the screen
    this->rScreen.clear();

    // set display function and reshape function
    this->rScreen.setDisplayFunc(Scene::dispatchSceneRender);
    this->rScreen.setReshapeFunc(Scene::dispatchReshape);
     

    // Set the timer for showing the noise
    this->_timer_id = Scene::registerTimer();
    this->rScreen.setTimerFunc((unsigned int)(this->_duration * 1000.0f),
        Scene::dispatchTimerEvent, this->_timer_id);

    // Start running the scene
    // FIX: This is actually a run design if there are multiple screens
    // e.g. the program will be blocked for each run()
    this->initDisplay();
    this->rScreen.run();

    return this->status;
}

BOOL noiseScene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int scrWidth = this->rScreen.rDevMode.dmPelsWidth;
    int scrHeight = this->rScreen.rDevMode.dmPelsHeight;
    GLfloat fAspect = (GLfloat)scrWidth / (GLfloat)scrHeight;

    glViewport(0, 0, scrWidth, scrHeight);

    //////////////////////////////////////////////////////
    // Draw the 2D Object

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat xMax, xMin;
    GLfloat yMax, yMin;

    if(scrWidth <= scrHeight)
    {
        xMax = 100.0f;
        xMin = -100.0f;
        yMax = 100.0f/fAspect;
        yMin = -100.0f/fAspect;
    }
    else
    {
        xMax = 100.0f*fAspect;
        xMin = -100.0f*fAspect;
        yMax = 100.0f;
        yMin = -100.0f;
    }
       
    glOrtho(xMin, xMax, yMin, yMax, 1, -1);
    
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, *this->rScreen.noiseTex);

    glBegin(GL_QUADS);

    
    glVertex2f(xMin, yMin);
    glTexCoord2f(0.0, 0.0);

    glVertex2f(xMin, yMax);
    glTexCoord2f(0.0, 1.0);

    glVertex2f(xMax, yMax);
    glTexCoord2f(1.0, 1.0);

    glVertex2f(xMax, yMin);
    glTexCoord2f(1.0, 0.0);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix(); 

    this->rScreen.render();

    return TRUE;
}

BOOL noiseScene::reshape(int w, int h)
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

BOOL noiseScene::initDisplay()
{
    this->reshape(this->rScreen.rDevMode.dmPelsWidth,
            this->rScreen.rDevMode.dmPelsHeight); 

    // Disable multisample
    glDisable(GL_MULTISAMPLE_ARB);

    return TRUE;
}

BOOL noiseScene::handleKeyboardEvent(unsigned char key, int x, int y)
{
    return TRUE;
}

BOOL noiseScene::handleKeyboardSpecialEvent(int key, int x, int y)
{
    return TRUE;
}

BOOL noiseScene::handleMouseEvent(int button, int state, int x, int y)
{
    return TRUE;
}

BOOL noiseScene::handleMouseMotionEvent(int x, int y)
{
    return TRUE;
}

BOOL noiseScene::handleMousePassiveMotionEvent(int x, int y)
{
    return TRUE;

}

BOOL noiseScene::handleTimerEvent(int timerID)
{
    if(timerID ==  this->_timer_id)
    {
        this->rScreen.stopped = TRUE;
    }
    return TRUE;
}
