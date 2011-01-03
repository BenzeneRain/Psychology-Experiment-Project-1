#pragma once

#include "Screen.h"
#include <vector>

using namespace std;

class Scene
{
    public:
        virtual ~Scene(void) = 0;
        virtual BOOL startScene() = 0;

        static void dispatchSceneRender();
        static void dispatchReshape(int w, int h);
         
        vector<Screen *> screens;

    protected:
        Scene();
        virtual BOOL renderScene() = 0;
        virtual BOOL reshape(int w, int h) = 0;

        static Scene *currScene;
};
