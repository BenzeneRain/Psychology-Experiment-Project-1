#include "stdafx.h"
#include "Scene.h"
#include "Screen.h"
#include "Experiment.h"
#include <vector>

using namespace std;

Scene::Scene()
{
    Scene::currScene = this;

    Experiment *pExperi = Experiment::getInstance(NULL);
    this->screens = pExperi->screens;
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

Scene *Scene::currScene;
