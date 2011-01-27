#include "stdafx.h"
#include "Scene.h"
#include "Screen.h"
#include "Experiment.h"
#include <vector>
#include <hash_map>

using namespace stdext;

Scene::Scene():
    rScreen(*Experiment::getInstance(NULL)->pScreen)
{
    Scene::currScene = this;
    this->status = TRUE;

    this->_duration = 0;
    this->_minDuration = 0;
}

Scene::~Scene()
{
}

void Scene::dispatchSceneRender()
{
     Scene::currScene->renderScene(); 
}

void Scene::dispatchKeyboardEvent(unsigned char key, int x, int y)
{
    Scene::currScene->handleKeyboardEvent(key, x, y);
}

void Scene::dispatchKeyboardSpecialEvent(int key, int x, int y)
{
    Scene::currScene->handleKeyboardSpecialEvent(key, x, y);
}

void Scene::dispatchMouseEvent(int button, int state, int x, int y)
{
    Scene::currScene->handleMouseEvent(button, state, x, y);
}

void Scene::dispatchMouseMotionEvent(int x, int y)
{
    Scene::currScene->handleMouseMotionEvent(x, y);
}

void Scene::dispatchMousePassiveMotionEvent(int x, int y)
{
    Scene::currScene->handleMousePassiveMotionEvent(x, y);
}

void Scene::dispatchReshape(int w, int h)
{
    Scene::currScene->reshape(w, h);
}

void Scene::dispatchTimerEvent(int timerID)
{
    if(Scene::isRegisteredTimer(timerID))
        Scene::currScene->handleTimerEvent(timerID);
}

int Scene::registerTimer()
{
    // FIX: NOT Thread safe
    int timerID = Scene::getFreeTimerID();

    Scene::registeredTimerID[timerID] = TRUE;
    return timerID;
}

void Scene::unregisterTimer(int timerID)
{
    if(Scene::registeredTimerID.find(timerID) != Scene::registeredTimerID.end())
        if(Scene::registeredTimerID[timerID] == TRUE)
            {
                Scene::registeredTimerID[timerID] = FALSE;
                //KillTimer(Scene::currScene->rScreen.hWnd(), timerID);
            }
}

BOOL Scene::isRegisteredTimer(int timerID)
{
    if(Scene::registeredTimerID.find(timerID) != Scene::registeredTimerID.end())
        return Scene::registeredTimerID[timerID];
    else
        return FALSE;
}

int Scene::getFreeTimerID()
{
    int timerID;

    do
    {
        timerID = (rand() % 997) + 1;
    }while(Scene::isRegisteredTimer(timerID) != FALSE);

    return timerID;
}

void Scene::reset()
{
    Scene::registeredTimerID.clear();
}

Scene *Scene::currScene;
hash_map<int, BOOL> Scene::registeredTimerID;
