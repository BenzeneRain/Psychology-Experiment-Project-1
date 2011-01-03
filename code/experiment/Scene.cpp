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

void Scene::dispatchReshape(int w, int h)
{
    Scene::currScene->reshape(w, h);
}

Scene *Scene::currScene;
